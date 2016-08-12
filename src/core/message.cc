#include "lc2pp/core/message.h"

#include "json/src/json.hpp"

#include <vector>
#include <string>

using json = nlohmann::json;

namespace lc2pp {
  namespace core {
    Message::Message(json header) {
      this->header = header;
    }

    void Message::AddAttachment(Attachment attachment) {
      // TODO: Add fields in header for the attachment
      // TODO: Add type support for attachments
      this->attachments.push_back(attachment);
    }

    Attachment Message::GetAttachment(size_t index) {
      // TODO: Handle error when index is out of bounds
      return this->attachments.at(index);
    }

    size_t Message::GetNumAttachments() {
      return this->attachments.size();
    }

    json Message::GetHeader() {
      return this->header;
    }
  }
}
