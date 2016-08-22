#include "messagetest.h"

// TODO: Restructure the tests to find bottle necks.

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
    lc2pp::core::Attachment attachment = {
      5,
      "abcde",
      "float32 array",
      "testname"
    };
  }

  TEST(MessageTest, CreateIncompleteAttachment) {
      lc2pp::core::Attachment atc1(5, "abcde");
  }

  TEST(MessageTest, CompareCompleteAttachments) {
    lc2pp::core::Attachment atc1(5, "abcde", "format", "name");
    lc2pp::core::Attachment atc2(5, "abcde", "format", "name");
    lc2pp::core::Attachment atc3(5, "edcba", "format", "name");
    lc2pp::core::Attachment atc4(5, "edcba", "format", "name");
    lc2pp::core::Attachment atc5(5, "edcba", "tamrof", "name");
    lc2pp::core::Attachment atc6(5, "edcba", "tamrof", "name");
    lc2pp::core::Attachment atc7(5, "edcba", "tamrof", "eman");
    lc2pp::core::Attachment atc8(5, "edcba", "tamrof", "eman");
    lc2pp::core::Attachment atc9(6, "edcba1", "tamrof", "eman");
    lc2pp::core::Attachment atc10(6, "edcba1", "tamrof", "eman");

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

  TEST(MessageTest, CompareIncompleteAttachments) {
    lc2pp::core::Attachment atc1(5, "abcde");
    lc2pp::core::Attachment atc2(5, "abcde");
    lc2pp::core::Attachment atc3(5, "edfba");
    lc2pp::core::Attachment atc4(6, "abcdef");

    ASSERT_EQ(atc1, atc2);
    ASSERT_NE(atc2, atc3);
    ASSERT_NE(atc3, atc4);
  }

  TEST(MessageTest, ChangeAttachment) {
    lc2pp::core::Attachment attachment = {5, "Test1"};
    attachment.data = "Test2";

    ASSERT_EQ(attachment.data, "Test2");
  }

  TEST(MessageTest, AddCompleteAttachmentWithoutHeader) {
    lc2pp::core::Attachment atc1(5, "abcde", "format", "name");
    json header = {
      {"run", "test.Randomly"},
      {"test", {0,1,2}}
    };
    lc2pp::core::Message* message = new lc2pp::core::Message(header);
    message->AddAttachment(&atc1);
    ASSERT_EQ(atc1, *message->GetAttachment(0));
  }

  TEST(MessageTest, AddIncompleteAttachmentWithoutHeader) {
    lc2pp::core::Attachment atc1(5, "abcde");
    json header = {
      {"run", "test.Randomly"},
      {"test", {0,1,2}}
    };
    lc2pp::core::Message* message = new lc2pp::core::Message(header);
    message->AddAttachment(&atc1);
    ASSERT_EQ(atc1, *message->GetAttachment(0));
  }

  TEST(MessageTest, AddCompleteAttachmentWithHeader) {
    lc2pp::core::Attachment atc1(5, "abcde", "format", "atc1");
    json header = {
      {"run", "test.Randomly"},
      {"test", {0,1,2}},
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
    lc2pp::core::Message* message = new lc2pp::core::Message(header);
    message->AddAttachment(&atc1);
    ASSERT_EQ(atc1, *message->GetAttachment(0));
  }

  TEST(MessageTest, AddIncompleteAttachmentWithHeader) {
    lc2pp::core::Attachment atc1(5, "abcde");
    json header = {
      {"run", "test.Randomly"},
      {"test", {0,1,2}},
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
    lc2pp::core::Message* message = new lc2pp::core::Message(header);
    message->AddAttachment(&atc1);

    lc2pp::core::Attachment atc2(5, "abcde", "format", "atc1");
    lc2pp::core::Attachment atc1p = *message->GetAttachment(0);
    ASSERT_EQ(atc2, *message->GetAttachment(0));
  }

  TEST(MessageTest, AddAttachmentWrongChecksumInHeader) {
    lc2pp::core::Attachment atc1(5, "abcde");
    json header = {
      {"run", "test.Randomly"},
      {"test", {0,1,2}},
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
    lc2pp::core::Message* message = new lc2pp::core::Message(header);
    ASSERT_ANY_THROW(message->AddAttachment(&atc1));
  }

  TEST(MessageTest, AddAttachmentWrongLengthInHeader) {
    lc2pp::core::Attachment atc1(5, "abcde");
    json header = {
      {"run", "test.Randomly"},
      {"test", {0,1,2}},
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
    lc2pp::core::Message* message = new lc2pp::core::Message(header);
    ASSERT_ANY_THROW(message->AddAttachment(&atc1));
  }

  TEST(MessageTest, AddAttachmentIncompleteHeader) {
    lc2pp::core::Attachment atc1(5, "abcde");
    json header = {
      {"run", "test.Randomly"},
      {"test", {0,1,2}},
      {"atc1", {
          {"attachment", {
              {"position", 1}
          }}
      }}
    };

    lc2pp::core::Message* message = new lc2pp::core::Message(header);
    ASSERT_ANY_THROW(message->AddAttachment(&atc1));

    header["atc1"]["name"] = "atc1";
    message = new lc2pp::core::Message(header);
    ASSERT_ANY_THROW(message->AddAttachment(&atc1));

    header["atc1"]["format"] = "format";
    message = new lc2pp::core::Message(header);
    ASSERT_ANY_THROW(message->AddAttachment(&atc1));

    header["atc1"]["attachment"]["length"] = 5;
    message = new lc2pp::core::Message(header);
    ASSERT_ANY_THROW(message->AddAttachment(&atc1));

    header["atc1"]["attachment"]["checksum"] = "ab56b4d92b40713acc5af89985d4b786";
    message = new lc2pp::core::Message(header);
    message->AddAttachment(&atc1);
  }

  TEST(MessageTest, AddMultipleAttachments) {
    lc2pp::core::Message* message = new lc2pp::core::Message({});
    std::vector<lc2pp::core::Attachment*> attachments;

    size_t N = 64;
    for (size_t i = 0; i < N; i++) {
      lc2pp::core::Attachment attachment(5, "abcde");
      attachments.push_back(&attachment);
      message->AddAttachment(&attachment);
    }

    for(size_t i = 0; i < N; i++)
      ASSERT_EQ(attachments[i], message->GetAttachment(i));
  }

  TEST(MessageTest, CompareMessage) {
    lc2pp::core::Attachment atc1(1, "a", "float32 array", "atc1");
    lc2pp::core::Attachment atc2(1, "a", "float32 array", "atc2");;

    json hd1 = {{"a", "b"}};
    json hd2 = {{"b", "b"}};

    lc2pp::core::Message* msg1 = new lc2pp::core::Message(hd1);
    lc2pp::core::Message* msg2 = new lc2pp::core::Message(hd1);
    msg1->AddAttachment(&atc1);
    ASSERT_NE(*msg1, *msg2);
    msg2->AddAttachment(&atc1);
    ASSERT_EQ(*msg1, *msg2);
    msg1->AddAttachment(&atc1);
    ASSERT_NE(*msg1, *msg2);
    msg2->AddAttachment(&atc1);
    ASSERT_EQ(*msg1, *msg2);
    msg1->AddAttachment(&atc2);
    ASSERT_NE(*msg1, *msg2);
    msg2->AddAttachment(&atc1);
    ASSERT_NE(*msg1, *msg2);

    // different header
    lc2pp::core::Message* msg3 = new lc2pp::core::Message(hd2);
    ASSERT_NE(*msg1, *msg3);
  }
}
