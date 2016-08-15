#include "lc2pp/core/message.h"
#include "md5/md5.h"

namespace lc2pp {
  namespace core {
    Message::Message(json header) {
      // TODO: Parse header for different message types
      this->header_ = header;
    }

    size_t Message::AddAttachment(Attachment attachment) {
      this->attachments_.push_back(attachment);
      return this->GetNumAttachments() - 1;
    }

    Attachment Message::GetAttachment(size_t index) {
      if (index >= this->attachments_.size()) {
        throw "Index out of bounds.";
      }
      return this->attachments_.at(index);
    }

    size_t Message::GetNumAttachments() {
      return this->attachments_.size();
    }

    json Message::GetHeader() {
      // copy header
      json header = json::parse(this->header_.dump());

      MD5::MD5 md5;
      // add attachments
      for (size_t i = 0; i < this->GetNumAttachments(); i++) {
        Attachment attachment = this->GetAttachment(i);

        // compute checksum
        std::string checksum = md5(attachment.data, attachment.size);

        // construct header data for attachment
        json attachmentjson;
        attachmentjson["format"] = attachment.format;
        attachmentjson["attachment"] = { {"length", attachment.size}, {"position", i+1}, {"checksum", checksum}};
        attachmentjson["name"] = attachment.name;
        for (std::pair<std::string, std::string> key_value_pair : attachment.keys)
          attachmentjson[key_value_pair.first] = key_value_pair.second;

        header[attachment.name] = attachmentjson;
      }

      return header;
    }
  }
}
