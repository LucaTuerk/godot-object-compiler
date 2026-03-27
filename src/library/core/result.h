/**************************************************************************/
/* result.h                                                               */
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
class Error;
class Node;

template <typename ResultT, typename ErrorT = Error> class Result
{
public:
  Result(const Ref<ResultT>& p_result);
  Result(const Ref<ErrorT>& p_error);

  Ref<ResultT> get_result() const;
  bool get_result(Ref<ResultT>& r_result) const;
  Ref<ErrorT> get_error() const;
  bool get_error(Ref<ErrorT>& r_error) const;

  bool has_error() const;
  bool has_result() const;

private:
  bool _has_result = false;
  Ref<Node> _data;
};

template <typename ResultT, typename ErrorT>
Result<ResultT, ErrorT>::Result(const Ref<ResultT>& p_result)
{
  _data = p_result;
  _has_result = true;
}

template <typename ResultT, typename ErrorT>
Result<ResultT, ErrorT>::Result(const Ref<ErrorT>& p_error)
{
  _data = p_error;
  _has_result = false;
}

template <typename ResultT, typename ErrorT>
Ref<ResultT> Result<ResultT, ErrorT>::get_result() const
{
  PANIC_COND(!_has_result, "Invalid result access.");
  return std::dynamic_pointer_cast<ResultT>(_data);
}

template <typename ResultT, typename ErrorT>
bool Result<ResultT, ErrorT>::get_result(Ref<ResultT>& r_result) const
{
  if (!_has_result) {
    r_result = nullptr;
    return false;
  }
  r_result = std::dynamic_pointer_cast<ResultT>(_data);
  return true;
}

template <typename ResultT, typename ErrorT>
Ref<ErrorT> Result<ResultT, ErrorT>::get_error() const
{
  PANIC_COND(_has_result, "Invalid error access.");
  return std::dynamic_pointer_cast<ErrorT>(_data);
}

template <typename ResultT, typename ErrorT>
bool Result<ResultT, ErrorT>::get_error(Ref<ErrorT>& r_error) const
{
  if (_has_result) {
    r_error = nullptr;
    return false;
  }
  r_error = std::dynamic_pointer_cast<ErrorT>(_data);
  return true;
}

template <typename ResultT, typename ErrorT>
bool Result<ResultT, ErrorT>::has_error() const
{
  return !_has_result;
}

template <typename ResultT, typename ErrorT>
bool Result<ResultT, ErrorT>::has_result() const
{
  return _has_result;
}
} // namespace GodotObjectCompiler

#define RESULT_ERROR_PASS_ON(type, result, unwrapped_name)                     \
  if (result.has_error()) {                                                    \
    return ERROR_CAST(type, result.get_error());                               \
  }                                                                            \
  auto unwrapped_name = result.get_result();
