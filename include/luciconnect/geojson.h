#ifndef luciconnect_GEOJSON_H
#define luciconnect_GEOJSON_H

#include "luciconnect/json/json.hpp"
#include "luciconnect/geometry.h"
#include "luciconnect/triangulation.h"
#include <string>

using json = nlohmann::json;

namespace luciconnect {
  namespace geojson {
    std::vector<vec3> triangulate_polygon(json js) {
      std::vector<vec3> points = {};
      std::vector<vec3> triangles = {};

      for (auto& ring : js) {
        for (auto& point : ring) {
          points.push_back({point[0], point[1], point[2]});
        }
      }
      return triangulation::triangulate(points);
    }

    std::vector<vec3> get_triangles(json js) {
      std::vector<vec3> triangles = {};

      if (js.count("type") > 0) {
         if (js["type"] == "FeatureCollection") {
           for (auto& feature : js["features"]) {
             std::vector<vec3> new_triangles = get_triangles(feature);
             triangles.insert(triangles.end(), new_triangles.begin(), new_triangles.end());
           }
         }
         else if (js["type"] == "Feature") {
           std::vector<vec3> new_triangles = get_triangles(js["geometry"]);
           triangles.insert(triangles.end(), new_triangles.begin(), new_triangles.end());
         }
         else if (js["type"] == "MultiPolygon") {
           for (auto& polygon : js["coordinates"]) {
             std::vector<vec3> new_triangles = triangulate_polygon(polygon);
             triangles.insert(triangles.end(), new_triangles.begin(), new_triangles.end());
           }
         }
         else if (js["type"] == "Polygon") {
           std::vector<vec3> new_triangles = triangulate_polygon(js["coordinates"]);
           triangles.insert(triangles.end(), new_triangles.begin(), new_triangles.end());
         }
      }

      return triangles;
    }

    std::vector<vec3> parse(std::string text) {
      json js = json::parse(text);
      std::vector<vec3> triangles = get_triangles(js);
      return triangles;
    }
  }
}

#endif // GEOJSON_HPP
