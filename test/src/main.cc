#include "lucipp/core/message_tests.h"
#include "lucipp/core/connection_tests.h"
#include "lucipp/node_tests.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
