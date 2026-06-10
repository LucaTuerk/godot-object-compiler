/**************************************************************************/
/* project.cpp                                                            */
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

#include "project.h"

#include "library/core/config.h"
#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"

namespace GodotObjectCompiler
{

    void Project::read_from(IStructuredReader* p_reader)
    {
        p_reader->read_from_section("Godot");
        paths_godot_cpp_include =
            string_split(p_reader->read<String, String>("GodotCppIncludePaths"), ",", true);
        path_extension_api = p_reader->read<String, String>("ExtensionAPI");

        p_reader->read_from_section("Paths");
        paths_root = p_reader->read<String, String>("RootPath");
        paths_goc = p_reader->read<String, String>("GOCPath");
        paths_generated = p_reader->read<String, String>("GeneratedPath");
        paths_cache = p_reader->read<String, String>("CachePath");
        paths_include = string_split(p_reader->read<String, String>("IncludePaths"), ",", true);

        p_reader->read_from_section("Options");
        options_source_parser = p_reader->read<String, String>("SourceParser");
    }

    void Project::write_to(IStructuredWriter* p_writer)
    {
        p_writer->write_to_section("Godot");
        p_writer->write<String, String>(
            "GodotCppIncludePaths", string_vector_combine(paths_godot_cpp_include, ","));
        p_writer->write<String, String>("ExtensionAPI", path_extension_api);
        p_writer->write_to_section("Paths");
        p_writer->write<String, String>("RootPath", paths_root);
        p_writer->write<String, String>("GeneratedPath", paths_generated);
        p_writer->write<String, String>("CachePath", paths_cache);
        p_writer->write<String, String>("GOCPath", paths_goc);
        p_writer->write<String, String>("IncludePaths", string_vector_combine(paths_include, ","));
        p_writer->write_to_section("Options");
        p_writer->write<String, String>("SourceParser", options_source_parser);
    }

    bool Project::read_from_file(const String& p_path)
    {
        JsonConfig config;
        if (!config.read_from_file(p_path)) {
            return false;
        }

        read_from(&config);
        return true;
    }

    bool Project::write_to_file(const String& p_path)
    {
        JsonConfig config;
        write_to(&config);
        return config.write_to_file(p_path);
    }
} // namespace GodotObjectCompiler
