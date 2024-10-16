#ifndef LLKIT_TOUCH_H
#define LLKIT_TOUCH_H

#include <optional>
#include "errors.h"
#include "core/globals.h"

namespace llkit {
	namespace seat {
		namespace touch {
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

#endif // LLKIT_TOUCH_H
