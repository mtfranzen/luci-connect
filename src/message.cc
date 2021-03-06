#include "luciconnect/message.h"

namespace luciconnect {
  Message::Message(json header) {
    this->header_ = header;

    if(!this->ValidateHeader()) {
      std::cout << "ERROR: Header validation failed" << std::endl;
      throw "Header validation failed";
    }
  }

  Message* Message::RunMessage(int64_t callId, std::string serviceName, json inputs, std::vector<Attachment*> attachments) {
    json header = {{"run", serviceName},{"callID", callId}};
    for (json::iterator it = inputs.begin(); it != inputs.end(); ++it)
      header[it.key()] = it.value();
    Message* message = new Message(header);
    for (Attachment* atc : attachments)
      message->AddAttachment(atc);
    return message;
  }

  Message* Message::CancelMessage(int64_t callId) {
    json header = {{"cancel", callId}, {"callID", callId}};
    return new Message(header);
  }

  Message* Message::ResultMessage(int64_t callId, json result, std::vector<Attachment*> attachments) {
    json header = {
      {"result", result},
      {"callID", callId}
    };
    Message* message = new Message(header);
    for (Attachment* atc : attachments)
      message->AddAttachment(atc);
    return message;
  }

  Message* Message::ProgressMessage(int64_t callId, int64_t percentage, std::vector<Attachment*> attachments, json intermediateResult) {
    json header = {
      {"progress", percentage},
      {"callID", callId},
      {"intermediateResult", intermediateResult}
    };
    Message* message = new Message(header);
    for (Attachment* atc : attachments)
      message->AddAttachment(atc);
    return message;
  }

  Message* Message::ErrorMessage(int64_t callId, std::string error) {
    json header = {{"error", error}, {"callID", callId}};
    return new Message(header);
  }

  size_t Message::AddAttachment(Attachment* attachment) {
    size_t index = this->attachments_.size();
    size_t position = index + 1; // LC2 position starts at 1

    MD5 md5;
    std::string md5sum = md5(attachment->data, attachment->size);

    // Attachments are only for result-messages in the "result" dicitionary,
    // other wise on root level.
    json base_element = this->header_;
    if (this->header_.count("result") > 0) base_element = this->header_["result"];

    bool attachment_header_missing = true;
    for (json::iterator it = base_element.begin(); it != base_element.end(); ++it) {
      json element = it.value();
      std::string element_name = it.key();
      if (element.count("attachment") > 0 && element["attachment"]["position"].get<std::size_t>() == position) {
          if (element.count("format") == 0) {
            std::cout << "ERROR: Attachment header is incomplete" << std::endl;
            throw "Attachment header is incomplete";
          }

          if (element["attachment"].count("length") == 0  || element["attachment"].count("checksum") == 0) {
            std::cout << "ERROR: Attachment subheader is incomplete" << std::endl;
            throw "Attachment subheader is incomplete";
          }

          // validate
          if (md5sum != element["attachment"]["checksum"]) {
            std::cout << "ERROR: Attachment checksums do not match up" << std::endl;
            throw "Attachment checksums do not match up";
          }

          if (attachment->size != element["attachment"]["length"].get<std::size_t>()) {
            std::cout << "ERROR: Attachment sizes do not match up." << std::endl;
            throw "Attachment sizes do not match up";
          }

          attachment_header_missing = false;
          attachment->format = element["format"].get<std::string>();
          attachment->name = element_name;
      }
    }

    if (attachment_header_missing) {
      // construct header data for attachment
      json attachmentjson;
      attachmentjson["format"] = attachment->format;
      attachmentjson["attachment"] = {
        {"length", attachment->size},
        {"position", position},
        {"checksum", md5sum}
      };
      attachmentjson["name"] = attachment->name;

      base_element[attachment->name] = attachmentjson;
    }

    if (this->header_.count("result") > 0) {
      this->header_["result"] = base_element;
    }
    else {
      this->header_ = base_element;
    }

    this->attachments_.push_back(attachment);
    return index;
  }

