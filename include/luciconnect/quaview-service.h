#ifndef luciconnect_quaview_SERVICE_H_
#define luciconnect_quaview_SERVICE_H_

#include "connection.h"
#include "message.h"
#include "node.h"
#include "geojson.h"

#include <chrono>
#include <thread>

namespace luciconnect {
  namespace quaview {
    // TODO Comment
    class Service : public luciconnect::Node {
    public:

      // TODO Comment
      Service(std::shared_ptr<Connection> connection);

      // TODO Comment
      void Run() override;

      virtual std::string GetName() = 0;

      virtual std::string GetDescription() = 0;

      virtual std::string GetUnit() = 0;

      virtual json GetInputs() = 0;

      virtual json GetConstraints() = 0;

      virtual bool SupportsPointMode() = 0;

      // TODO Comment
      virtual std::vector<float> ComputeOnPoints(std::vector<vec3> scenario_triangles, std::vector<vec3> points, json inputs) = 0; // TODO

    protected:
      void HandleRun(int64_t callId, std::string serviceName, json inputs = {}, std::vector<Attachment*> attachments = {}) override;
      void HandleCancel(int64_t callId) override;
      void HandleResult(int64_t callId, json result, std::vector<Attachment*> attachments = {}) override;
      void HandleProgress(int64_t callId, int64_t percentage, std::vector<Attachment*> attachments = {}, json intermediateResult = {}) override;
      void HandleError(int64_t callId, std::string error) override;

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
      json client_inputs;
    };
  }
}

#endif
