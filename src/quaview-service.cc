#include "luciconnect/quaview-service.h"

namespace luciconnect {
  namespace quaview {
    Service::Service(std::shared_ptr<Connection> connection) : luciconnect::Node(connection) {
      this->terminated = false;
    }

    void Service::Run() {
      this->Connect();
      this->Register();
      while (!this->terminated) std::this_thread::sleep_for(std::chrono::microseconds(50));;
      this->Disconnect();
    }

    void Service::Register() {
      json register_message = {};
      register_message["serviceName"] = this->GetName();
      register_message["description"] = this->GetDescription();
      register_message["qua-view-compliant"] = true;

      register_message["inputs"] = this->GetInputs();
      register_message["inputs"]["ScID"] = "number";
      register_message["inputs"]["mode"] = "string";
      register_message["inputs"]["points"] = "attachment";

      register_message["outputs"] = {{"unit", "string"}, {"values", "number"}};

      register_message["constraints"] = this->GetConstraints();
      std::vector<std::string> modes;
      if (this->SupportsPointMode()) modes.push_back("points"); // TODO: Other modes
      register_message["constraints"]["mode"] = modes;

      register_message["exampleCall"] = {
        {"run", this->GetName()},
        {"callID", 42},
        {"mode", modes[0]},
        {"attachment", {
          {"length", 512},
          {"position", 1},
          {"checksum", "abc"}
        }} // TODO
      };

      srand (time(NULL));
      this->register_callid = rand() % 10000;
      this->SendRun(this->register_callid, "RemoteRegister", register_message);
    }

    void Service::HandleRun(int64_t callId, std::string serviceName, json inputs, std::vector<Attachment*> attachments) {
      this->is_busy.lock();
      this->client_callid = callId;

      if (inputs["mode"] == "points") { // TODO: Other modes
        this->client_mode = "points";
        if (!this->SupportsPointMode()) {
          this->SendError(callId, "Mode not Supported.");
        }

        luciconnect::Attachment atc = *attachments[0];
        vec3* raw = (vec3*)atc.data;
        this->client_points = std::vector<vec3>(raw, raw + attachments[0]->size / sizeof(vec3));
        this->client_scenario_id = inputs["ScID"];
      }
      std::cout << inputs << std::endl;
      this->client_inputs = inputs;
	  srand (time(NULL)+1);
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

          std::vector<float> results = this->ComputeOnPoints(scenario_triangles, points, this->client_inputs);
          float* raw = results.data();
          luciconnect::Attachment atc {results.size()*sizeof(float), (const char*)raw, "Float32Array", "values"};
          std::vector<luciconnect::Attachment*> atcs = {&atc};
          json result_header = {{"units", this->GetUnit()},{"mode", "points"}};
          this->SendResult(this->client_callid, result_header, atcs);
        }

        this->is_busy.unlock();
      }
    }

    void Service::HandleCancel(int64_t callId) {}
    void Service::HandleProgress(int64_t callId, int64_t percentage, std::vector<Attachment*> attachments, json intermediateResult) {}

    void Service::HandleError(int64_t callId, std::string error) {
      std::cout << "ERROR " << error << std::endl;
    }
  }
}
