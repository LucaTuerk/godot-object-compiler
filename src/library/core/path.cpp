/**************************************************************************/
/* path.cpp                                                               */
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

#include "path.h"

namespace GodotObjectCompiler
{
    Path::Path(const char* p_path) : data(std::filesystem::u8path(p_path))
    {
    }

    Path::Path(const String& p_path) : data(std::filesystem::u8path(p_path))
    {
    }

    Path::Path(const std::filesystem::path& p_path) : data(p_path)
    {
    }

    Path::Path(const Path& other) : data(other.data)
    {
    }

    Path::Path(Path&& other) noexcept : data(std::move(other.data))
    {
    }

    Path& Path::operator=(const Path& other)
    {
        if (this == &other)
            return *this;
        data = other.data;
        return *this;
    }

    Path& Path::operator=(Path&& other) noexcept
    {
        if (this == &other)
            return *this;
        data = std::move(other.data);
        return *this;
    }

    const std::filesystem::path& Path::path() const
    {
        return data;
    }

    String Path::string() const
    {
        return data.u8string();
    }

    const char* Path::c_str() const
    {
        string_data = string();
        return string_data.c_str();
    }

    Path Path::parent_path() const
    {
        return Path(data.parent_path());
    }

    Path Path::filename() const
    {
        return Path(data.filename());
    }

    Path Path::stem() const
    {
        return Path(data.stem());
    }

    String Path::extension() const
    {
        return data.extension().string();
    }

    bool Path::empty() const
    {
        return data.empty();
    }

    void Path::replace_extension(const String& p_extension)
    {
        data.replace_extension(p_extension);
    }

    bool Path::operator<(const Path& p_other) const
    {
        return data < p_other.data;
    }

    bool Path::operator>(const Path& p_other) const
    {
        return data > p_other.data;
    }

    bool Path::operator==(const Path& p_other) const
    {
        return data == p_other.data;
    }

    bool Path::operator!=(const Path& p_other) const
    {
        return data != p_other.data;
    }

} // namespace GodotObjectCompiler
