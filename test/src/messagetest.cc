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

  TEST(MessageTest, CreateCompleteAttachments) {
    lc2pp::core::Attachment attachment(5, "abcde");
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

  TEST(MessageTest, CompareCompleteAttachments) {
    // TODO: Test equality operator for complete attachments
  }

  TEST(MessageTest, CompareIncompleteAttachments) {
    // TODO: Test equality operator for incomplete attachments
  }

  TEST(MessageTest, CreateCorruptAttachment) {
    // TODO: Test creating a corrupt attachment (checksum, size)
  }

  TEST(MessageTest, ChangeAttachment) {
    lc2pp::core::Attachment attachment = {5, "Test1"};
    attachment.data = "Test2";

    ASSERT_EQ(attachment.data, "Test2");
  }

  TEST(MessageTest, CompareAttachments) {
    // TODO: Compare Attachments
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
    // TEST: Test support for multiple attachments

    lc2pp::core::Message* message = new lc2pp::core::Message({});
    std::vector<lc2pp::core::Attachment> attachments;

    size_t N = 64;
    for (size_t i = 0; i < N; i++) {
      lc2pp::core::Attachment attachment(5, "abcde");
      attachments.push_back(attachment);
      message->AddAttachment(attachment);
    }

    for(size_t i = 0; i < N; i++)
      ASSERT_EQ(attachments[i], message->GetAttachment(i));
  }

  TEST(MessageTest, CompareMessage) {
    // TODO: Test message comparison

    /*
    lc2pp::core::Attachment atc1(1, "a", "float32 array", "atc1");
    lc2pp::core::Attachment atc2(1, "a", "float32 array", "atc2");;

    json hd1 = {{"a", "b"}};
    json hd2 = {{"b", "b"}};

    lc2pp::core::Message* msg1 = new lc2pp::core::Message(hd1);
    lc2pp::core::Message* msg2 = new lc2pp::core::Message(hd1);
    msg1->AddAttachment(atc1);
    ASSERT_NE(*msg1, *msg2);
    msg2->AddAttachment(atc1);
    ASSERT_EQ(*msg1, *msg2);
    msg1->AddAttachment(atc1);
    ASSERT_NE(*msg1, *msg2);
    msg2->AddAttachment(atc1);
    ASSERT_EQ(*msg1, *msg2);
    msg1->AddAttachment(atc2);
    ASSERT_NE(*msg1, *msg2);
    msg2->AddAttachment(atc2);
    ASSERT_NE(*msg1, *msg2);

    // different header
    lc2pp::core::Message* msg3 = new lc2pp::core::Message(hd2);
    ASSERT_NE(msg1, msg3);
    */
  }

  TEST(MessageTest, CreateMessageUTF8) {
    // TODO: Test creating a message that contains UTF-8
  }

  TEST(MessageTest, CheckAttachmentHeaderConsistency) {
    // TODO: Check whether header data always corresponds to attachment data
    // (position, format, etc.)
  }

}
