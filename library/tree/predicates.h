
#pragma once

#include "library/core/core.h"

namespace GodotObjectCompiler {

  namespace NamedContextPredicates {

    template <typename T>
    Predicate<T> name(const char* name) {
      return [name](Ref<T> node) { return node->name() == name; };
    }

    template <typename T>
    Predicate<T> qualified_name(const char* qualified_name) {
      return [qualified_name](Ref<T> node) { return node->qualified_name() == qualified_name; };
    }

  }  // namespace NamedContextPredicates

  namespace ClassPredicates {

    template <typename T>
    Predicate<T> base_class(const char* base_class) {
      return [base_class](Ref<T> node) {
        auto baseclasses = node->base_classes();
        return baseclasses.find(base_class) != baseclasses.end();
      };
    }

  }  // namespace ClassPredicates

}  // namespace GodotObjectCompiler
