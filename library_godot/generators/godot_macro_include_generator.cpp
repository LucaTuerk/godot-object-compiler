/**************************************************************************/
/* godot_macro_include_generator.cpp                                      */
/*                        ___  ___  ___   ___ _____                       */
/*                       / __|/ _ \|   \ / _ \_   _|                      */
/*                      | (_ | (_) | |) | (_) || |                        */
/*                       \___|\___/|___/ \___/ |_|                        */
/*   ___  ___    _ ___ ___ _____    ___ ___  __  __ ___ ___ _    ___ ___  */
/*  / _ \| _ )_ | | __/ __|_   _|  / __/ _ \|  \/  | _ \_ _| |  | __| _ \ */
/* | (_) | _ \ || | _| (__  | |   | (_| (_) | |\/| |  _/| || |__| _||   / */
/*  \___/|___/\__/|___\___| |_|    \___\___/|_|  |_|_| |___|____|___|_|_\ */
/*                                                                        */
/*              This file is part of Godot Object Compiler                */
/*                  Copyright (c) 2026 Luca Ian Tuerk                     */
/**************************************************************************/
/*                            MIT LICENCE                                 */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "godot_macro_include_generator.h"

#include "library/attribute_db.h"
#include "library/core/file_system_utilities.h"

#include "library/core/resources.h"
#include "library/tree/output/output.h"
#include "library/tree/output/output_file.h"
#include "library/type_db.h"

namespace GodotObjectCompiler {

  Vector<Vector<Size>> find_all_subsets(Size n) {
    if (n == 0) {
      return {{}};
    }

    Vector<Vector<Size>> prev = find_all_subsets(n - 1);
    Vector<Vector<Size>> result;
    std::copy(prev.begin(), prev.end(), std::back_inserter(result));

    for (Vector<Size>& val : result) {
      val.push_back(n - 1);
    }
    std::copy(prev.begin(), prev.end(), std::back_inserter(result));

    return result;
  }

  bool GodotMacroIncludeGenerator::generate_macros(Ref<Context> p_write_to) {
    p_write_to->add_child(Writer::Define("GOC_BODY_COMBINE_INNER",
        {Writer::Text("A"), Writer::Text("B"), Writer::Text("C"), Writer::Text("D")}, "A##B##C##D"));
    p_write_to->add_child(
        Writer::Define("GOC_BODY_COMBINE", {Writer::Text("A"), Writer::Text("B"), Writer::Text("C"), Writer::Text("D")},
            "GOC_BODY_COMBINE_INNER(A, B, C, D)"));
    String generated_content = "GOC_BODY_COMBINE(GOC_GENERATED_, __LINE__, _, GOC_FILE_ID())()";

    for (const String& macro : AttributeDB::instance()->get_all_macros()) {

#ifdef DEV_BUILD
      String doc_file = path_concat_ext("./resources/doc", macro, "txt");
      ensure_file_exists(doc_file, "No documentation available");
#endif

      String res_file = "res://" + path_concat_ext("doc", macro, "txt");
      if (Resources::instance()->has_resource(res_file)) {
        Ref<Context> params_docu = Writer::Params({});
        // clang-format off
        Ref<Context> comment = Writer::DocComment(Writer::Lines({
          Writer::Text(Resources::instance()->load_text_resource(res_file)),
          Writer::NewLine(),
          Writer::Spaces({Writer::BoldText("Available Parameters: "), params_docu->as<Writer::IOutputNode>() })}
        ));
        // clang-format on
        p_write_to->add_child(comment);

        Size index = 0;
        Vector<Ref<IAttributeParameterType>> params = AttributeDB::instance()->get_parameters_for_macro(macro);
        for (const Ref<IAttributeParameterType>& param : params) {
          if ((++index % 5) != 0) {
            params_docu->add_child(Writer::Text(param->get_return_type()));
          } else {
            params_docu->add_child(Writer::Text("\n" + param->get_return_type()));
          }
        }
      }

      Ref<Attribute> attr = AttributeDB::instance()->create_for_macro(macro);
      if (attr->is<GeneratedBodyAttribute>() || attr->is<GeneratedGlobalAttribute>()) {
        p_write_to->add_child(Writer::Define(macro, {Writer::Text("...")},
            generated_content + "; static_assert(" + macro + "_prototype(__VA_ARGS__),\"\");"));
      } else {
        p_write_to->add_child(
            Writer::Define(macro, {Writer::Text("...")}, "static_assert(" + macro + "_prototype(__VA_ARGS__),\"\")"));
      }
    }
    return true;
  }

