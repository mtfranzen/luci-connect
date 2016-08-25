#include "node_tests.h"

namespace {
  TEST_F(AbstractNodeTest, Simple) {
    node_->Run();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}
