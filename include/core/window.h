#ifndef LLKIT_WINDOW_H
#define LLKIT_WINDOW_H

#include <string_view>
#include <optional>
#include "core/display.h"
#include "core/globals.h"
#include "core/registry.h"
#include "errors.h"

#define DEFAULT_DISPLAY ""
#define WL_MAIN_LOOP_FAIL -1
#define XDG_WM_BASE_FAIL -2
#define XDG_WM_SURFACE_FAIL -3
#define XDG_WM_TOPLEVEL_FAIL -4
#define LL_EGL_FAILED_TO_INIT -5

namespace llkit {
	namespace window {
		class obj {
			public:
				explicit obj(const std::string_view& display_name);
				~obj();
				struct wl_display*		get_display(void);
				std::optional<struct llkit_err> main_loop(void);
				std::optional<struct llkit_err> error = std::nullopt;

			private:
				std::shared_ptr<llkit::display::obj>  ldisplay	= nullptr;
				std::unique_ptr<llkit::registry::obj> lregistry = nullptr;
				std::shared_ptr<llkit::globals::obj>  lglobals	= nullptr;
		};
	}
}

#endif // LLKIT_WINDOW_H
