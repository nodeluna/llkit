#ifndef LLKIT_DISPLAY_H
#define LLKIT_DISPLAY_H

#include <string_view>
#include <optional>
#include <expected>
#include <wayland-client.h>
#include "errors.h"

#define WL_DISPLAY_INIT_FAIL -1

namespace llkit {
	namespace display {
		class obj {
			public:
				obj() {};
				~obj();
				explicit obj(const std::string_view& name);
				struct wl_display*					    get_display(void);
				const char*						    get_name(void);
				void							    set_name(const std::string_view& name_);
				static std::expected<llkit::display::obj, struct llkit_err> init(const std::string_view& name);
				std::optional<struct llkit_err>				    error = std::nullopt;

			private:
				struct wl_display* display = nullptr;
				std::string	   name;
		};
	}
}

#endif // LLKIT_DISPLAY_H
