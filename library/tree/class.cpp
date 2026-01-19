
#include "class.h"

#include "identifier.h"

namespace GodotObjectCompiler {

	Vector<Class *> Class::_base_classes_lazy_get() {
		print_err("UNIMPLEMENTED");
		return {};
	}

	bool Class::copy_to(Node *other) const {
		COPY_GUARD(Class, Namespace);
		COPY_LAZY(base_classes);
		return true;
	}

} //namespace GodotObjectCompiler