  Attachment* Message::GetAttachment(size_t index) {
    if (index >= this->attachments_.size()) {
      throw "Index out of bounds";
    }
    return this->attachments_[index];
  }

  std::vector<Attachment*> Message::GetAttachments() {
    return this->attachments_;
  }

  size_t Message::GetNumAttachments() {
    return this->attachments_.size();
  }

  json Message::GetHeader() {
    // copy header
    json header = json::parse(this->header_.dump());
    return header;
  }

  MessageType Message::GetType() {
    // copy header
    return this->type_;
  }

  bool Message::ValidateHeader() {
    // callID-check currently not enforced for error messages => handling also doesn't work! // TODO
    if (this->header_.count("error") == 0 && this->header_.count("callID") != 1) {
      std::cout << "WARNING: Message does not contain a callID." << std::endl;
      return false;
    }
    this->callId_ = this->header_["callID"];

    size_t runC, cancelC, resultC, progressC, errorC;
    runC = this->header_.count("run");
    cancelC = this->header_.count("cancel");
    resultC = this->header_.count("result");
    progressC = this->header_.count("progress");
    errorC = this->header_.count("error");

    size_t totalC = runC + cancelC + resultC + progressC + errorC;

    if (totalC < 1) {
      std::cout << "WARNING: Message contains no type keyword." << std::endl;
      return false;
    }

    if (totalC > 1) {
      std::cout << "WARNING: Message contains more than two type keywords." << std::endl;
      return false;
    }

    if (runC) {
      this->type_ = MessageType::run;
      return ValidateRunMessage();
    }

    if (cancelC) {
      this->type_ = MessageType::cancel;
      return ValidateCancelMessage();
    }

    if (resultC) {
      this->type_ = MessageType::result;
      return ValidateResultMessage();
    }

    if (progressC) {
      this->type_ = MessageType::progress;
      return ValidateProgressMessage();
    }

    if (errorC) {
      this->type_ = MessageType::error;
      return ValidateErrorMessage();
    }

    return false;
  }

  bool Message::ValidateRunMessage() {
    std::string serviceName;
    try {
      serviceName =this->header_["run"].get<std::string>();
      return !serviceName.empty();
    }
    catch (std::exception& err) {
      std::cout << "WARNING " << this->header_.dump() << std::endl;;
      return false;
    }
  }

  bool Message::ValidateCancelMessage() {
    try {
      return this->header_["cancel"] == nullptr\
		  || this->header_["cancel"] == new json{}\
       || this->header_["cancel"] == this->header_["callID"];
    }
    catch (std::exception& err) {
      std::cout << "WARNING " << this->header_.dump() << std::endl;;
      return false;
    }
  }

  bool Message::ValidateResultMessage() {
    json result;
    std::string serviceName;

    try {
      result = this->header_["result"];
      (int64_t)this->header_["callID"];
      return true;
    }
    catch (std::exception& err) {
      std::cout << "WARNING " << this->header_.dump() << std::endl;
      return false;
    }
  }

  bool Message::ValidateProgressMessage() {
    json intermediateResult;
    std::string serviceName;

    try {
      (int64_t)this->header_["progress"];
      (int64_t)this->header_["callID"];
      if (this->header_.count("intermediateResult") > 0)
        intermediateResult = this->header_["intermediateResult"];
      return true;
    }
    catch (std::exception& err) {
      std::cout << "WARNING " << this->header_.dump() << std::endl;;
      return false;
    }
  }

  bool Message::ValidateErrorMessage() {
    std::string error;

    try {
      error = this->header_["error"].get<std::string>();
      (int64_t)this->header_["callID"];
      return true;
    }
    catch (std::exception& err) {
      std::cout << "WARNING " << this->header_.dump() << std::endl;;
      return false;
    }
  }
}
