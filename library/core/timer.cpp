
#include "timer.h"

#include "string_writer.h"

namespace GodotObjectCompiler {

  Timer::Timer(const String& step) : _step(step) {}

  String ensure_lenght(const String& message, Size length) {
    if (message.length() < length) {
      StreamWriter stream_writer;
      for (Size i = 0; i < length - message.length(); i++) {
        stream_writer.write(" ");
      }
      stream_writer.write(message);
      return stream_writer.get_string();
    }

    return message;
  }

  Timer::~Timer() {
    using namespace std::chrono;
    auto duration = duration_cast<milliseconds>(steady_clock::now() - begin);
    print(ensure_lenght(std::to_string(duration.count()), 10));
    print(" [ms]\t");
    print_ln(_step);
  }

}  // namespace GodotObjectCompiler