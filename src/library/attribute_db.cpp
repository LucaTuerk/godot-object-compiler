/**************************************************************************/
/* attribute_db.cpp                                                       */
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

#include "attribute_db.h"

#include "core/result.h"
#include "library/generator/generator.h"
#include "library/tree/syntax/attribute.h"

namespace GodotObjectCompiler
{

    bool AttributeDB::register_attribute(
        const String& p_class_name, const String& p_macro, CreationFunc p_creator)
    {
        _macro_aliases[p_macro] = p_class_name;
        _creation_funcs[p_class_name] = p_creator;
        return true;
    }

    bool AttributeDB::register_attribute_parameter(
        const String& p_class_name, const Ref<IAttributeParameterType>& p_parameter)
    {
        HashSet<String>& registered = _registered_parameter_types[p_class_name];
        if (registered.find(p_parameter->get_type()) != registered.end()) {
            return false;
        }

        registered.insert(p_parameter->get_type());
        _parameters[p_class_name].push_back(p_parameter);
        return true;
    }

    bool AttributeDB::is_known_macro(const String& p_macro)
    {
        return _macro_aliases.find(p_macro) != _macro_aliases.end();
    }

    Result<Attribute> AttributeDB::create_for_macro(const String& p_macro)
    {
        const auto name_itr = _macro_aliases.find(p_macro);
        ERROR_COND(name_itr == _macro_aliases.end(), "Unknown macro \"%s\"", p_macro.c_str())

        const auto creator_itr = _creation_funcs.find(name_itr->second);
        ERROR_COND(
            creator_itr == _creation_funcs.end(), "No creator function bound for macro \"%s\"",
            p_macro.c_str());

        return creator_itr->second();
    }

    Vector<Ref<IAttributeParameterType>>
    AttributeDB::get_parameters_for_macro(const String& p_macro)
    {
        const auto name_itr = _macro_aliases.find(p_macro);
        if (name_itr == _macro_aliases.end()) {
            return {};
        }

        return _parameters[name_itr->second];
    }

    String AttributeDB::get_macro_for_attribute(const String& p_class_name)
    {
        for (const auto& [macro, name] : _macro_aliases) {
            if (name == p_class_name) {
                return macro;
            }
        }
        return "";
    }

    Vector<String> AttributeDB::get_all_macros()
    {
        Vector<String> macros;
        for (auto& [key, val] : _macro_aliases) {
            macros.push_back(key);
        }
        return macros;
    }

    bool AttributeDB::register_class_generator(
        const String& p_generator_name, const Ref<ClassGenerator>& p_generator)
    {
        if (_registered_generator_names.find(p_generator_name) ==
            _registered_generator_names.end()) {
            _registered_generator_names.insert(p_generator_name);
            _class_generators.push_back(p_generator);
            return true;
        }
        return false;
    }

    const Vector<Ref<ClassGenerator>>& AttributeDB::class_generators() const
    {
        return _class_generators;
    }

} // namespace GodotObjectCompiler
