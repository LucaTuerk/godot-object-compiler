/**************************************************************************/
/* library_context.cpp                                                    */
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

#include "library_context.h"

#include "attribute_db.h"
#include "core/file_system_utilities.h"
#include "library/core/config.h"
#include "library/core/core.h"
#include "library/node_db.h"
#include "parser.h"
#include "type_db.h"

namespace GodotObjectCompiler
{

    void LibraryContext::force_regenerate(const Path& p_path)
    {
        Path absolute = path_absolute(p_path);
        clear_generated_files(p_path);
        last_modified_times.erase(absolute);
        out_last_modified_times.erase(absolute);
    }

    const Vector<Ref<IParser>>& LibraryContext::get_parsers() const
    {
        return parsers;
    }

    void LibraryContext::set_default_parser(const String& p_name, int p_capabilities)
    {
        for (const auto& parser : parsers) {
            if (parser->get_type() == p_name) {
                PANIC_COND(
                    (parser->get_capabilities() & p_capabilities) == 0,
                    "Parser %s does not support the requested capabilities %d (has %d)",
                    parser->get_type().c_str(), p_capabilities, parser->get_capabilities());
                default_parsers.insert_or_assign(p_capabilities, parser);
                return;
            }
        }
        PANIC("Parser %s could not be found.", p_name.c_str());
    }

    Ref<IParser>
    LibraryContext::get_default_parser(const int p_capabilities, bool p_get_most_capable)
    {
        if (p_get_most_capable) {
            int max = 0;
            Ref<IParser> default_parser = nullptr;
            for (const auto& [capabilities, parser] : default_parsers) {
                if ((capabilities & p_capabilities) == p_capabilities && capabilities > max) {
                    max = capabilities;
                    default_parser = parser;
                }
            }
            return default_parser;
        }

        auto itr = default_parsers.find(p_capabilities);
        return itr == default_parsers.end() ? nullptr : itr->second;
    }

    void LibraryContext::set_temporary_path(const Path& p_path)
    {
        temp_path = p_path;
    }

    Path LibraryContext::get_temporary_path() const
    {
        return temp_path;
    }

    String error_level_to_string(ErrorLevel level)
    {
        switch (level) {
        case ERROR:
            return "Error";
        case WARNING:
            return "Warning";
        case INFO:
            return "Info";
        case VERBOSE:
            return "Verbose";
        }
        return "";
    }

    LibraryContext* LibraryContext::instance()
    {
        static LibraryContext _instance = LibraryContext();
        if (!_instance.initialized) {
            _instance.init();
        }
        return &_instance;
    }

    void LibraryContext::init()
    {
        node_db = make_ref<NodeDB>(NodeDB::Private());
        attribute_db = make_ref<AttributeDB>(AttributeDB::Private());
        type_db = make_ref<TypeDB>(TypeDB::Private());
        usings = {};
        temp_path = Path();
        input_files = {};
        remove_macros = {};
        include_paths = {};
        generated_from = {};
        last_modified_times = {};
        out_last_modified_times = {};
        generic_singletons = {};
        parsers = {};
        error_level = INFO;
        error_detail = FULL;
        initialized = true;

        for (const RegisterCallback& callback : register_callbacks) {
            callback(this);
        }
    }

    void LibraryContext::ensure_unique_include_paths()
    {
        HashSet<Path> paths;
        for (auto itr = include_paths.begin(); itr != include_paths.end();) {
            if (paths.find(*itr) == paths.end()) {
                paths.insert(*itr);
                ++itr;
            } else {
                itr = include_paths.erase(itr);
            }
        }
    }

    bool LibraryContext::add_register_callback(const RegisterCallback& callback)
    {
        register_callbacks.push_back(callback);
        return true;
    }

    Hash LibraryContext::get_path_hash(const String& p_absolute_path)
    {
        Hasher<String> hasher;
        return hasher(p_absolute_path);
    }

    NodeDB* LibraryContext::get_node_db() const
    {
        return node_db.get();
    }

    AttributeDB* LibraryContext::get_attribute_db() const
    {
        return attribute_db.get();
    }

    TypeDB* LibraryContext::get_type_db() const
    {
        return type_db.get();
    }

    const Vector<String>& LibraryContext::get_remove_macros()
    {
        return remove_macros;
    }

    void LibraryContext::set_remove_macros(const Vector<String>& p_value)
    {
        remove_macros = p_value;
    }

    const Vector<Path>& LibraryContext::get_include_paths() const
    {
        return include_paths;
    }

    void LibraryContext::set_include_paths(const Vector<Path>& p_value)
    {
        include_paths = p_value;
        ensure_unique_include_paths();
    }

    void LibraryContext::add_include_paths(const Vector<Path>& p_value)
    {
        include_paths.insert(include_paths.begin(), p_value.begin(), p_value.end());
        ensure_unique_include_paths();
    }

    void LibraryContext::set_error_level(ErrorLevel p_level, ErrorDetail p_error_detail)
    {
        error_level = p_level;
        error_detail = p_error_detail;
    }

    ErrorLevel LibraryContext::get_error_level() const
    {
        return error_level;
    }

    ErrorDetail LibraryContext::get_error_detail() const
    {
        return error_detail;
    }

