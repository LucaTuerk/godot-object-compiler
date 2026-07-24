/**************************************************************************/
/* path.h                                                                 */
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

namespace GodotObjectCompiler
{
    class Path
    {
      public:
        Path() = default;

        Path(const char* p_path);

        explicit Path(const String& p_path);

        explicit Path(const std::filesystem::path& p_path);

        Path(const Path& other);

        Path(Path&& other) noexcept;

        Path& operator=(const Path& other);

        Path& operator=(Path&& other) noexcept;

        const std::filesystem::path& path() const;

        String string() const;

        const char* c_str() const;

        Path parent_path() const;

        Path filename() const;

        Path stem() const;

        String extension() const;

        bool empty() const;

        void replace_extension(const String& p_extension);

        friend Path operator/(const Path& p_left, const Path& p_right);

        friend std::ostream& operator<<(std::ostream& out, const Path& c);

        friend std::istream& operator>>(std::istream& in, Path& c);

        bool operator<(const Path& p_other) const;

        bool operator>(const Path& p_other) const;

        bool operator==(const Path& p_other) const;

        bool operator!=(const Path& p_other) const;

      private:
        mutable String string_data;
        std::filesystem::path data;
    };
} // namespace GodotObjectCompiler
