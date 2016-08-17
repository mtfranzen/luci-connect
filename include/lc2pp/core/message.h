#ifndef LC2PP_MESSAGE_H
#define LC2PP_MESSAGE_H

#include "md5/md5.h"
#include "json/src/json.hpp"
#include "easylogging/src/easylogging++.h"

#include <vector>
#include <string>
#include <unordered_map>

using json = nlohmann::json;

namespace lc2pp {
  namespace core {
    /**
    * The attachment structure is a container for LC2 attachments that can
    * contain arbitrary binary data.
    */
    typedef struct Attachment {
      // TODO: Add attribute "keys" to attachments
      // TODO: Add extra enum for attachment formats

      Attachment(size_t s, char* d) : size(s), data(d) {}
      Attachment(size_t s, char* d, std::string f, std::string n) : size(s), data(d), format(f), name(n) {}
      Attachment(size_t s, const char* d) : size(s), data(d) {}
      Attachment(size_t s, const char* d, std::string f, std::string n) : size(s), data(d), format(f), name(n) {}

      /**
      * The attribute describes the attachment's size in bytes
      */
      size_t size = 0;

      /**
      * The attribute contains the attachment's data
      */
      const char* data = "";

      /**
      * The attribute describes the attachment's data format
      */
      std::string format;

      /**
      * The attribute describes an attachment's name.
      */
      std::string name;
    } Attachment;

    /**
    * The operator compares two attachments element-wise.
    */
    inline bool operator==(const Attachment& atc1, const Attachment& atc2) {
      return atc1.size == atc2.size && \
        atc1.data == atc2.data && \
        atc1.format == atc2.format && \
        atc1.name == atc2.name;
    }

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
      * newly added attachment. If a header attribute is specified that
      * describes the attachment at the newly created position, the
      * data within the attachment is overwritten by the header data.
      */
      size_t AddAttachment(Attachment attachment);

    private:
      json header_;
      std::vector<Attachment> attachments_;
    };

    /**
    * The operator compares two messages element-wise including attachments.
    */
    inline bool operator==(Message& msg1, Message& other) {
      bool eq = msg1.GetHeader() == other.GetHeader();
      eq &= msg1.GetNumAttachments() == other.GetNumAttachments();

      if (!eq) return false;
      for (size_t i = 0; i < msg1.GetNumAttachments(); i++)
        eq &= msg1.GetAttachment(i) == other.GetAttachment(i);

      return eq;
    }
  }
}
#endif
