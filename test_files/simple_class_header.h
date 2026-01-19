#pragma once
#include <vector>
#include "../library/parser/handlers/all.h"
#define Hallo Hallo

#define GODOT_CLASS(...)

namespace NamespaceA {
	template <typename T>
	using Vector = std::vector<T>;

	namespace B {
		class A {};
	}

	class InnerStruct : public B::A {
		GODOT_CLASS(B::A, "Hallo")

		auto function2123(int a, void*) const -> const int**&;
		const std::vector<int>& vec;
		const Vector<int>& vec2;
		mutable int field;
		int function();
		int&& function1(const int&& huh, std::vector<int>&& vec);
		void _search_box_text_changed(const String &p_query);

		int test();
	};
}
