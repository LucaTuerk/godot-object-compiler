/**************************************************************************/
/* string_writer.h                                                        */
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
#include "library/core/core.h"
#include "reader_writer.h"

namespace GodotObjectCompiler {

class StreamWriter : public IStringWriter {
public:
	void write(const String &p_value) override;

	String get_string() override;

	Size current_length() override;

private:
	std::stringstream _stream;
	Size _current_length = 0;
};

class FileWriter : public IStringWriter {
public:
	FileWriter(const FileWriter &other) = delete;

	FileWriter(FileWriter &&other) noexcept
			: IStringWriter(std::move(other)),
			  _path(std::move(other._path)),
			  _do_not_write_same_content(other._do_not_write_same_content),
			  _generated(other._generated),
			  _moved(false),
			  _stream(std::move(other._stream)),
			  _file(std::move(other._file)) {
		other._moved = true;
	}

	FileWriter &operator=(const FileWriter &other) = delete;

	FileWriter &operator=(FileWriter &&other) noexcept {
		if (this == &other) {
			return *this;
		}
		IStringWriter::operator=(std::move(other));
		other._moved = true;
		_path = std::move(other._path);
		_do_not_write_same_content = other._do_not_write_same_content;
		_generated = other._generated;
		_moved = false;
		_stream = std::move(other._stream);
		_file = std::move(other._file);
		return *this;
	}

	explicit FileWriter(const String &path, bool do_not_write_same_content = true);

	~FileWriter() override;

	static FileWriter generated(const String &path, const String &p_generated_from);

	void write(const String &p_value) override;

	String get_string() override;

	Size current_length() override;

private:
	FileWriter(const String &path, const String &initial_content);

	static String _generated_header(const String &p_file_name);

	String _path;
	bool _do_not_write_same_content = true;
	bool _generated = false;
	bool _moved = false;
	StreamWriter _stream;
	std::fstream _file;
};

} // namespace GodotObjectCompiler
