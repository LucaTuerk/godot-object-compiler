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
        static LibraryContext* instance()
        {
            static LibraryContext singleton = LibraryContext();
            return &singleton;
        }

        using RegisterCallback = std::function<void(LibraryContext*)>;

        static bool add_register_callback(const RegisterCallback& callback);

        [[nodiscard]] NodeDB* get_node_db() const;

        [[nodiscard]] AttributeDB* get_attribute_db() const;

        [[nodiscard]] TypeDB* get_type_db() const;

        [[nodiscard]] ErrorLevel get_error_level() const;

        [[nodiscard]] ErrorDetail get_error_detail() const;

        const Vector<String>& get_remove_macros();

        void register_generated_file(
            const String& p_generated_path, const String& p_generated_from_path);

        bool load_generated_from_file(const String& p_path);

        bool save_generated_from_file(const String& p_path);

        void clear_generated_from();

        void regenerate_file(const String& p_path);

        bool load_last_modified_times_file(const String& p_path);

        bool save_last_modified_times_file(const String& p_path);

        void clear_last_modified_times();

        bool file_modified(const String& p_path, bool p_update_time = true);

        void clean_generated_files();

        bool clear_generated_files(const String& p_path);

        void set_remove_macros(const Vector<String>& p_value);

        const Vector<String>& get_include_paths();

        void set_include_paths(const Vector<String>& p_value);

        void set_error_level(ErrorLevel p_level, ErrorDetail p_error_detail);

        void set_usings(const Vector<String>& p_value);

        const Vector<String>& get_usings();

        void add_using(const String& p_value);

        void remove_using(const String& p_value);

        void clear_usings();

        void print(ErrorLevel p_level, const String& p_message) const;

        void force_regenerate(const String& p_path);

      private:
        void init();

        static Hash get_path_hash(const String& p_absolute_path);

        LibraryContext();

        Ref<NodeDB> node_db;
        Ref<AttributeDB> attribute_db;
        Ref<TypeDB> type_db;

        String cache_path;
        Vector<String> usings;
        Vector<String> input_files;
        Vector<String> remove_macros;
        Vector<String> include_paths;
        Dictionary<String, Vector<String>> generated_from;
        Dictionary<String, Size> last_modified_times;
        Dictionary<String, Size> out_last_modified_times;

        ErrorLevel error_level;
        ErrorDetail error_detail;

        static inline Vector<RegisterCallback> register_callbacks;
        friend class Application;
    };

} // namespace GodotObjectCompiler

#define PRINT_LEVEL(level, ...)                                                                    \
    if (LibraryContext::instance()->get_error_level() >= (level)) {                                \
        if (level != ERROR)                                                                        \
            fmt_print_ln(__VA_ARGS__);                                                             \
        else                                                                                       \
            fmt_print_err(__VA_ARGS__);                                                            \
    }

#define PRINT_ERROR(...) PRINT_LEVEL(ERROR, __VA_ARGS__)

#define PRINT_WARNING(...) PRINT_LEVEL(WARNING, __VA_ARGS__)

#define PRINT_INFO(...) PRINT_LEVEL(INFO, __VA_ARGS__)

#define PRINT_VERBOSE(...) PRINT_LEVEL(VERBOSE, __VA_ARGS__)
