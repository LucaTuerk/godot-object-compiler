
#pragma once

#include "core.h"

namespace GodotObjectCompiler {

  class Timer {
   public:
    Timer(const String& step);
    ~Timer();

   private:
    String _step;
    TimePoint begin = std::chrono::steady_clock::now();
  };

}  // namespace GodotObjectCompiler
