
#include "main.h"

#include "library/core/helpers.h"
#include "test_registry.h"
#include "tests/parser/all.h"
#include "tests/core/all.h"

int main(int argc, char* argv[]) {
  using namespace GodotObjectCompiler;
  Size failed_count = 0;
  Size success_count = 0, ignore_count = 0, all_count = 0;
  for (const auto& [test_name, test_functor] : TestRegistry::instance()->get_tests()) {
    all_count++;

    TestResult result = test_functor();

    switch (result) {
      case TEST_RESULT_SUCCESS:
        print_ln(format("%s\tSuccess!", test_name.c_str()));
        success_count++;
        break;
      case TEST_RESULT_FAILURE:
        print_err(format("%s\tFailed!", test_name.c_str()));
        failed_count++;
        break;
      case TEST_RESULT_IGNORED:
        print_ln(format("%s\tIgnored!", test_name.c_str()));
        ignore_count++;
        break;
    }
  }

  print_ln(format("Summary: %d failed, %d succeeded, %d ignored, %d tests run", failed_count, success_count,
      ignore_count, all_count));

  if (failed_count != 0) {
    return 1;
  }
  return 0;
}