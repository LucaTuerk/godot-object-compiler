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
#include "library/core/string_utilities.h"
#include "library/tree/output/output.h"
#include "library/type_db.h"

namespace GodotObjectCompiler
{

    Vector<Vector<Size>> find_all_subsets(Size n)
    {
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

    bool GodotMacroIncludeGenerator::generate_macros(const Ref<Context>& p_write_to)
    {
        p_write_to->add_child(Output::Define(
            "GOC_BODY_COMBINE_INNER",
            {Output::Text("A"), Output::Text("B"), Output::Text("C"), Output::Text("D")},
            "A##B##C##D"));
        p_write_to->add_child(Output::Define(
            "GOC_BODY_COMBINE",
            {Output::Text("A"), Output::Text("B"), Output::Text("C"), Output::Text("D")},
            "GOC_BODY_COMBINE_INNER(A, B, C, D)"));
        String generated_content = "GOC_BODY_COMBINE(GOC_GENERATED_, __LINE__, _, GOC_FILE_ID())()";

        for (const String& macro :
             LibraryContext::instance()->get_attribute_db()->get_all_macros()) {
            String res_file = "res://" + path_concat_ext("doc", macro, "txt");
            if (Resources::instance()->has_resource(res_file)) {
                Ref<Context> params_docu = Output::Params({});
                Ref<Context> comment = Output::DocComment(Output::Lines(
                    {Output::Text(Resources::instance()->load_text_resource(res_file)),
                     Output::NewLine(),
                     Output::Spaces(
                         {Output::BoldText("Available Parameters: "),
                          params_docu->as<Output::OutputNode>()})}));
                p_write_to->add_child(comment);

                Size index = 0;
                Vector<Ref<IAttributeParameterType>> params =
                    LibraryContext::instance()->get_attribute_db()->get_parameters_for_macro(macro);
                for (const Ref<IAttributeParameterType>& param : params) {
                    if ((++index % 5) != 0) {
                        params_docu->add_child(Output::Text(param->get_return_type()));
                    } else {
                        params_docu->add_child(Output::Text("\n" + param->get_return_type()));
                    }
                }
            }

            Result<Attribute> attr_result =
                LibraryContext::instance()->get_attribute_db()->create_for_macro(macro);
            PANIC_COND(attr_result.has_error(), "Failed to create macro attribute");
            Ref<Attribute> attr = attr_result.get_result();

            if (attr->is<GeneratedBodyAttribute>() || attr->is<GeneratedGlobalAttribute>()) {
                p_write_to->add_child(Output::Define(
                    macro, {Output::Text("...")},
                    generated_content +
                        "; static_assert( [](){ using namespace GOC_Macros; return " + macro +
                        "_prototype(__VA_ARGS__);},\"\")"));
            } else {
                p_write_to->add_child(Output::Define(
                    macro, {Output::Text("...")},
                    "static_assert( [](){ using namespace GOC_Macros; return " + macro +
                        "_prototype(__VA_ARGS__);},\"\")"));
            }
        }
        return true;
    }

    bool GodotMacroIncludeGenerator::generate_attribute_parameter_type(
        const Ref<IAttributeParameterType>& p_type, const Ref<Context>& p_write_to)
    {
        if (p_type->is_builtin()) {
            return true;
        }

        const String type_name = p_type->get_return_type();

        const String doc_res_path = "res://" + path_concat("doc", type_name + ".txt");
        const String doc_res_dir = "res://" + path_concat("doc", type_name);

        const Ref<Context> value_names_documentation = Output::Params({});
        const Ref<Context> type_documentation = Output::Lines({});

        if (Resources::instance()->has_resource(doc_res_path)) {
            const String content = Resources::instance()->load_text_resource(doc_res_path);
            type_documentation->add_child(Output::Text(content));
            type_documentation->add_child(Output::NewLine());
        }
        type_documentation->add_child(Output::Spaces(
            {Output::BoldText("Possible Values:"),
             value_names_documentation->as<Output::OutputNode>()}));

        p_write_to->add_child(Output::DocComment(type_documentation));
        p_write_to->B<Output::SnippetNode>("class " + type_name + " {};");
        p_write_to->add_child(Output::NewLine());

        if ((p_type->get_features() & IAttributeParameterType::FEATURE_FLAG)) {
            p_write_to->B<Function>()[{
                B<ConstExpression>(), B<Type>()[B<Identifier>(type_name)],
                B<Identifier>("operator |"),
                B<Parameters>()[{
                    B<Parameter>()[{B<Type>()[B<Identifier>(type_name)], B<Identifier>(" a")}],
                    B<Parameter>()[{B<Type>()[B<Identifier>(type_name)], B<Identifier>(" b")}]}],
                B<Body>()[Output::Return("{}")]}];
        }

        Size index = 0;
        const Vector<IAttributeParameterType::Argument> arguments = p_type->get_arguments();
        for (const String& value_name : p_type->get_value_names()) {
            auto value_res_path = path_concat_ext(doc_res_dir, value_name, "txt");

            if (Resources::instance()->has_resource(value_res_path)) {
                String content = Resources::instance()->load_text_resource(value_res_path);
                p_write_to->add_child(Output::DocComment(Output::Text(content)));
            }
            if ((++index % 5) == 0) {
                value_names_documentation->add_child(Output::Text("\n" + value_name));
            } else {
                value_names_documentation->add_child(Output::Text(value_name));
            }

            if (arguments.empty()) {
                p_write_to->B<Field>()[{
                    B<ConstExpression>(), B<Type>()[B<Identifier>(type_name)],
                    B<Identifier>(value_name), B<Literal>("= {}")}][Output::Semicolon()];
            } else {
                Ref<Parameters> parameters;
                p_write_to->B<Function>()[{
                    B<ConstExpression>(), B<Type>()[B<Identifier>(type_name)],
                    B<Identifier>(value_name), R<Parameters>(&parameters),
                    B<Body>()[Output::Return("{}")]}];

                for (const auto& argument : arguments) {
                    if (argument.type == IAttributeParameterType::ARG_STRING) {
                        Ref<Parameter> parameter;
                        parameters->add_child(R<Parameter>(&parameter)[{
                            B<Type>()[{B<Const>(), B<Identifier>("char"), B<Pointer>()}],
                            B<Identifier>(argument.name),
                        }]);
                        if (argument.optional) {
                            parameter->B<Literal>("\"\"");
                        }
                    } else if (argument.type == IAttributeParameterType::ARG_INTEGER) {
                        Ref<Parameter> parameter;
                        parameters->add_child(R<Parameter>(&parameter)[{
                            B<Type>()[B<Identifier>("int")],
                            B<Identifier>(argument.name),
                        }]);
                        if (argument.optional) {
                            parameter->B<Literal>("0");
                        }
                    } else {
                        PANIC(
                            "Unimplemented IAttributeParameterType %d",
                            static_cast<int>(argument.type));
                    }
                }
            }

            p_write_to->add_child(Output::NewLine());
        }
        return true;
    }

    bool GodotMacroIncludeGenerator::generate_prototype_methods(
        const Ref<Context>& p_write_to, const String& p_macro,
        const Vector<Ref<IAttributeParameterType>>& p_params)
    {
        Vector<Vector<Size>> subsets = find_all_subsets(p_params.size());

        p_write_to->B<Function>()[{
            B<ConstExpression>(), B<Type>()[B<Identifier>("bool")],
            B<Identifier>(p_macro + "_prototype"), B<Parameters>()[B<Parameter>()],
            B<Body>()[Output::Return("true")]}];
        p_write_to->add_child(Output::NewLine());

        for (const Ref<IAttributeParameterType>& param : p_params) {
            p_write_to->B<Function>()[{
                B<ConstExpression>(), B<Type>()[B<Identifier>("bool")],
                B<Identifier>(p_macro + "_prototype"),
                B<Parameters>()[B<Parameter>()[Output::Text(param->get_return_type())]],
                B<Body>()[Output::Return("true")]}];
            p_write_to->add_child(Output::NewLine());
        }

        for (Size size = 2; size <= p_params.size(); ++size) {
            Ref<Output::ListNode> template_params;
            Ref<Parameters> params;
            Ref<Body> body;

            p_write_to->B<Output::ListNode>(" ", false, false)[{
                Output::Text("template "),
                B<Output::EnclosingNode>("<", ">").with_child_ref<Output::ListNode>(
                    &template_params, ", ", false, false)}];

            p_write_to->B<Function>()[{
                B<ConstExpression>(), B<Type>()[B<Identifier>("bool")],
                B<Identifier>(p_macro + "_prototype"), R<Parameters>(&params), R<Body>(&body)}];

            p_write_to->add_child(Output::NewLine());

            for (Size j = 0; j < size; ++j) {
                template_params->add_child(Output::FmtText("typename T%d", j + 1));
                params->B<Parameter>()[Output::FmtText("T%d p_arg%d", j + 1, j + 1)];
            }

            for (Size curr = 0; curr < size - 1; ++curr) {
                for (Size cmp = curr + 1; cmp < size; ++cmp) {
                    body->add_child(Output::FmtText(
                        "static_assert(!std::is_same_v<T%d,T%d>, \"Duplicate argument types %d and "
                        "%d\");",
                        curr + 1, cmp + 1, curr + 1, cmp + 1));
                }
            }

            for (Size curr = 0; curr < size; ++curr) {
                body->add_child(
                    Output::FmtText("%s_prototype(p_arg%d);", p_macro.c_str(), curr + 1));
            }

            body->add_child(Output::Return("true"));
        }

        return true;
    }

    Ref<Context> GodotMacroIncludeGenerator::generate(Ref<Context> p_tree, Ref<Node> p_entry_point)
    {
        UNUSED(p_tree);

        Ref<Context> entry = p_entry_point->as<Context>();
        if (!entry) {
            return nullptr;
        }
        entry->add_child(Output::PragmaOnce());
        entry->add_child(Output::SystemInclude("type_traits"));

        generate_macros(entry);

        Ref<Body> namespace_body;
        entry->B<Namespace>()[{B<Identifier>("GOC_Macros"), R<Body>(&namespace_body)}];

        HashSet<String> generated_param_types;
        for (const String& macro :
             LibraryContext::instance()->get_attribute_db()->get_all_macros()) {
            Vector<Ref<IAttributeParameterType>> params =
                LibraryContext::instance()->get_attribute_db()->get_parameters_for_macro(macro);

            for (const auto& param : params) {
                if (generated_param_types.find(param->get_return_type()) ==
                    generated_param_types.end()) {
                    generated_param_types.insert(param->get_return_type());
                    generate_attribute_parameter_type(param, namespace_body);
                }
            }

            generate_prototype_methods(namespace_body, macro, params);
        }

        return entry;
    }

} // namespace GodotObjectCompiler
