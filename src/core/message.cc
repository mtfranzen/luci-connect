#include "lc2pp/core/message.h"

namespace lc2pp {
  namespace core {
    Message::Message(json header) {
      // TODO: Parse header for different message types
      this->header_ = header;
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
  }
}
