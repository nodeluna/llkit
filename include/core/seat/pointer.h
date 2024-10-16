#ifndef LLKIT_POINTER_H
#define LLKIT_POINTER_H

#include <optional>
#include "errors.h"
#include "core/globals.h"

namespace llkit {
	namespace seat {
		namespace pointer {
			class obj {
				public:
					obj();
					~obj();
					std::optional<struct llkit_err> error = std::nullopt;

				private:
					llkit::globals::obj* globals = nullptr;
			};
		}
	}
}

#endif // LLKIT_POINTER_H
