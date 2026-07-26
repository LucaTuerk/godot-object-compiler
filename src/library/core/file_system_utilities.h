/**************************************************************************/
/* file_system_utilities.h                                                */
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
#include "core.h"
#include "path.h"

namespace GodotObjectCompiler
{

    Vector<Path> directory_files(const Path& p_path);

    Vector<Path> directory_files_recursive(const Path& p_path);

    Vector<Path> directory_entries(const Path& p_path);

    String read_file(const Path& p_path);

    Vector<String> read_lines(const Path& p_path);

    void write_file(const Path& p_path, const String& p_content);

    void write_initial_file_content(const Path& p_path, const String& p_initial_content);

    bool create_dir_recursive(const Path& p_path);

    bool file_exists(const Path& p_path);

    bool remove_file(const Path& p_path);

    bool remove_directory(const Path& p_path);

    bool remove_entry(const Path& p_path);

    bool directory_exits(const Path& p_path);

    Size file_write_time(const Path& p_path);

    Path input(const String& p_prompt, const Path& p_default_value = "");

    Path path_relative(const Path& p_path, const Path& p_base);

    Path path_absolute(const Path& p_path);

    String path_vector_combine(const Vector<Path>& p_paths, const String& p_delimiter);

    Vector<Path> path_vector_split(const String& p_paths, const String& p_delimiter);

    Path path_cwd();

    char path_seperator();

    bool path_is_descendant(const Path& p_possible_ancestor, const Path& p_possible_child);

    bool could_be_path(const Path& p_path);

    bool could_be_dir_path(const Path& p_path);

    bool could_be_file_path(const Path& p_path);

    String header_path(const Path& p_include_path, const Path& p_file_path);

} // namespace GodotObjectCompiler
