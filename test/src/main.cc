#include "luciconnect/message_tests.h"
#include "luciconnect/connection_tests.h"
#include "luciconnect/node_tests.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
