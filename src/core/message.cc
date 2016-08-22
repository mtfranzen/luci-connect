#include "lc2pp/core/message.h"

namespace lc2pp {
  namespace core {
    Message::Message(json header) {
      this->header_ = header;

      if(!this->ValidateHeader()) {
        LOG(ERROR) << "Header validation failed";
        throw "Header validation failed";
      }
    }

    size_t Message::AddAttachment(Attachment* attachment) {
      size_t index = this->attachments_.size();
      size_t position = index + 1; // LC2 position starts at 1

      MD5 md5;
      std::string md5sum = md5(attachment->data, attachment->size);

      // TODO: Add procedure to allow arbitrary names for attachments in the header
      bool attachment_header_missing = true;
      for (json element : this->header_) {
        if (element.count("attachment") > 0 && element["attachment"]["position"] == position) {
            if (element.count("format") == 0  || element.count("name") == 0) {
              LOG(ERROR) << "Attachment header is incomplete";
              throw "Attachment header is incomplete";
            }

            if (element["attachment"].count("length") == 0  || element["attachment"].count("checksum") == 0) {
              LOG(ERROR) << "Attachment subheader is incomplete";
              throw "Attachment subheader is incomplete";
            }

            // validate
            if (md5sum != element["attachment"]["checksum"]) {
              LOG(ERROR) << "Attachment checksums do not match up";
              throw "Attachment checksums do not match up";
            }

            if (attachment->size != element["attachment"]["length"]) {
              LOG(ERROR) << "Attachment sizes do not match up.";
              throw "Attachment sizes do not match up";
            }

            attachment_header_missing = false;
            attachment->format = element["format"];
            attachment->name = element["name"];
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

        this->header_[attachment->name] = attachmentjson;
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

    size_t Message::GetNumAttachments() {
      return this->attachments_.size();
    }

    json Message::GetHeader() {
      // copy header
      json header = json::parse(this->header_.dump());
      return header;
    }

    bool Message::ValidateHeader() {
      if (this->header_.count("callID") != 1) {
        LOG(WARNING) << "Message does not contain a callID.";
        return false;
      }
      this->callId_ = this->header_["callID"];

      size_t runC, cancelC, resultC, progressC, errorC;
      runC = this->header_.count("run");
      cancelC = this->header_.count("cancel");
      resultC = this->header_.count("result");
      progressC = this->header_.count("progress");
      errorC = this->header_.count("error");

      if (runC + cancelC + resultC + progressC + errorC != 1) {
        LOG(WARNING) << "Message contains more than two type keywords.";
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
        serviceName = this->header_["run"];
        return !serviceName.empty();
      }
      catch (std::exception& err) {
        LOG(WARNING) << "Message of type `run` - validation failed";
        return false;
      }
    }

    bool Message::ValidateCancelMessage() {
      try {
        return this->header_["cancel"] == nullptr\
         || this->header_["cancel"] == (json){}\
         || this->header_["cancel"] == this->header_["callID"];
      }
      catch (std::exception& err) {
        LOG(WARNING) << "Message of type `cancel` - validation failed";
        return false;
      }
    }

    bool Message::ValidateResultMessage() {
      json result;
      std::string serviceName;

      try {
        result = this->header_["result"];
        serviceName = this->header_["serviceName"];
        (int64_t)this->header_["callID"];
        (int64_t)this->header_["taskID"];
        return true;
      }
      catch (std::exception& err) {
        LOG(WARNING) << "Message of type `result` - validation failed";
        return false;
      }
    }

    bool Message::ValidateProgressMessage() {
      json progress;
      std::string serviceName;

      try {
        progress = this->header_["progress"];
        serviceName = this->header_["serviceName"];
        (int64_t)this->header_["callID"];
        (int64_t)this->header_["taskID"];
        (int64_t)this->header_["percentage"];
        return true;
      }
      catch (std::exception& err) {
        LOG(WARNING) << "Message of type `progress` - validation failed";
        return false;
      }
    }

    bool Message::ValidateErrorMessage() {
      std::string error;

      try {
        error = this->header_["error"];
        return true;
      }
      catch (std::exception& err) {
        LOG(WARNING) << "Message of type `error` - validation failed";
        return false;
      }
    }
  }
}
