#include "lc2pp/core/message.h"

namespace lc2pp {
  namespace core {
    Message::Message(json header) {
      // TODO: Parse header for different message types
      this->header_ = header;
    }

    size_t Message::AddAttachment(Attachment attachment) {
      // TODO: Accept pointer instead of object when adding an attachment to a message

      // position + 1 because the position argument starts at 1 in Luci
      size_t position = this->attachments_.size() + 1;

      // TODO: Validate checksum when an attachment is added to a message
      // TODO: Add procedure to allow arbitrary names for attachments in the header
      bool attachment_header_missing = true;
      for (json element : this->header_) {
        if (element.count("attachment") > 0) {
          if (element["attachment"]["position"] == position) {
              attachment_header_missing = false;
              attachment.format = element["format"];
              attachment.name = element["attachment"]["name"];
          }
        }
      }

      if (attachment_header_missing) {
        // TODO: Add handling of incomplete attachments when adding an attachment
        MD5 md5;

        // construct header data for attachment
        json attachmentjson;
        attachmentjson["format"] = attachment.format;
        attachmentjson["attachment"] = {
          {"length", attachment.size},
          {"position", position},
          {"checksum", md5(attachment.data, attachment.size)}
        };
        attachmentjson["name"] = attachment.name;

        this->header_[attachment.name] = attachmentjson;
      }

      this->attachments_.push_back(attachment);
      return position;
    }

    Attachment Message::GetAttachment(size_t index) {
      if (index >= this->attachments_.size()) {
        throw "Index out of bounds.";
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
