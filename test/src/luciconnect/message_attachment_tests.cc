#include "luciconnect/message_tests.h"

/**
* Contains tests for constructing messages.
*/
namespace {
  TEST_F(MessageTest, CreateCompleteAttachments) {
    luciconnect::Attachment attachment = {
      5,
      "abcde",
      "float32 array",
      "testname"
    };
  }

  TEST_F(MessageTest, CreateIncompleteAttachment) {
      luciconnect::Attachment atc1(5, "abcde");
  }

  TEST_F(MessageTest, CompareCompleteAttachments) {
    luciconnect::Attachment atc1(5, "abcde", "format", "name");
    luciconnect::Attachment atc2(5, "abcde", "format", "name");
    luciconnect::Attachment atc3(5, "edcba", "format", "name");
    luciconnect::Attachment atc4(5, "edcba", "format", "name");
    luciconnect::Attachment atc5(5, "edcba", "tamrof", "name");
    luciconnect::Attachment atc6(5, "edcba", "tamrof", "name");
    luciconnect::Attachment atc7(5, "edcba", "tamrof", "eman");
    luciconnect::Attachment atc8(5, "edcba", "tamrof", "eman");
    luciconnect::Attachment atc9(6, "edcba1", "tamrof", "eman");
    luciconnect::Attachment atc10(6, "edcba1", "tamrof", "eman");

    ASSERT_EQ(atc1, atc2);
    ASSERT_NE(atc2, atc3);
    ASSERT_EQ(atc3, atc4);
    ASSERT_NE(atc4, atc5);
    ASSERT_EQ(atc5, atc6);
    ASSERT_NE(atc6, atc7);
    ASSERT_EQ(atc7, atc8);
    ASSERT_NE(atc8, atc9);
    ASSERT_EQ(atc9, atc10);
  }

  TEST_F(MessageTest, CompareIncompleteAttachments) {
    luciconnect::Attachment atc1(5, "abcde");
    luciconnect::Attachment atc2(5, "abcde");
    luciconnect::Attachment atc3(5, "edfba");
    luciconnect::Attachment atc4(6, "abcdef");

    ASSERT_EQ(atc1, atc2);
    ASSERT_NE(atc2, atc3);
    ASSERT_NE(atc3, atc4);
  }

  TEST_F(MessageTest, ChangeAttachment) {
    luciconnect::Attachment attachment = {5, "Test1"};
    attachment.data = "Test2";

    ASSERT_EQ(attachment.data, "Test2");
  }

  TEST_F(MessageTest, AddCompleteAttachmentWithoutHeader) {
    luciconnect::Attachment atc1(5, "abcde", "format", "name");
    luciconnect::Message* message = new luciconnect::Message(simple_header_);
    message->AddAttachment(&atc1);
    ASSERT_EQ(atc1, *message->GetAttachment(0));
  }

  TEST_F(MessageTest, AddIncompleteAttachmentWithoutHeader) {
    luciconnect::Attachment atc1(5, "abcde");
    luciconnect::Message* message = new luciconnect::Message(simple_header_);
    message->AddAttachment(&atc1);
    ASSERT_EQ(atc1, *message->GetAttachment(0));
  }

  TEST_F(MessageTest, AddCompleteAttachmentWithHeader) {
    luciconnect::Attachment atc1(5, "abcde", "format", "atc1");
    json header = {
      {"run", "test.Randomly"},
      {"test", {0,1,2}},
      {"callID", 0},
      {"atc1", {
        {"attachment", {
              {"length", 5},
              {"checksum", "ab56b4d92b40713acc5af89985d4b786"},
              {"position", 1}
        }},
        {"name", "atc1"},
        {"format", "format"}
      }}
    };
    luciconnect::Message* message = new luciconnect::Message(header);
    message->AddAttachment(&atc1);
    ASSERT_EQ(atc1, *message->GetAttachment(0));
  }

