#include "core/message.h"

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
      this->attachments.push_back(attachment);
    }

    Attachment Message::GetAttachment(size_t index) {
      return this->attachments.at(index);
    }

    size_t Message::GetNumAttachments() {
      return this->attachments.size();
    }
  }
}
