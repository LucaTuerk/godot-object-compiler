#include "main.h"

#include <chrono>
#include <iostream>

#include "../library/core/config.h"
#include "../library/tree/output/output_file.h"
#include "../library/tree/syntax/all.h"
#include "library/core/core.h"
#include "library/core/helpers.h"
#include "library/core/timer.h"
#include "library/execution_context.h"
#include "library/generator/generated_class_generator.h"
#include "library/parser/parser.h"
#include "library/tree/iterators.h"
#include "library/tree/predicates.h"

using namespace GodotObjectCompiler;

int main() {
  Vector<String> paths = {
      "/home/luca/Repositories/godot-object-compiler/test_files/"
      "simple_class_header.h",
      "/home/luca/Repositories/godot-object-compiler/test_files/"
      "simple_class_header.h"};

  constexpr std::array<Size, 3> column_size = {10, 30, 10};

  for (const String& path : paths) {
    Timer timer{"Handling " + path};
    ExecutionContext* context = ExecutionContext::instance();
    context->init();
    context->set_remove_macros(read_lines(".goc/macro_remove.txt"));
    context->set_include_paths(read_lines(".goc/include_paths.txt"));

    TreeSitterParser* parser = new TreeSitterParser();

    Namespace* ns = nullptr;
    {
      Timer parse_timer{"Parse \"" + path + "\""};
      ns = parser->parse(read_file(path))->as<Namespace>();
    }

    Namespace* target = default_construct<Namespace>()->as<Namespace>();
    {
      Timer merge_timer{"Merge Includes \"" + path + "\""};
      // ns->merge_includes(target);
    }

    GodotGeneratedClassGenerator generator;

    for (Class* _class : BranchIterator<Class>(ns, BFS)) {
      print_ln(_class->name());
    }

    Vector<Class*> classes = ns->classes_recursive();
    for (Class* cl : classes) {
      Context* generated = generator.generate(ns, cl);
      for (Node* child : generated->get_children()) {
        if (OutputFile* output = child->as<OutputFile>()) {
          output->print();
        }
      }
    }
  }
  return 0;
};
//
// {
//   Timer search_timer{"Find A"};
//   Class* A = target->find_descendant<Class>(BFS, NamedContextPredicates::name<Class>("A"));
//   if (A) {
//     auto bases = A->base_classes();
//     for (auto base : bases) {
//       print(base->qualified_name() + " ");
//     }
//     print_ln("");
//   }
// }
//
// {
//   Timer search_timer{"Find C"};
//   Class* C = target->find_descendant<Class>(BFS, NamedContextPredicates::name<Class>("C"));
//   if (C) {
//     auto bases = C->base_classes();
//     for (auto base : bases) {
//       print(base->qualified_name() + " ");
//     }
//     print_ln("");
//   }
// }
