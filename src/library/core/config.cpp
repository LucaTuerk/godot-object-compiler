/**************************************************************************/
/* config.cpp                                                             */
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

#include "library/core/config.h"

#include "file_system_utilities.h"
#include "library/library_context.h"
#include "permissions.h"

namespace GodotObjectCompiler
{

    const Vector<String>& JsonConfig::get_sections()
    {
        if (sections_dirty) {
            sections.clear();
            for (auto& [key, value] : json.items()) {
                if (value.is_object()) {
                    sections.push_back(key);
                }
            }
        }

        return sections;
    }

    bool JsonConfig::has_config_value(const String& p_key)
    {
        if (current_section.empty()) {
            return json.contains(p_key);
        }
        return json[current_section].contains(p_key);
    }

    String JsonConfig::dump()
    {
        return json.dump(1);
    }

    bool JsonConfig::write_to_file(const Path& p_path)
    {
        try {
            PANIC_COND(
                !Permissions::instance()->is_allowed_write_path(p_path),
                "JsonConfig: \"%s\" is not an allowed write path.");
            std::ofstream file(p_path.path());
            file << json.dump(1);
        } catch (std::exception& e) {
            PRINT_ERROR(
                "JsonConfig: Failed to write config to file \"%s\" due to exception: %s",
                p_path.c_str(), e.what());
            return false;
        }
        return true;
    }

    bool JsonConfig::read_from_file(const Path& p_path)
    {
        if (!file_exists(p_path)) {
            return false;
        }

        try {
            std::ifstream file(p_path.path());
            json = Json::parse(file);
        } catch (std::exception& e) {
            PRINT_ERROR(
                "JsonConfig: Failed to read config from file \"%s\" due to exception: %s",
                p_path.c_str(), e.what());
            return false;
        }
        return true;
    }

    void JsonConfig::_write_to_section(const String& p_section)
    {
        if (current_section == p_section) {
            return;
        }
        current_section = p_section;
        sections_dirty = true;
    }

    void JsonConfig::_write(const String& p_key, const String& p_value)
    {
        try {
            if (current_section.empty()) {
                json[p_key] = p_value;
            } else {
                json[current_section][p_key] = p_value;
            }
        } catch (std::exception& e) {
            PANIC("JsonConfig: Exception writing to key \"%s\": %s", p_key.c_str(), e.what());
        }
    }

    void JsonConfig::_read_from_section(const String& p_section)
    {
        current_section = p_section;
    }

    String JsonConfig::_read(const String& p_key)
    {
        try {
            if (current_section.empty()) {
                return json[p_key];
            } else {
                return json[current_section][p_key];
            }
        } catch (std::exception& e) {
            PANIC("JsonConfig: Exception reading from key \"%s\": %s", p_key.c_str(), e.what());
        }
    }

} // namespace GodotObjectCompiler
