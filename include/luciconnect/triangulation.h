#ifndef luciconnect_TRIANGULATION_HPP
#define luciconnect_TRIANGULATION_HPP

#include <vector>
#include <numeric> // iota
#include <algorithm> // sort, rotate
#include <iostream> // TODO: Remove

#include "luciconnect/geometry.h"

namespace luciconnect {
  namespace triangulation {
    /**
     * Inverted base change using orthogonal basis of Subvectorspace spanned by polygon
     */
    inline std::vector<vec3> to3d(std::vector<vec2> points, std::vector<vec3> basis, vec3 origin) {
      std::vector<vec3> points3d (points.size());
      for (size_t i = 0; i < points.size(); i++) {
        // multiply by transposed (=inverted) base change matrix
        points3d[i] = origin + basis[0]*points[i].x + basis[1]*points[i].y;
      }

      return points3d;
    }

    /**
     * Base change using orthogonal basis of Subvectorspace spanned by polygon
     */
    inline std::vector<vec2> to2d(std::vector<vec3> points, std::vector<vec3> basis, vec3 origin) {
      std::vector<vec2> points2d(points.size());
      for (size_t i = 0; i < points.size(); i++) {
        // multiply by base change matrix (row-wise orthogonal vectors)
        vec3 v = points[i] - origin;
        points2d[i] = {basis[0]*v, basis[1]*v};
      }

      return points2d;
    }

    /**
     * Gram-Schmidt orthogonalization, returns the orthogonal basis of the 2d-subspace
     */
    inline std::vector<vec3> gramschmidt(std::vector<vec3> points) {
      std::vector<vec3> b(2);
      vec3 v1 = points[2] - points[1];
      b[0] = points[1] - points[0];
      b[1] = v1 - b[0]*((v1*b[0])/(b[0]*b[0]));

      b[0] = b[0] / abs(b[0]);
      b[1] = b[1] / abs(b[1]);

      return b;
    }

    inline bool intriangle2d(vec2 p, vec2 t1, vec2 t2, vec2 t3) {
      bool b1, b2, b3;
      b1 = ccw(p, t1, t2) < 0;
      b2 = ccw(p, t2, t3) < 0;
      b3 = ccw(p, t3, t1) < 0;

      return ((b1 == b2) && (b2 == b3));
    }

    /**
     * Augments the polygon with edges to create a suitible structure to use the
     * ear-clipping algorithm
     */
    inline std::vector<vec2> join_holes(std::vector<vec2> points) {
      // TODO: Allow nested holes
      // divide the point sequence into one outer polygon and multiple inner polygons (holes)
      std::vector<vec2> outer_polygon = {};
      std::vector<std::vector<vec2>> inner_polygons = {};
      size_t polygon_index = 0;
      for (size_t i = 1; i < points.size(); i++) {
        if (points[i] == points[polygon_index]) {
          if (polygon_index == 0) {
            outer_polygon = std::vector<vec2> {points.begin()+polygon_index, points.begin()+i};
          }
          else {
            inner_polygons.push_back(std::vector<vec2> {points.begin()+polygon_index, points.begin()+i});
          }
          polygon_index = ++i;
        }
      }

      // for each inner polygon, sort its components by x and rotate it such
      // that the rightmost vertex is at the first place
      std::vector<size_t> rightmost_vertex(inner_polygons.size());
      for (size_t i = 0; i < inner_polygons.size(); i++) {
        size_t max_index = 0;
        float max_x = FLT_MIN;
        for (size_t j = 0; j < inner_polygons[i].size(); j++) {
          if (inner_polygons[i][j].x > max_x) {
            max_x = inner_polygons[i][j].x;
            max_index = j;
          }
        }

        std::rotate(
          inner_polygons[i].begin(),
          inner_polygons[i].begin()+max_index,
          inner_polygons[i].end()
        );
      }

      // sort the inner polygons by their maximum x value (descending)
      std::vector<int> inner_order(inner_polygons.size());
      std::iota(inner_order.begin(), inner_order.end(), 0);
      std::sort(
        inner_order.begin(),
        inner_order.end(),
        [&inner_polygons](size_t i, size_t j) -> bool {
          return inner_polygons[i][0].x > inner_polygons[j][0].x;
        }
      );

      // combine outer polygon P0 with first hole H0 to P1
      // compine P1 with second hole H1 to P2
      // continue until all holes are contained in P
      std::vector<vec2> polygon = outer_polygon;
      for (int i : inner_order) {
        vec2 inner_point = inner_polygons[i][0];

        // find the closest point on the outer polygon
        // that is to the right of the inner point
        // TODO: This can be buggy for certain cases (e.g. a spiral)
        size_t closest_index = 0;
        float min_distance = FLT_MAX;
        for (size_t j = 0; j < polygon.size(); j++) {
          if (polygon[j].x < inner_point.x) continue;

          float distance = abs(inner_point - polygon[j]);
          if (distance < min_distance) {
            min_distance = distance;
            closest_index = j;
          }
        }

        // augment the polygon
        polygon.insert(polygon.begin()+closest_index+1, inner_polygons[i].begin(), inner_polygons[i].end());
        polygon.insert(polygon.begin()+closest_index+1+inner_polygons[i].size(), inner_polygons[i][0]);
        polygon.insert(polygon.begin()+closest_index+1+inner_polygons[i].size()+1, polygon[closest_index]);
      }

      return polygon;
    }

