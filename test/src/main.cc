#include "lc2pp/core/message_tests.h"
#include "lc2pp/core/connection_tests.h"
#include "lc2pp/node_tests.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
