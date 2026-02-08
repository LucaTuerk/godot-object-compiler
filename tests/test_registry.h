
#pragma once
#include "library/core/core.h"
#include "library/core/string_writer.h"
#include "library/tree/syntax/function.h"

namespace GodotObjectCompiler {

  enum TestResult { TEST_RESULT_SUCCESS, TEST_RESULT_FAILURE, TEST_RESULT_IGNORED };

  using TestFunctor = std::function<TestResult()>;

  class TestRegistry {
   public:

    static TestRegistry* instance() {
      static TestRegistry instance;
      return &instance;
    }

    bool register_test(const String& name, TestFunctor functor);
    const Dictionary<String, TestFunctor>& get_tests();

   private:

    Dictionary<String, TestFunctor> tests;
  };

  class TestRegister {
    String name;

   public:

    explicit TestRegister(const String& name) : name(name) {}

    bool operator<<(TestFunctor functor) const;
  };

}  // namespace GodotObjectCompiler

#define GOC_TEST(name)                                                                        \
  static inline bool __##name##__test_registered__ = GodotObjectCompiler::TestRegister(#name) \
                                                     << []() -> GodotObjectCompiler::TestResult

#define GOC_TEST_IGNORE() return GodotObjectCompiler::TEST_RESULT_IGNORED;

#define GOC_TEST_ASSERT(condition, message)          \
  if (!(condition)) {                                \
    print_err(message);                              \
    return GodotObjectCompiler::TEST_RESULT_FAILURE; \
  }

#define GOC_TEST_EQ(a, b, message)                              \
  if (!((a) == (b))) {                                          \
    GodotObjectCompiler::StreamWriter writer;                   \
    writer.write(message);                                      \
    writer.write(" Expected to be ");                           \
    writer.write_generic(b);                                    \
    writer.write_generic(" but was "), writer.write_generic(a); \
    writer.write("."), print_err(writer.get_string());          \
    return GodotObjectCompiler::TEST_RESULT_FAILURE;            \
  }

#define GOC_TEST_NEQ(a, b, message)                             \
  if (((a) == (b))) {                                           \
    GodotObjectCompiler::StreamWriter writer;                   \
    writer.write(message);                                      \
    writer.write(" Expected not equal to ");                    \
    writer.write_generic(b);                                    \
    writer.write_generic(" but was "), writer.write_generic(a); \
    writer.write("."), print_err(writer.get_string());          \
    return GodotObjectCompiler::TEST_RESULT_FAILURE;            \
  }

#define GOC_TEST_PARSE_FILE(path)                                       \
  Ref<Namespace> global_namespace = node_new<Namespace>();              \
  {                                                                     \
    TreeSitterParser parser;                                            \
    Ref<ParserError> error = parser.parse_file(path, global_namespace); \
    GOC_TEST_EQ(error, ParserError::OK, "Parser error occurred");       \
  }                                                                     \
  GOC_TEST_ASSERT(global_namespace, "Global Namespace is invalid.");\

