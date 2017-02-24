#include "luciconnect/quaview-service.h"

namespace luciconnect {
  namespace quaview {
    Service::Service(std::shared_ptr<Connection> connection) : luciconnect::Node(connection) {
      this->terminated = false;
    }

    void Service::Run() {
      this->Connect();
      this->Register();
      while (!this->terminated) usleep(50);
      this->Disconnect();
    }

    void Service::Register() {
      this->register_callid = rand() % 10000;
    }

    void Service::HandleRun(int64_t callId, std::string serviceName, json inputs, std::vector<Attachment*> attachments) {
      this->is_busy.lock();
      this->client_callid = callId;

      if (inputs["mode"] == "points") { // TODO: Other modes
        this->client_mode = "points";
        if (!this->supports_point_mode) {
          this->SendError(callId, "Mode not Supported.");
        }

        luciconnect::Attachment atc = *attachments[0];
        vec3* raw = (vec3*)atc.data;
        this->client_points = std::vector<vec3>(raw, raw + attachments[0]->size / sizeof(vec3));
        this->client_scenario_id = inputs["ScID"];
      }

      this->scenario_callid = rand() % 10000;
      this->SendRun(this->scenario_callid, "scenario.geojson.Get", {{"ScID", this->client_scenario_id}});
    }

    void Service::HandleResult(int64_t callId, json result, std::vector<Attachment*> attachments) {
      if (callId == this->register_callid) {
        // Registration Successful
      }
      else if (callId == this->scenario_callid) {
        // Geojson Received
        std::string geojson = result["geometry_output"]["geometry"].dump();

        // Delegate geojson to computation method
        if (this->client_mode == "points") { // TODO: Other modes
          std::vector<vec3> scenario_triangles = geojson::parse(result["geometry_output"]["geometry"].dump());
          std::vector<vec3> points = this->client_points;

          std::vector<float> results = this->ComputeOnPoints(scenario_triangles, points);
          luciconnect::Attachment atc {results.size()*sizeof(float), (const char*)atc.data, "Float32Array", "values"};
          std::vector<luciconnect::Attachment*> atcs = {&atc};
          this->SendResult(this->client_callid, result, atcs);
        }

        this->is_busy.unlock();
      }
    }

    void Service::HandleCancel(int64_t callId) {}
    void Service::HandleProgress(int64_t callId, int64_t percentage, std::vector<Attachment*> attachments, json intermediateResult) {}
    void Service::HandleError(int64_t callId, std::string error) {}
  }
}
