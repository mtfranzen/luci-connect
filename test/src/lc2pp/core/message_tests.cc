#include "lc2pp/core/message_tests.h"

/**
* Contains tests for constructing messages.
*/
namespace {

  TEST_F(MessageTest, CreateMessageAndGetHeader) {
    lc2pp::core::Message* message = new lc2pp::core::Message(simple_header_);

    json header2 = message->GetHeader();
    ASSERT_EQ(simple_header_, header2);
  }

  TEST_F(MessageTest, CompareMessage) {
    lc2pp::core::Attachment atc1(1, "a", "float32 array", "atc1");
    lc2pp::core::Attachment atc2(1, "a", "float32 array", "atc2");;

    json hd1 = {
      {"run", "test.Randomly"},
      {"callID", 0}
    };

    json hd2 = {
      {"run", "test.Randomly2"},
      {"callID", 0}
    };

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

  TEST_F(MessageTest, MissingCallId) {
    json header = {
      {"run", "serviceName"},
      {"someinput", "42"}
    };
    ASSERT_ANY_THROW(new lc2pp::core::Message(header));

    header["callID"] = 0;
    new lc2pp::core::Message(header);
  }

  TEST_F(MessageTest, WrongCallIdType) {
    json hd1 = {{"run", "serviceName"},{"callID", nullptr}};
    json hd2 = {{"run", "serviceName"},{"callID", ""}};
    json hd3 = {{"run", "serviceName"},{"callID", {}}};
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd1));
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd2));
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd3));
  }

  TEST_F(MessageTest, NoMessageType) {
    json header = {
      {"wrongmethod", "serviceName"},
      {"someinput", "42"},
      {"callID", 0}
    };

    ASSERT_ANY_THROW(new lc2pp::core::Message(header));
  }

  TEST_F(MessageTest, MultipleMessageTypes) {
    json header = {
      {"run", "serviceName"},
      {"result", "42"},
      {"callID", 0}
    };

    ASSERT_ANY_THROW(new lc2pp::core::Message(header));
  }

  TEST_F(MessageTest, RunMessageEmptyObject) {
    json header = {
      {"run", {}},
      {"callID", 0}
    };

    ASSERT_ANY_THROW(new lc2pp::core::Message(header));
  }

  TEST_F(MessageTest, RunMessageEmptyString) {
    json header = {
      {"run", ""},
      {"callID", 0}
    };
    ASSERT_ANY_THROW(new lc2pp::core::Message(header));
  }

  TEST_F(MessageTest, RunMessageIntegerName) {
    json header = {
      {"run", 5},
      {"callID", 0}
    };
    ASSERT_ANY_THROW(new lc2pp::core::Message(header));
  }

  TEST_F(MessageTest, CancelMessageInconsistentCallIds) {
    json header = {
      {"cancel", 1},
      {"callID", 0}
    };
    ASSERT_ANY_THROW(new lc2pp::core::Message(header));
  }

  TEST_F(MessageTest, CancelMessageConsistentCallIds) {
    json header = {
      {"cancel", 0},
      {"callID", 0}
    };
    new lc2pp::core::Message(header);
  }

  TEST_F(MessageTest, CancelMessageNoCancelValue) {
    json header = {
      {"cancel", nullptr},
      {"callID", 0}
    };
    new lc2pp::core::Message(header);
  }

  TEST_F(MessageTest, CancelMessageEmptyObjectCancelValue) {
    json header = {
      {"cancel", {}},
      {"callID", 0}
    };
    new lc2pp::core::Message(header);
  }

  TEST_F(MessageTest,ProgressMessageMissingFields) {
    json hd1 = {{"progress", {}},{"callID", 0},{"serviceName", "testService"},{"taskID", 5},{"percentage", 1}};
    json hd2 = {{"progress", {}},{"callID", 0},{"taskID", 5},{"percentage", 1}};
    json hd3 = {{"progress", {}},{"callID", 0},{"serviceName", "testService"},{"percentage", 1}};
    json hd4 = {{"progress", {}},{"callID", 0},{"serviceName", "testService"},{"taskID", 5}};
    new lc2pp::core::Message(hd1);
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd2));
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd3));
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd4));
  }

  TEST_F(MessageTest,ProgressMessageWrongFieldTypes) {
    json hd3 = {{"progress", {}},{"callID", 0},{"serviceName", 5},{"taskID", 5},{"percentage", 1}};
    json hd4 = {{"progress", {}},{"callID", 0},{"serviceName", {}},{"taskID", 5},{"percentage", 1}};
    json hd5 = {{"progress", {}},{"callID", 0},{"serviceName", nullptr},{"taskID", 5},{"percentage", 1}};
    json hd6 = {{"progress", {}},{"callID", 0},{"serviceName", "testService"},{"taskID", ""},{"percentage", 1}};
    json hd7 = {{"progress", {}},{"callID", 0},{"serviceName", "testService"},{"taskID", {}},{"percentage", 1}};
    json hd8 = {{"progress", {}},{"callID", 0},{"serviceName", "testService"},{"taskID", nullptr},{"percentage", 1}};
    json hd9 = {{"progress", {}},{"callID", 0},{"serviceName", "testService"},{"taskID", 5},{"percentage", ""}};
    json hd10 = {{"progress", {}},{"callID", 0},{"serviceName", "testService"},{"taskID", 5},{"percentage", {}}};
    json hd11 = {{"progress", {}},{"callID", 0},{"serviceName", "testService"},{"taskID", 5},{"percentage", nullptr}};

    ASSERT_ANY_THROW(new lc2pp::core::Message(hd3));
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd4));
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd5));
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd6));
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd7));
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd8));
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd9));
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd10));
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd11));
  }

  TEST_F(MessageTest,ResultMessageMissingFields) {
    json hd1 = {{"result", {}},{"callID", 0},{"serviceName", "testService"},{"taskID", 5}};
    json hd2 = {{"result", {}},{"callID", 0},{"taskID", 5}};
    json hd3 = {{"result", {}},{"callID", 0},{"serviceName", "testService"}};
    new lc2pp::core::Message(hd1);
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd2));
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd3));
  }

  TEST_F(MessageTest,ResultMessageWrongFieldTypes) {
    json hd3 = {{"result", {}},{"callID", 0},{"serviceName", 5},{"taskID", 5}};
    json hd4 = {{"result", {}},{"callID", 0},{"serviceName", {}},{"taskID", 5}};
    json hd5 = {{"result", {}},{"callID", 0},{"serviceName", nullptr},{"taskID", 5}};
    json hd6 = {{"result", {}},{"callID", 0},{"serviceName", "testService"},{"taskID", ""}};
    json hd7 = {{"result", {}},{"callID", 0},{"serviceName", "testService"},{"taskID", {}}};
    json hd8 = {{"result", {}},{"callID", 0},{"serviceName", "testService"},{"taskID", nullptr}};

    ASSERT_ANY_THROW(new lc2pp::core::Message(hd3));
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd4));
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd5));
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd6));
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd7));
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd8));
  }

  TEST_F(MessageTest, ErrorMessageWrongFieldTypes) {
    json hd0 = {{"error", ""},{"callID", 0}};
    json hd1 = {{"error", {}},{"callID", 0}};
    json hd2 = {{"error", 0},{"callID", 0}};
    json hd3 = {{"error", nullptr},{"callID", 0}};
    new lc2pp::core::Message(hd0);
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd1));
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd2));
    ASSERT_ANY_THROW(new lc2pp::core::Message(hd3));
  }
}
