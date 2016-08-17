#include "connectiontest.h"

/**
* Contains tests for constructing messages.
*/
namespace {
  TEST(MessageTest, CreateEmptyMessage) {
    json header = {};
    new lc2pp::core::Message(header);
  }

  TEST(MessageTest, CreateMessageAndGetHeader) {
    json header = {
      {"run", "test.Randomly"},
      {"test", {0,1,2}}
    };
    lc2pp::core::Message* message = new lc2pp::core::Message(header);

    json header2 = message->GetHeader();
    ASSERT_EQ(header, header2);
  }

  TEST(MessageTest, ElementwiseCopyMessage) {
    // TODO: Test to copy a message from its elements
  }

  TEST(MessageTest, CreateMessageUTF8) {
    // TODO: Testcreating a message that contains UTF-8
  }

  TEST(MessageTest, CreateCompleteAttachments) {
    lc2pp::core::Attachment attachment;
    attachment.size = 5;
    attachment.data = "abcde";
    attachment.format = "float32 array";
    attachment.name = "testname";

    lc2pp::core::Attachment attachment2 = {
      5,
      "abcde",
      "float32 array",
      "testname"
    };
  }

  TEST(MessageTest, CreateIncompleteAttachment) {
    // TODO: Test creating an incomplete attachment
  }

  TEST(MessageTest, CreateCorruptAttachment) {
    // TODO: Test creating a corrupt attachment (checksum, size)
  }

  TEST(MessageTest, ChangeAttachment) {
    lc2pp::core::Attachment attachment = {5};
    attachment.data = "Test";
  }

  TEST(MessageTest, AddCompleteAttachmentWithoutHeader) {
    // TODO: Test adding attachments without a header and with complete data
  }

  TEST(MessageTest, AddIncompleteAttachmentWithoutHeader) {
    // TODO: Test adding attachments without a header and with incomplete data
    // use different levels of completness! Check Header!
  }

  TEST(MessageTest, AddCompleteAttachmentWithHeader) {
    // TODO: Test adding attachments without a header and with complete data
  }

  TEST(MessageTest, AddIncompleteAttachmentWithHeader) {
    // TODO: Test adding attachments without a header and with incomplete data
    // use different levels of completness! Check Header!
  }

  TEST(MessageTest, AddMultipleAttachments) {
    // TODO: Test support for multiple attachments

    /*
    lc2pp::core::Message* message = new lc2pp::core::Message({});
    std::vector<lc2pp::core::Attachment> attachments;

    size_t N = 64;
    for (size_t i = 0; i < N; i++) {
      lc2pp::core::Attachment attachment = {5, "abcde", "float32 array", std::to_string(i)};
      attachments.push_back(attachment);
      message->AddAttachment(attachment);
    }

    for(size_t i = 0; i < N; i++)
      ASSERT_EQ(attachments[i], message->GetAttachment(i));
      */
  }

  TEST(MessageTest, CheckAttachmentHeaderConsistency) {
    // TODO: Check whether header data always corresponds to attachment data
    // (position, format, etc.)
  }
}
