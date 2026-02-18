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

namespace GodotObjectCompiler {

  Vector<String> directory_files(const String& p_path);

  Vector<String> directory_files_recursive(const String& p_path);

  Vector<String> directory_dirs(const String& p_path);

  Vector<String> directory_entries(const String& p_path);

  String read_file(const String& p_path);

  Vector<String> read_lines(const String& p_path);

  void write_file(const String& p_path, const String& p_content);

  void write_initial_file_content(const String& p_path, const String& p_initial_content);

  bool create_dir_recursive(const String& p_path);

  bool file_exists(const String& p_path);

  bool remove_file(const String& p_path);

  bool copy_file(const String& p_source, const String& p_destination);

  bool remove(const String& p_path);

  bool directory_exits(const String& p_path);

  Size file_write_time(const String& p_path);

  String input(const String& p_prompt, const String& p_default_value = "");

  String path_base(const String& p_path);

  String path_concat(const String& p_left, const String& p_right);

  String path_concat_ext(const String& p_dir, const String& p_filename, const String& p_extension);

  String path_relative(const String& p_path, const String& p_base);

  String path_absolute(const String& p_path);

  String path_file_name(const String& p_path);

  String path_cwd();

  String path_stem(const String& p_path);

  bool path_is_descendant(const String& p_possible_ancestor, const String& p_possible_child);

  bool could_be_path(const String& p_path);

  bool could_be_dir_path(const String& p_path);

  bool could_be_file_path(const String& p_path);
}
