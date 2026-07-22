/**************************************************************************/
/* permissions.cpp                                                        */
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

#include "permissions.h"

#include "file_system_utilities.h"
#include "string_utilities.h"

namespace GodotObjectCompiler
{

    void Permissions::clear()
    {
        allowed_write_paths.clear();
    }

    void Permissions::add_write_path(const Path& p_path)
    {
        const String absolute = path_absolute(p_path);
        allowed_write_paths.insert(absolute);
    }

    bool Permissions::is_allowed_write_path(const Path& p_path) const
    {
        String absolute = path_absolute(p_path);
        if (!path_is_descendant(path_cwd(), absolute)) {
            return false;
        }

        return std::find_if(
                   allowed_write_paths.begin(), allowed_write_paths.end(),
                   [absolute](const String& allowed) {
                       return string_prefix(absolute, allowed);
                   }) != allowed_write_paths.end();
    }

    void Permissions::ensure_is_allowed_write_path(const Path& p_path) const
    {
        PANIC_COND(
            !is_allowed_write_path(p_path),
            "Trying to write to \"%s\" but it is not an allowed write path!",
            path_absolute(p_path).c_str());
    }

} // namespace GodotObjectCompiler
