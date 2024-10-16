#ifndef LLKIT_WINDOW_H
#define LLKIT_WINDOW_H

#include <string_view>
#include <optional>
#include "core/display.h"
#include "core/globals.h"
#include "core/registry.h"
#include "errors.h"

namespace llkit {
	namespace window {
		class obj {
			public:
				explicit obj(const std::string_view& display_name);
				~obj();
				struct wl_display*		get_display(void);
				std::optional<struct llkit_err> error = std::nullopt;

			private:
				std::shared_ptr<llkit::display::obj>  ldisplay	= nullptr;
				std::unique_ptr<llkit::registry::obj> lregistry = nullptr;
				std::shared_ptr<llkit::globals::obj>  lglobals	= nullptr;
		};
	}
}

#endif // LLKIT_WINDOW_H
