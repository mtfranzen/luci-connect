#ifndef luciconnect_quaview_SERVICE_H_
#define luciconnect_quaview_SERVICE_H_

#include "connection.h"
#include "message.h"
#include "node.h"
#include "geojson.h"

namespace luciconnect {
  namespace quaview {
    class Service : public luciconnect::Node {
    public:
      Service(std::shared_ptr<Connection> connection);
      void Run() override;

    protected:
      json inputs;
      json outputs;
      bool supports_point_mode;

      void HandleRun(int64_t callId, std::string serviceName, json inputs = {}, std::vector<Attachment*> attachments = {}) override;
      void HandleCancel(int64_t callId) override;
      void HandleResult(int64_t callId, json result, std::vector<Attachment*> attachments = {}) override;
      void HandleProgress(int64_t callId, int64_t percentage, std::vector<Attachment*> attachments = {}, json intermediateResult = {}) override;
      void HandleError(int64_t callId, std::string error) override;

      virtual std::vector<float> ComputeOnPoints(std::vector<vec3> scenario_triangles, std::vector<vec3> points) = 0; // TODO

    private:
      void Register();

      std::mutex is_busy;
      bool terminated;
      bool waiting_for_registration;
      bool waiting_for_scenario;
      int64_t client_callid;
      int64_t scenario_callid;
      int64_t register_callid;

      std::string client_mode;
      int64_t client_scenario_id;
      std::vector<vec3> client_points;
      std::vector<int64_t> client_objects;
    };
  }
}

#endif
