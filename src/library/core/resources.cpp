/**************************************************************************/
/* resources.cpp                                                          */
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

#include "resources.h"

#include "file_system_utilities.h"
#include "string_utilities.h"

namespace GodotObjectCompiler
{

    void Resources::load_pack(ResourcePack* p_pack)
    {
        _loaded_packs.push_back(p_pack);
    }

    Vector<String> Resources::resources_recursive(const Path& p_path) const
    {
        Vector<String> res;
        for (const ResourcePack* pack : _loaded_packs) {
            for (const auto& [res_path, _] : *pack) {
                if (string_prefix(res_path, p_path)) {
                    res.push_back(res_path);
                }
            }
        }
        return res;
    }

    String Resources::load_text_resource(const Path& p_path) const
    {
        for (ResourcePack* pack : _loaded_packs) {
            if (auto itr = pack->find(p_path); itr != pack->end()) {
                return itr->second;
            }
        }
        return "";
    }

    bool Resources::has_resource(const Path& p_path) const
    {
        for (ResourcePack* pack : _loaded_packs) {
            if (auto itr = pack->find(p_path); itr != pack->end()) {
                return true;
            }
        }
        return false;
    }

    bool Resources::copy_resource_to_file(
        const String& p_resource_path, const String& p_target_file) const
    {
        if (!file_exists(p_target_file)) {
            if (String folder_path = path_base(p_target_file);
                !directory_exits(folder_path) && !create_dir_recursive(folder_path)) {
                return false;
            }
            write_file(p_target_file, load_text_resource(p_resource_path));
            return true;
        }
        return false;
    }

    bool Resources::copy_resources_to_folder(
        const Vector<String>& p_resource_glob_paths, const String& p_target_folder) const
    {
        for (const String& copy_resources : p_resource_glob_paths) {
            for (const String& res_path : resources_recursive(copy_resources)) {
                String relative = path_relative(res_path, copy_resources);
                auto file_path = path_concat(
                    path_concat(p_target_folder, string_replace(copy_resources, "res://", "")),
                    relative);
                if (file_exists(file_path)) {
                    continue;
                }

                if (!copy_resource_to_file(res_path, file_path)) {
                    return false;
                }
            }
        }
        return true;
    }

} // namespace GodotObjectCompiler
