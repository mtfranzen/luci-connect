#include "lc2pp/core/message.h"

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
      if (index >= this->attachments.size()) {
        throw "Index out of bounds.";
      }
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
