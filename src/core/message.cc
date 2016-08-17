#include "lc2pp/core/message.h"

namespace lc2pp {
  namespace core {
    Message::Message(json header) {
      // TODO: Parse header for different message types
      this->header_ = header;
    }

    size_t Message::AddAttachment(Attachment attachment) {
      // TODO: Accept pointer instead of object.

      size_t position = this->attachments_.size();

      // TODO: Overwrite checksum when attachment is added.
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
        // TODO: Add handling of incomplete attachments in AddAttachment
        // if it has not been found, create header field for it
        MD5 md5;

        // compute checksum
        std::string checksum = md5(attachment.data, attachment.size);

        // construct header data for attachment
        json attachmentjson;
        attachmentjson["format"] = attachment.format;
        attachmentjson["attachment"] = { {"length", attachment.size}, {"position", position+1}, {"checksum", checksum}};
        attachmentjson["name"] = attachment.name;
        //for (std::pair<std::string, std::string> key_value_pair : attachment.keys)
        //  attachmentjson[key_value_pair.first] = key_value_pair.second;

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
