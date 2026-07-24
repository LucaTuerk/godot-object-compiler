/**************************************************************************/
/* library_context.h                                                      */
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
#include "core/core.h"

namespace GodotObjectCompiler
{
    class IParser;

    class NodeDB;
    class AttributeDB;
    class TypeDB;

    enum ErrorLevel {
        ERROR = 0,
        WARNING = 1,
        INFO = 2,
        VERBOSE = 3,
    };

    enum ErrorDetail {
        CONDENSED = 0,
        FULL = 1,
    };

    String error_level_to_string(ErrorLevel level);

    class LibraryContext
    {
      public:
        static LibraryContext* instance();

        using RegisterCallback = std::function<void(LibraryContext*)>;

        static bool add_register_callback(const RegisterCallback& callback);

        [[nodiscard]] NodeDB* get_node_db() const;

        [[nodiscard]] AttributeDB* get_attribute_db() const;

        [[nodiscard]] TypeDB* get_type_db() const;

        [[nodiscard]] ErrorLevel get_error_level() const;

        [[nodiscard]] ErrorDetail get_error_detail() const;

        const Vector<String>& get_remove_macros();

        template <typename T> Ref<T> get_singleton();

        void
        register_generated_file(const Path& p_generated_path, const Path& p_generated_from_path);

        bool load_generated_from_file(const Path& p_path);

        bool save_generated_from_file(const Path& p_path);

        void clear_generated_from();

        void regenerate_file_apply();

        void regenerate_file(const Path& p_path);

        bool load_last_modified_times_file(const Path& p_path);

        bool save_last_modified_times_file(const Path& p_path);

        void clear_last_modified_times();

        bool file_modified(const Path& p_path, bool p_update_time = true);

        void clean_generated_files();

        bool clear_generated_files(const Path& p_path);

        void set_remove_macros(const Vector<String>& p_value);

        const Vector<Path>& get_include_paths();

        void set_include_paths(const Vector<Path>& p_value);

        void add_include_paths(const Vector<Path>& p_value);

        void set_error_level(ErrorLevel p_level, ErrorDetail p_error_detail);

        void set_usings(const Vector<String>& p_value);

        const Vector<String>& get_usings();

        void add_using(const String& p_value);

        void remove_using(const String& p_value);

        void clear_usings();

        void print(ErrorLevel p_level, const String& p_message) const;

        void force_regenerate(const Path& p_path);

        template <typename T> void register_source_parser();

        const Vector<Ref<IParser>>& get_parsers() const;

        template <typename T> void set_default_parser(int p_capabilities);

        void set_default_parser(const String& p_name, int p_capabilities);

        Ref<IParser> get_default_parser(int p_capabilities, bool p_get_most_capable = true);

        void set_temporary_path(const Path& p_path);

        Path get_temporary_path() const;

      private:
        LibraryContext() = default;
        void init();

        void ensure_unique_include_paths();

        static Hash get_path_hash(const String& p_absolute_path);

        Ref<NodeDB> node_db = nullptr;
        Ref<AttributeDB> attribute_db = nullptr;
        Ref<TypeDB> type_db = nullptr;
        Vector<Ref<IParser>> parsers;
        Dictionary<int, Ref<IParser>> default_parsers;
        Dictionary<TypeIndex, AnyType> generic_singletons;

        String temp_path{};
        Vector<String> usings{};
        Vector<String> input_files{};
        Vector<String> remove_macros{};
        Vector<Path> include_paths{};
        Vector<String> regenerate_files{};
        Dictionary<Path, Vector<String>> generated_from{};
        Dictionary<Path, Size> last_modified_times{};
        Dictionary<Path, Size> out_last_modified_times{};

        ErrorLevel error_level = ERROR;
        ErrorDetail error_detail = FULL;
        bool initialized = false;

        static inline Vector<RegisterCallback> register_callbacks;
        friend class Application;
    };

    template <typename T> Ref<T> LibraryContext::get_singleton()
    {
        const auto itr = generic_singletons.find(typeid(T));
        if (itr == generic_singletons.end()) {
            Ref<T> singleton = make_ref<T>();
            std::ignore = generic_singletons.insert({typeid(T), singleton});
            return singleton;
        }

        return std::any_cast<Ref<T>>(itr->second);
    }

    template <typename T> void LibraryContext::register_source_parser()
    {
        parsers.push_back(make_ref<T>());
    }

    template <typename T> void LibraryContext::set_default_parser(int p_capabilities)
    {
        set_default_parser(T::get_type_static(), p_capabilities);
    }

} // namespace GodotObjectCompiler

#define ONCE(action)                                                                               \
    {                                                                                              \
        static bool __action_called__ = false;                                                     \
        if (!__action_called__) {                                                                  \
            action;                                                                                \
            __action_called__ = true;                                                              \
        }                                                                                          \
    }

#define PRINT_LEVEL(level, ...)                                                                    \
    if (LibraryContext::instance()->get_error_level() >= (level)) {                                \
        if (level != ERROR) {                                                                      \
            fmt_print_ln(__VA_ARGS__);                                                             \
        } else {                                                                                   \
            fmt_print_err(__VA_ARGS__);                                                            \
        }                                                                                          \
    }

#define PRINT_ERROR(...) PRINT_LEVEL(ERROR, __VA_ARGS__)

#define PRINT_WARNING(...) PRINT_LEVEL(WARNING, __VA_ARGS__)

#define PRINT_INFO(...) PRINT_LEVEL(INFO, __VA_ARGS__)

#define PRINT_VERBOSE(...) PRINT_LEVEL(VERBOSE, __VA_ARGS__)
