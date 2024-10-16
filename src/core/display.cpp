#include <print>
#include <expected>
#include <exception>
#include <string_view>
#include <wayland-client-core.h>
#include <wayland-client.h>
#include "errors.h"
#include "core/display.h"

namespace llkit {
	namespace display {
		struct wl_display* obj::get_display(void) {
			return display;
		}

		const char* obj::get_name(void) {
			if (name.empty())
				return nullptr;
			else
				return name.c_str();
		}

		void obj::set_name(const std::string_view& name_) {
			name = name_;
		}

		obj::obj(const std::string_view& name_) : name(name_) {
			if (name.empty())
				display = wl_display_connect(NULL);
			else
				display = wl_display_connect(name.data());

			if (display == NULL)
				error = llkit::set_error(true, "couldn't initialize wayland display", WL_DISPLAY_INIT_FAIL);
		}

		obj::~obj() {
			if (display != nullptr)
				wl_display_disconnect(display);
		}
	}
}
