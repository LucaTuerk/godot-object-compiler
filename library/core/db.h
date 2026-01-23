#pragma once
#include "core.h"

namespace GodotObjectCompiler {

  class Node;
  class Namespace;

  class DB {
   public:

    static DB init(Namespace* root = nullptr);
    static DB read_from_config(const String& path);

    void write_to_config(const String& path) const;
    Namespace* get_root() const;

   private:

    Namespace* _root = nullptr;
  };

}  // namespace GodotObjectCompiler