    void LibraryContext::print(ErrorLevel p_level, const String& p_message) const
    {
        if (p_level >= error_level) {
            print_ln(p_message);
        }
    }

    void LibraryContext::register_generated_file(
        const Path& p_generated_path, const Path& p_generated_from_path)
    {
        generated_from[p_generated_from_path].push_back(p_generated_path);
    }

    bool LibraryContext::load_generated_from_file(const Path& p_path)
    {
        JsonConfig config;
        if (!config.read_from_file(p_path)) {
            return false;
        }

        generated_from.clear();

        for (const String& key : config.get_sections()) {
            config.read_from_section(key);
            if (config.has_config_value("generated_files")) {
                Vector<Path> generated =
                    path_vector_split(config.read<String, String>("generated_files"), ";");
                generated_from.try_emplace(Path(key), generated);
            }
        }

        return true;
    }

    bool LibraryContext::save_generated_from_file(const Path& p_path) const
    {
        JsonConfig config;

        for (const auto& [path, generated] : generated_from) {
            config.write_to_section(path.string());
            config.write("generated_files", path_vector_combine(generated, ";"));
        }

        return config.write_to_file(p_path);
    }

    void LibraryContext::clear_generated_from()
    {
        generated_from.clear();
    }

    void LibraryContext::regenerate_file_apply()
    {
        for (const Path& path : regenerate_files) {
            last_modified_times.erase(path);
            out_last_modified_times.erase(path);

            if (auto itr = generated_from.find(path); itr != generated_from.end()) {
                for (const Path& generated_file : itr->second) {
                    if (file_exists(generated_file)) {
                        read_file(path);
                    }
                }
            }
            generated_from.erase(path);
        }
        regenerate_files.clear();
    }

    void LibraryContext::clear_last_modified_times()
    {
        last_modified_times.clear();
        out_last_modified_times.clear();
    }

    void LibraryContext::regenerate_file(const Path& p_path)
    {
        regenerate_files.push_back(p_path);
    }

    void LibraryContext::clean_generated_files()
    {
        auto itr = generated_from.begin();
        while (itr != generated_from.end()) {
            const auto& [path, generated_files] = *itr;

            if (!path.empty() && !file_exists(path)) {
                for (const Path& generated_file : generated_files) {
                    PRINT_VERBOSE(
                        "Removing orphan \"%s\", generated from \"%s\"", generated_file.c_str(),
                        path.c_str());
                    if (file_exists(generated_file)) {
                        remove_file(generated_file);
                    }
                }
                itr = generated_from.erase(itr);
                continue;
            }

            for (const Path& generated_file : generated_files) {
                if (!file_exists(generated_file)) {
                    regenerate_file(path);
                    break;
                }
            }
            ++itr;
        }

        regenerate_file_apply();
    }

    bool LibraryContext::clear_generated_files(const Path& p_path)
    {
        const auto itr = generated_from.find(p_path);

        if (itr == generated_from.end()) {
            return false;
        }

        for (const Path& generated : itr->second) {
            if (file_exists(generated)) {
                PRINT_VERBOSE(
                    "Removing orphan \"%s\", generated from \"%s\"", generated.c_str(),
                    p_path.c_str());
                remove_file(generated);
            }
        }
        generated_from.erase(p_path);
        return true;
    }

    bool LibraryContext::load_last_modified_times_file(const Path& p_path)
    {
        JsonConfig config;
        if (!config.read_from_file(p_path)) {
            return false;
        }

        last_modified_times.clear();
        out_last_modified_times.clear();

        for (const String& section : config.get_sections()) {
            config.read_from_section(section);
            if (config.has_config_value("last_modified")) {
                last_modified_times[Path(section)] = config.read<String, Size>("last_modified");
                out_last_modified_times[Path(section)] = config.read<String, Size>("last_modified");
            }
        }

        return true;
    }

    bool LibraryContext::save_last_modified_times_file(const Path& p_path) const
    {
        JsonConfig config;

        for (const auto& [path, last_modified] : out_last_modified_times) {
            config.write_to_section(path.string());
            config.write<String, Size>("last_modified", last_modified);
        }

        return config.write_to_file(p_path);
    }

    bool LibraryContext::file_modified(const Path& p_path, bool p_update_time)
    {
        const Path absolute = path_absolute(p_path);
        const Size last_modified = file_write_time(absolute);

        if (last_modified == 0) {
            return false;
        }

        bool modified = false;

        if (const auto itr = last_modified_times.find(absolute); itr == last_modified_times.end()) {
            modified = true;
        } else {
            modified = itr->second != last_modified;
        }

        if (p_update_time) {
            out_last_modified_times[absolute] = last_modified;
        }

        return modified;
    }

    void LibraryContext::set_usings(const Vector<String>& p_value)
    {
        usings = p_value;
    }

    const Vector<String>& LibraryContext::get_usings()
    {
        return usings;
    }

    void LibraryContext::add_using(const String& p_value)
    {
        usings.push_back(p_value);
    }

    void LibraryContext::remove_using(const String& p_value)
    {
        if (const auto itr = std::find(usings.begin(), usings.end(), p_value);
            itr != usings.end()) {
            usings.erase(itr);
        }
    }

    void LibraryContext::clear_usings()
    {
        usings.clear();
    }

} // namespace GodotObjectCompiler
