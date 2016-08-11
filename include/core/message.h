#ifndef LC2PP_MESSAGE_H
#define LC2PP_MESSAGE_H

#include "lib/json/src/json.hpp"

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
      size_t length;
      char* data;
    } Attachment;

    /** The message class is primarily intendet for conveniently contain,
     * serialize and deserialize messages for LC2. A message consists of a
     * header and a number of attachments. The header must be a json container
     * while attachments can have an arbitrary binary format.
     *
     * Usage:
     * ```
     * json header = {{"name", "test"}, {"abc", "def"}};
     * Attachment attachment = {5, "abcde"};
     *
     * // compose message
     * Message* message = new Message(header);
     * message->add_attachment(attachment);
     *
     * ``` */
    class Message {
    public:
      /** Creates a new message object with the specified header. */
      Message(json header);

      /** Adds an attachment to the message and returns the index of the
      * newly added attachment.
      */
      unsigned int add_attachment(Attachment attachment);

      /**
      * Removes the attachment at position `index`.
      */
      void remove_attachment(unsigned int index);

      /**
      * Returns the attachment at position `index`.
      */
      Attachment get_attachment(unsigned int index);

    protected:
      /** Deserializes a message object from a string. */
      static Message* deserialize(std::string serialized);

      /** Serializes a message to a string. */
      static std::string serialize(Message* message);

    private:
      json body;
      std::vector<Attachment> attachments;
    };
  }
}
#endif
