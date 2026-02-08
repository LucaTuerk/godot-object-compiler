
#include "test_registry.h"

namespace GodotObjectCompiler {

  bool TestRegistry::register_test(const String& name, TestFunctor functor) {
    auto [_, success] = tests.emplace(name, functor);
    return success;
  }

  const Dictionary<String, TestFunctor>& TestRegistry::get_tests() { return tests; }

  bool TestRegister::operator<<(TestFunctor functor) const {
    return TestRegistry::instance()->register_test(name, std::move(functor));
  }

}  // namespace GodotObjectCompiler