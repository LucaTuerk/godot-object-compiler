/**************************************************************************/
/* nested_namespaces.h                                                    */
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

namespace Tests {
namespace A {

GODOT_CLASS();
class classA : public Object {
	GODOT_GENERATED_BODY();
	GODOT_PROPERTY();
	int propA = 0;
};
namespace B {
GODOT_CLASS();
class classAB : public Object {
	GODOT_GENERATED_BODY();
	GODOT_PROPERTY();
	int propAB = 0;
};
} //namespace B
} //namespace A

namespace C {

GODOT_CLASS();
class classC : public Object {
	GODOT_GENERATED_BODY();
	GODOT_PROPERTY();
	int propC = 0;
};
namespace D {

GODOT_CLASS();
class classCD : public Object {
	GODOT_GENERATED_BODY();
	GODOT_PROPERTY();
	int propCD = 0;
};
} //namespace D
} //namespace C

namespace E {

GODOT_CLASS();
class classE : public Object {
	GODOT_GENERATED_BODY();
	GODOT_PROPERTY();
	int propE = 0;
};
namespace F {

GODOT_CLASS();
class classEF : public Object {
	GODOT_GENERATED_BODY();
	GODOT_PROPERTY();
	int propEF = 0;
};
namespace G {

GODOT_CLASS();
class classEFG : public Object {
	GODOT_GENERATED_BODY();
	GODOT_PROPERTY();
	int propEFG = 0;
};

} //namespace G
} //namespace F
} //namespace E
} //namespace Tests
GODOT_GENERATED_GLOBAL();