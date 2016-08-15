#ifndef LC2PP_MESSAGE_H
#define LC2PP_MESSAGE_H

#include "json/src/json.hpp"

#include <vector>
#include <string>

using json = nlohmann::json;

namespace lc2pp {
  namespace core {
    /**
    * The attachment structure is a container for LC2 attachments that can
    * contain arbitrary binary data.
    */
    typedef struct Attachment {
      size_t size;
      const char* data;
    } Attachment;

    /** The message class is primarily intendet for conveniently contain,
     * serialize and deserialize messages for LC2. A message consists of a
     * header and a number of attachments. The header must be a json container
     * while attachments can have an arbitrary binary format.
     *
     * Composing a message:
     * ```
     * json header = {{"name", "test"}, {"abc", "def"}};
     * Attachment attachment = {5, "abcde"};
     *
     * // compose message
     * Message* message = new Message(header);
     * message->AddAttachment(attachment);
     * ```
     *
     * Reading a message:
     * ```
     * json header = message->GetHeader();
     * for(int i = 0; i < message->GetNumAttachments(); i++) {
     *   Attachment attachment = message->GetAttachment(i);
     * }
     * ``` */
    class Message {
    public:
      /** Creates a new message object with the specified header. */
      Message(json header);

      /** Returns the message header. */
      json GetHeader();

      /** Returns the number of attachments */
      size_t GetNumAttachments();

      /** Returns the attachment at position `index`. */
      Attachment GetAttachment(size_t index);

      /** Adds an attachment to the message and returns the index of the
      * newly added attachment.
      */
      void AddAttachment(Attachment attachment);

    private:
      json header;
      std::vector<Attachment> attachments;
    };
  }
}
#endif
