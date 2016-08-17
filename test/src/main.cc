#include "connectiontest.h"
#include "messagetest.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
