/**************************************************************************/
/* type_db.h                                                              */
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
#pragma once
#include "core/assumption.h"
#include "core/core.h"
#include "core/result.h"
#include "library/tree/syntax/context.h"
#include "tree/syntax/attribute.h"
#include "tree/syntax/type.h"

namespace GodotObjectCompiler
{

    class Attribute;
    class Type;
    class Define;
    class Class;
    class Enum;
    class EnumValue;

    class ConfigNodeReaderWriter : public INodeReader, public INodeWriter
    {
      public:
        bool write_to_file(Ref<Node> node, const String& path) override;

        Result<Node> read_from_file(const String& path) override;
    };

    template <typename T> struct AssumeType {
        Ref<T> type;

        AssumeType() = default;

        AssumeType(const String& type_name, const Size& template_arg_count = 0)
            : name(type_name), template_parameter_count(template_arg_count)
        {
        }

      private:
        String name;
        Size template_parameter_count;

        friend class TypeDB;
    };

    class TypeDB : public IAssumptionValidator<AssumeType<Enum>>,
                   public IAssumptionValidator<AssumeType<Class>>,
                   public IAssumptionValidator<AssumeType<Define>>,
                   public IAssumptionValidator<AssumeType<EnumValue>>
    {
      private:
        struct Private {
        };

      public:
        static inline const char* INVALID_NAME = "___INVALID___";

        void set_cache_directory(const String& path);

        void save_type_data(const Ref<NamedContext>& p_type, const String& p_generated_from) const;

        void save_type_attribute(
            const Ref<NamedContext>& p_type, const Ref<Attribute>& p_attribute,
            const String& p_generated_from) const;

        Result<Node> get_type_data(
            const String& qualified_name, Size template_parameter_count = 0,
            const Ref<Namespace>& from_namespace = nullptr);

        Result<Attribute> get_type_attribute(
            const String& p_qualified_name, const String& p_attribute_name,
            Size p_template_parameter_count = 0, const Ref<Namespace>& p_from_namespace = nullptr);

        Result<Node>
        get_type_data(const Ref<Type>& type, const Ref<Namespace>& from_namespace = nullptr);

        template <typename T>
        Result<T> get_type_data(
            const String& qualified_name, Size template_parameter_count = 0,
            const Ref<Namespace>& from_namespace = nullptr);

        template <typename T>
        Result<T>
        get_type_data(const Ref<Type>& type, const Ref<Namespace>& from_namespace = nullptr);

        template <typename T>
        Result<T>
        get_type_attribute(const Ref<Type>& type, const Ref<Namespace>& from_namespace = nullptr);

        template <typename T>
        AssumptionState validate_t(Assumption<AssumeType<T>>& type_assumption);

        AssumptionState validate_assumption(Assumption<AssumeType<Enum>>& p_assumption) override;

        AssumptionState validate_assumption(Assumption<AssumeType<Class>>& p_assumption) override;

        AssumptionState validate_assumption(Assumption<AssumeType<Define>>& p_assumption) override;

        AssumptionState
        validate_assumption(Assumption<AssumeType<EnumValue>>& p_assumption) override;

        static String mangle_name(const String& qualified_name, Size template_parameter_count);

        static Vector<String> resolve_possible_namespaces(
            const String& qualified_name, const Ref<Namespace>& from_namespace);

        TypeDB() = delete;

        TypeDB(Private)
        {
        }

      private:
        using Reader = ConfigNodeReaderWriter;
        using Writer = ConfigNodeReaderWriter;

        enum class CacheType { READONLY_CACHE, READWRITE_CACHE };

        Result<Node> _get_type_data(
            const String& p_qualified_name, Size template_parameter_count,
            const Ref<Namespace>& p_from_namespace, CacheType p_cache_type);

        Result<Attribute> _get_type_attribute(
            const String& p_qualified_name, const String& p_attribute_name,
            Size p_template_parameter_count, const Ref<Namespace>& p_from_namespace,
            CacheType cache_type);

        [[nodiscard]] String _get_cache_file_path(
            const String& p_qualified_name, CacheType p_cache_type,
            Size p_template_argument_count = INVALID_SIZE) const;

        [[nodiscard]] String _get_attribute_cache_file_path(
            const String& p_qualified_name, const String& p_attribute_name, CacheType p_cache_type,
            Size p_template_argument_count = INVALID_SIZE) const;

        Dictionary<String, Ref<Node>> _cache;
        String _cache_directory;
        String _readonly_cache_directory;

        friend LibraryContext;
    };

    template <typename T>
    Result<T> TypeDB::get_type_data(
        const String& qualified_name, Size template_parameter_count,
        const Ref<Namespace>& from_namespace)
    {
        Result<Node> result =
            get_type_data(qualified_name, template_parameter_count, from_namespace);
        RESULT_ERROR_PASS_ON(Error, result, node);
        ERROR_COND(
            !node->is<T>(), "Invalid type. Expected \"%s\" but is \"%s\"",
            T::get_type_static().c_str(), node->get_type().c_str());
        return node->as<T>();
    }

    template <typename T>
    Result<T> TypeDB::get_type_data(const Ref<Type>& type, const Ref<Namespace>& from_namespace)
    {
        Result<Node> result = get_type_data(type, from_namespace);
        RESULT_ERROR_PASS_ON(Error, result, node);
        ERROR_COND(
            !node->is<T>(), "Invalid type. Expected \"%s\" but is \"%s\"",
            T::get_type_static().c_str(), node->get_type().c_str());
        return node->as<T>();
    }

    template <typename T>
    Result<T>
    TypeDB::get_type_attribute(const Ref<Type>& type, const Ref<Namespace>& from_namespace)
    {
        Result<Attribute> result = get_type_attribute(
            type->qualified_name(), T::get_type_static(), type->template_argument_count(),
            from_namespace);
        RESULT_ERROR_PASS_ON(Error, result, node);
        ERROR_COND(
            !node->is<T>(), "Invalid type. Expected \"%s\" but is \"%s\"",
            T::get_type_static().c_str(), node->get_type().c_str());
        return node->as<T>();
    }

    template <typename T>
    AssumptionState TypeDB::validate_t(Assumption<AssumeType<T>>& type_assumption)
    {
        AssumeType<T>& value = UNSAFE_VALUE_EXTRACTOR::GET_VERY_UNSAFELY(type_assumption);
        Result<T> result = get_type_data<T>(value.name, value.template_parameter_count);
        if (result.has_error()) {
            return STATE_INVALID;
        }

        value.type = result.get_result();
        return STATE_VALID;
    }

} // namespace GodotObjectCompiler
