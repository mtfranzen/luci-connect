#include "lucipp/core/message_tests.h"


/**
* Contains tests for constructing messages.
*/
namespace {
  using namespace lucipp::core;

  TEST_F(MessageTest, CreateMessageAndGetHeader) {
    Message* message = new Message(simple_header_);

    json header2 = message->GetHeader();
    ASSERT_EQ(simple_header_, header2);
  }

  TEST_F(MessageTest, CompareMessage) {
    Attachment atc1(1, "a", "float32 array", "atc1");
    Attachment atc2(1, "a", "float32 array", "atc2");;

    json hd1 = {
      {"run", "test.Randomly"},
      {"callID", 0}
    };

    json hd2 = {
      {"run", "test.Randomly2"},
      {"callID", 0}
    };

    Message* msg1 = new Message(hd1);
    Message* msg2 = new Message(hd1);
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
    Message* msg3 = new Message(hd2);
    ASSERT_NE(*msg1, *msg3);
  }

  TEST_F(MessageTest, MissingCallId) {
    json header = {
      {"run", "serviceName"},
      {"someinput", "42"}
    };
    ASSERT_ANY_THROW(new Message(header));

    header["callID"] = 0;
    new Message(header);
  }

  TEST_F(MessageTest, WrongCallIdType) {
    json hd1 = {{"run", "serviceName"},{"callID", nullptr}};
    json hd2 = {{"run", "serviceName"},{"callID", ""}};
    json hd3 = {{"run", "serviceName"},{"callID", {}}};
    ASSERT_ANY_THROW(new Message(hd1));
    ASSERT_ANY_THROW(new Message(hd2));
    ASSERT_ANY_THROW(new Message(hd3));
  }

  TEST_F(MessageTest, NoMessageType) {
    json header = {
      {"wrongmethod", "serviceName"},
      {"someinput", "42"},
      {"callID", 0}
    };

    ASSERT_ANY_THROW(new Message(header));
  }

  TEST_F(MessageTest, MultipleMessageTypes) {
    json header = {
      {"run", "serviceName"},
      {"result", "42"},
      {"callID", 0}
    };

    ASSERT_ANY_THROW(new Message(header));
  }

  TEST_F(MessageTest, RunMessageEmptyObject) {
    json header = {
      {"run", {}},
      {"callID", 0}
    };

    ASSERT_ANY_THROW(new Message(header));
  }

  TEST_F(MessageTest, RunMessageEmptyString) {
    json header = {
      {"run", ""},
      {"callID", 0}
    };
    ASSERT_ANY_THROW(new Message(header));
  }

  TEST_F(MessageTest, RunMessageIntegerName) {
    json header = {
      {"run", 5},
      {"callID", 0}
    };
    ASSERT_ANY_THROW(new Message(header));
  }

  TEST_F(MessageTest, CancelMessageInconsistentCallIds) {
    json header = {
      {"cancel", 1},
      {"callID", 0}
    };
    ASSERT_ANY_THROW(new Message(header));
  }

  TEST_F(MessageTest, CancelMessageConsistentCallIds) {
    json header = {
      {"cancel", 0},
      {"callID", 0}
    };
    new Message(header);
  }

  TEST_F(MessageTest, CancelMessageNoCancelValue) {
    json header = {
      {"cancel", nullptr},
      {"callID", 0}
    };
    new Message(header);
  }

  TEST_F(MessageTest, CancelMessageEmptyObjectCancelValue) {
    json header = {
      {"cancel", {}},
      {"callID", 0}
    };
    new Message(header);
  }

  TEST_F(MessageTest,ProgressMessageMissingFields) {
    json hd1 = {{"progress", 0},{"callID", 0},{"intermediateResult", {}}};
    json hd2 = {{"progress", 0},{"callID", 0}};
    new Message(hd1);
    new Message(hd2);
  }

  TEST_F(MessageTest,ProgressMessageWrongFieldTypes) {
    json hd9 = {{"progress", nullptr},{"callID", 0},{"intermediateResult", {}}};
    json hd10 = {{"progress", ""},{"callID", 0},{"intermediateResult", {}}};
    json hd11 = {{"progress", {}},{"callID", 0},{"intermediateResult", {}}};

    ASSERT_ANY_THROW(new Message(hd9));
    ASSERT_ANY_THROW(new Message(hd10));
    ASSERT_ANY_THROW(new Message(hd11));
  }

  TEST_F(MessageTest, ErrorMessageWrongFieldTypes) {
    json hd0 = {{"error", ""},{"callID", 0}};
    json hd1 = {{"error", {}},{"callID", 0}};
    json hd2 = {{"error", 0},{"callID", 0}};
    json hd3 = {{"error", nullptr},{"callID", 0}};
    new Message(hd0);
    ASSERT_ANY_THROW(new Message(hd1));
    ASSERT_ANY_THROW(new Message(hd2));
    ASSERT_ANY_THROW(new Message(hd3));
  }

  TEST_F(MessageTest, StaticConstructors) {
    Message::RunMessage(0, "serviceName", (json){{"some input", "value"}});
    Message::CancelMessage(0);
    Message::ErrorMessage(0, "some error");
    Message::ResultMessage(0, (json){"some result"});
    Message::ProgressMessage(0, 90, {}, (json){"some result"});
  }
}