  bool GodotMacroIncludeGenerator::generate_attribute_parameter_type(
      Ref<IAttributeParameterType> p_type, Ref<Context> p_write_to) {
    String type_name = p_type->get_return_type();

    String doc_res_path = "res://" + path_concat("doc", type_name + ".txt");
    String doc_res_dir = "res://" + path_concat("doc", type_name);

    Ref<Context> value_names_documentation = Writer::Params({});
    Ref<Context> type_documentation = Writer::Lines({});

    if (Resources::instance()->has_resource(doc_res_path)) {
      String content = Resources::instance()->load_text_resource(doc_res_path);
      type_documentation->add_child(Writer::Text(content));
      type_documentation->add_child(Writer::NewLine());
    }
    type_documentation->add_child(
        Writer::Spaces({Writer::BoldText("Possible Values:"), value_names_documentation->as<Writer::IOutputNode>()}));

    p_write_to->add_child(Writer::DocComment(type_documentation));
    p_write_to->build_child<Writer::SnippetNode>("class " + type_name + " {};");
    p_write_to->add_child(Writer::NewLine());

#ifdef DEV_BUILD
    auto doc_file = path_concat_ext("resources/doc", type_name, "txt");
    auto doc_dir = path_concat("resources/doc", type_name);
    create_dir_recursive(doc_dir);
    ensure_file_exists(doc_file, "No documentation available");
#endif

    if ((p_type->get_features() & IAttributeParameterType::FEATURE_FLAG)) {
      // clang-format off
      p_write_to->build_child<Function>().with_children({
        build<ConstExpression>(),
        build<Type>().with_child<Identifier>(type_name),
        build<Identifier>("operator |"),
        build<Parameters>().with_children({
          build<Parameter>().with_children({
            build<Type>().with_child<Identifier>(type_name),
            build<Identifier>(" a")
          }),
            build<Parameter>().with_children({
            build<Type>().with_child<Identifier>(type_name),
            build<Identifier>(" b")
          })
        }),
        build<Body>().with_child(Writer::Return("{}"))
      });
      // clang-format on
    }

    Size index = 0;
    const Vector<IAttributeParameterType::Argument> arguments = p_type->get_arguments();
    for (const String& value_name : p_type->get_value_names()) {
#ifdef DEV_BUILD
      auto value_doc_path = path_concat_ext(doc_dir, value_name, "txt");
      ensure_file_exists(value_doc_path, "No documentation available");
#endif

      auto value_res_path = path_concat_ext(doc_res_dir, value_name, "txt");

      if (Resources::instance()->has_resource(value_res_path)) {
        String content = Resources::instance()->load_text_resource(value_res_path);
        p_write_to->add_child(Writer::DocComment(Writer::Text(content)));
      }
      if ((++index % 5) == 0) {
        value_names_documentation->add_child(Writer::Text("\n" + value_name));
      } else {
        value_names_documentation->add_child(Writer::Text(value_name));
      }

      if (arguments.empty()) {
        // clang-format off
        p_write_to->build_child<Field>().with_children({
          build<ConstExpression>(),
          build<Type>().with_child<Identifier>(type_name),
          build<Identifier>(value_name),
          build<Literal>("= {}")
        }).with_child(Writer::Semicolon());
        // clang-format on
      } else {
        Ref<Parameters> parameters;
        // clang-format off
        p_write_to->build_child<Function>().with_children({
        build<ConstExpression>(),
        build<Type>().with_child<Identifier>(type_name),
        build<Identifier>(value_name),
        build_ref<Parameters>(&parameters),
        build<Body>().with_child(Writer::Return("{}"))});
        // clang-format on

        for (const auto& argument : arguments) {
          if (argument.type == IAttributeParameterType::ARG_STRING) {
            // clang-format off
              Ref<Parameter> parameter;
              parameters->add_child(build_ref<Parameter>(&parameter).with_children({
              build<Type>().with_children({
                build<Const>(),
                build<Identifier>("char"),
                build<Pointer>()
               }),
              build<Identifier>(argument.name),
              }));
            // clang-format on
            if (argument.optional) {
              parameter->build_child<Literal>("\"\"");
            }
          } else {
            PANIC("Unimplemented IAttributeParameterType %d", static_cast<int>(argument.type));
          }
        }
      }

      p_write_to->add_child(Writer::NewLine());
    }
    return true;
  }

  bool GodotMacroIncludeGenerator::generate_prototype_methods(
      const Ref<Context>& p_write_to, const String& p_macro, const Vector<Ref<IAttributeParameterType>>& p_params) {
    Vector<Vector<Size>> subsets = find_all_subsets(p_params.size());
    for (Vector<Size>& subset : subsets) {
      do {
        Ref<Parameters> parameters;
        // clang-format off
        p_write_to->build_child<Function>().with_children({
          build<ConstExpression>(),
          build<Type>().with_child<Identifier>("bool"),
          build<Identifier>(p_macro+"_prototype"),
          build_ref<Parameters>(&parameters),
          build<Body>().with_child(Writer::Return("true"))
        });
        p_write_to->add_child(Writer::NewLine());
        // clang-format on

        for (Size index : subset) {
          parameters->build_child<Parameter>().with_child(Writer::Text(p_params[index]->get_return_type()));
        }
      } while (std::next_permutation(subset.begin(), subset.end()));
    }
    return true;
  }

  Ref<Context> GodotMacroIncludeGenerator::generate(Ref<Context> p_tree, Ref<Node> p_entry_point) {
    Ref<Context> entry = p_entry_point->as<Context>();
    if (!entry) {
      return nullptr;
    }
    entry->add_child(Writer::PragmaOnce());

    generate_macros(entry);

    HashSet<String> generated_param_types;
    for (const String& macro : AttributeDB::instance()->get_all_macros()) {
      Ref<Attribute> attribute = AttributeDB::instance()->create_for_macro(macro);
      Vector<Ref<IAttributeParameterType>> params = AttributeDB::instance()->get_parameters_for_macro(macro);

      for (const auto& param : params) {
        if (generated_param_types.find(param->get_return_type()) == generated_param_types.end()) {
          generated_param_types.insert(param->get_return_type());
          generate_attribute_parameter_type(param, entry);
        }
      }

      generate_prototype_methods(entry, macro, params);
    }

    return entry;
  }

}