    /**
     * Triangulates a 2d polygon with one level of holes
     */
    inline std::vector<vec2> triangulate2d(std::vector<vec2> points) {
      points = join_holes(points);
      std::vector<vec2> triangles = {};
      std::vector<vec2> unmarked { points.begin(), points.end() };


      vec2 p0, p1, p2;
      int index1 = 0;
      int index2 = 1;
      int index3 = 2;
      while (unmarked.size() > 3) {
        p0 = unmarked[index1];
        p1 = unmarked[index2];
        p2 = unmarked[index3];

        bool found_removable_ear = false;
        // check if we found a counter-clockwise triangle
        if (ccw(p0, p1, p2) > 0) {
          found_removable_ear = true;
          // check if triangle is an ear
          for (vec2 pi : unmarked) {
            if (pi == p0 || pi == p1 || pi == p2) continue;

            if (intriangle2d(pi, p0, p1, p2)) {
              found_removable_ear = false;
              break;
            }
          }
        }

        // If it was an ear, we clip it
        if (found_removable_ear) {
          triangles.push_back(p0);
          triangles.push_back(p1);
          triangles.push_back(p2);

          // Now remove the 2nd vertex of this ear and adjust the indices
          unmarked.erase(unmarked.begin() + index2);
          index1 %= unmarked.size();
          index2 %= unmarked.size();
          index3 %= unmarked.size();
        }
        else {
          index1 = index2;
          index2 = index3;
          index3 = (index3 +1 )% unmarked.size();
        }
      }
      triangles.insert(triangles.end(), unmarked.begin(), unmarked.end());

      return triangles;
    }

    inline std::vector<vec3> triangulate(std::vector<vec3> points) {
      if (points.size() < 4) return points;

      // First, we compute the orthogonal basis of the points'
      // subvectorspace.
      // TODO: Note that if we have a non-planar
      // polygon, there are up to points/3 bases and the polygon
      // needs to be cut down
      std::vector<vec3> basis = gramschmidt(points);

      // Next, we do a basis transform and triangulate the polygon on the xy-axis
      std::vector<vec2> points2d = to2d(points, basis, points[0]);
      std::vector<vec2> triangles2d = triangulate2d(points2d);

      // And revert back to 3d coordinates (inverse base transform)
      return to3d(triangles2d, basis, points[0]);
    }
  }
}

#endif //TRIANGULATION_HPP