  TEST_F(MessageTest, AddIncompleteAttachmentWithHeader) {
    luciconnect::Attachment atc1(5, "abcde");
    json header = {
      {"run", "test.Randomly"},
      {"test", {0,1,2}},
      {"callID", 0},
      {"atc1", {
          {"attachment", {
              {"length", 5},
              {"checksum", "ab56b4d92b40713acc5af89985d4b786"},
              {"position", 1}
          }},
          {"name", "atc1"},
          {"format", "format"}
      }}
    };
    luciconnect::Message* message = new luciconnect::Message(header);
    message->AddAttachment(&atc1);

    luciconnect::Attachment atc2(5, "abcde", "format", "atc1");
    luciconnect::Attachment atc1p = *message->GetAttachment(0);
    ASSERT_EQ(atc2, *message->GetAttachment(0));
  }

  TEST_F(MessageTest, AddAttachmentWrongChecksumInHeader) {
    luciconnect::Attachment atc1(5, "abcde");
    json header = {
      {"run", "test.Randomly"},
      {"test", {0,1,2}},
      {"callID", 0},
      {"atc1", {
          {"attachment", {
              {"length", 5},
              {"checksum", "ab56b4d92b40713acc5af89985d4b787"},
              {"position", 1}
          }},
          {"name", "atc1"},
          {"format", "format"}
      }}
    };
    luciconnect::Message* message = new luciconnect::Message(header);
    ASSERT_ANY_THROW(message->AddAttachment(&atc1));
  }

  TEST_F(MessageTest, AddAttachmentWrongLengthInHeader) {
    luciconnect::Attachment atc1(5, "abcde");
    json header = {
      {"run", "test.Randomly"},
      {"test", {0,1,2}},
      {"callID", 0},
      {"atc1", {
          {"attachment", {
              {"length", 6},
              {"checksum", "ab56b4d92b40713acc5af89985d4b786"},
              {"position", 1}
          }},
          {"name", "atc1"},
          {"format", "format"}
      }}
    };
    luciconnect::Message* message = new luciconnect::Message(header);
    ASSERT_ANY_THROW(message->AddAttachment(&atc1));
  }

  TEST_F(MessageTest, AddAttachmentIncompleteHeader) {
    luciconnect::Attachment atc1(5, "abcde");
    json header = {
      {"run", "test.Randomly"},
      {"test", {0,1,2}},
      {"callID", 0},
      {"atc1", {
          {"attachment", {
              {"position", 1}
          }}
      }}
    };

    luciconnect::Message* message = new luciconnect::Message(header);
    ASSERT_ANY_THROW(message->AddAttachment(&atc1));

    header["atc1"]["name"] = "atc1";
    message = new luciconnect::Message(header);
    ASSERT_ANY_THROW(message->AddAttachment(&atc1));

    header["atc1"]["format"] = "format";
    message = new luciconnect::Message(header);
    ASSERT_ANY_THROW(message->AddAttachment(&atc1));

    header["atc1"]["attachment"]["length"] = 5;
    message = new luciconnect::Message(header);
    ASSERT_ANY_THROW(message->AddAttachment(&atc1));

    header["atc1"]["attachment"]["checksum"] = "ab56b4d92b40713acc5af89985d4b786";
    message = new luciconnect::Message(header);
    message->AddAttachment(&atc1);
  }

  TEST_F(MessageTest, AddMultipleAttachments) {
    luciconnect::Message* message = new luciconnect::Message(simple_header_);
    std::vector<luciconnect::Attachment*> attachments;

    size_t N = 64;
    for (size_t i = 0; i < N; i++) {
      luciconnect::Attachment attachment(5, "abcde");
      attachments.push_back(&attachment);
      message->AddAttachment(&attachment);
    }

    for(size_t i = 0; i < N; i++)
      ASSERT_EQ(attachments[i], message->GetAttachment(i));
  }
}
