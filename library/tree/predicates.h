
#pragma once

#include "library/core/core.h"
#include "library/parser/tree_sitter_node.h"

namespace GodotObjectCompiler {

  namespace NamedContextPredicates {

    template <typename T>
    Predicate<T> name(const char* p_name) {
      return [p_name](Ref<T> node) { return node->name() == p_name; };
    }

    template <typename T>
    Predicate<T> qualified_name(const char* p_qualified_name) {
      return [p_qualified_name](Ref<T> node) { return node->qualified_name() == p_qualified_name; };
    }

  }  // namespace NamedContextPredicates

  namespace ClassPredicates {

    template <typename T>
    Predicate<T> base_class(const char* p_base_class) {
      return [p_base_class](Ref<T> node) {
        auto baseclasses = node->base_classes();
        return baseclasses.find(p_base_class) != baseclasses.end();
      };
    }

  }  // namespace ClassPredicates

}  // namespace GodotObjectCompiler
