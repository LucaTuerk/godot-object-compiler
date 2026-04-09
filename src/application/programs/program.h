/**************************************************************************/
/* program.h                                                              */
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
#include "application/application_context.h"
#include "library/core/core.h"
#include "library/tree/syntax/parser_error.h"

namespace GodotObjectCompiler
{

    class ProgramError : public Error
    {
        NODE_TYPE(ProgramError);

      public:
        ProgramError(ErrorLevel level, const String& message) : Error(level, message)
        {
        }

        static inline const Ref<ProgramError> OK = nullptr;
    };

    class IProgram
    {
      public:
        virtual ~IProgram() = default;
        [[nodiscard]] virtual String get_type() const = 0;
        [[nodiscard]] virtual String program_name() const = 0;
        [[nodiscard]] virtual bool requires_project() const = 0;
        virtual bool validate_arguments(ApplicationContext& p_context) = 0;
        virtual Ref<ProgramError> run(ApplicationContext& p_context) = 0;
    };

    using ProgramPath = Vector<String>;

    class Programs
    {
      public:
        static Programs* instance()
        {
            static Programs instance;
            return &instance;
        }

        bool register_program(const Ref<IProgram>& p_program);

        Ref<IProgram> find_program(
            const Vector<String>& p_application_arguments, Vector<String>& r_program_arguments);

        const Dictionary<ProgramPath, Ref<IProgram>>& get_programs();

      private:
        static Size overlap(const ProgramPath& a, const ProgramPath& b);

        HashSet<String> _registered_programs;
        Dictionary<ProgramPath, Ref<IProgram>> _programs;
    };

} // namespace GodotObjectCompiler

#define PROG_ERR(...) return make_ref<ProgramError>(ERROR, format(__VA_ARGS__));

#define PROG_ERR_PASS_ON(target)                                                                   \
    {                                                                                              \
        Ref<ProgramError> __err = (target);                                                        \
        if (__err != ProgramError::OK) {                                                           \
            return __err;                                                                          \
        }                                                                                          \
    }

#define NO_PROGRAM_ARGS                                                                            \
    bool validate_arguments(ApplicationContext& p_context) override                                \
    {                                                                                              \
        return p_context.program_arguments.empty();                                                \
    }

#define PROG_ERR_COND(condition, ...)                                                              \
    if ((condition)) {                                                                             \
        PROG_ERR(__VA_ARGS__)                                                                      \
    }

#define PROGRAM(type, name)                                                                        \
  public:                                                                                          \
    static String get_type_static()                                                                \
    {                                                                                              \
        return #type;                                                                              \
    }                                                                                              \
    virtual String get_type() const override                                                       \
    {                                                                                              \
        return #type;                                                                              \
    }                                                                                              \
    virtual String program_name() const override                                                   \
    {                                                                                              \
        return name;                                                                               \
    }                                                                                              \
    virtual bool requires_project() const override                                                 \
    {                                                                                              \
        return true;                                                                               \
    }                                                                                              \
                                                                                                   \
  private:                                                                                         \
    static inline bool __program_registered_##type = Programs::instance()->register_program(       \
        std::dynamic_pointer_cast<IProgram>(make_ref<type>()));

#define PROJECTLESS_PROGRAM(type, name)                                                            \
  public:                                                                                          \
    static String get_type_static()                                                                \
    {                                                                                              \
        return #type;                                                                              \
    }                                                                                              \
    virtual String get_type() const override                                                       \
    {                                                                                              \
        return #type;                                                                              \
    }                                                                                              \
    virtual String program_name() const override                                                   \
    {                                                                                              \
        return name;                                                                               \
    }                                                                                              \
    virtual bool requires_project() const override                                                 \
    {                                                                                              \
        return false;                                                                              \
    }                                                                                              \
                                                                                                   \
  private:                                                                                         \
    static inline bool __program_registered_##type = Programs::instance()->register_program(       \
        std::dynamic_pointer_cast<IProgram>(make_ref<type>()));
