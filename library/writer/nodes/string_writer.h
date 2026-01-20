#pragma once
#include <sstream>

#include "library/core/core.h"

namespace GodotObjectCompiler {
  class IStringWriter {
  public:
    virtual ~IStringWriter() = default;
    virtual void write(const String& value) = 0;
    virtual String get_string() = 0;
  };

  class StringWriter : public IStringWriter {
  public:
    void write(const String& value) override;
    String get_string() override;

   private:
    std::stringstream stream;
  };

}  // namespace GodotObjectCompiler

