#include <memory>
#include <optional>
#include <xkbcommon/xkbcommon.h>
#include "core/window.h"
#include "core/registry.h"
#include "core/display.h"
#include "core/globals.h"
#include "core/xdg_wm.h"
#include "errors.h"

namespace llkit {
	namespace window {
		obj::obj(const std::string_view& display_name)
		    : ldisplay(std::make_shared<llkit::display::obj>(display_name)), lglobals(std::make_shared<llkit::globals::obj>()) {
			llkit::print("[llkit::window::obj::constructor] entered function");

			if (ldisplay->error) {
				error = ldisplay->error;
				return;
			}

			lglobals->wl_display = ldisplay->get_display();

			lregistry = std::make_unique<llkit::registry::obj>(ldisplay, lglobals);
			if (lregistry->error) {
				error = lregistry->error;
				return;
			}

			if (not lglobals->ll_egl->egl_running()) {
				error = llkit::set_error(true, "failed to initialize egl", LL_EGL_FAILED_TO_INIT);
				return;
			}

			if (not lglobals->compositor) {
				error = llkit::set_error(true, "failed to initialize compositor", WL_MAIN_LOOP_FAIL);
				return;
			}
			llkit::print("[llkit::window::obj::constructor] exiting function");
		}

		std::optional<struct llkit_err> obj::main_loop(void) {
			llkit::print("[llkit::window::obj::main_loop] entered function");
			if (ldisplay->get_display() == nullptr)
				return llkit::set_error(
				    true, "failed to initialize main loop, display isn't initialized", WL_MAIN_LOOP_FAIL);

			llkit::print("[llkit::window::obj::main_loop] before main loop");
			while (wl_display_dispatch(ldisplay->get_display()) && not lglobals->ll_egl->get_close_window()) {
			}

			if (lglobals->ll_egl->get_close_window()) {
				llkit::print("[window closed]");
			}

			llkit::print("[llkit::window::obj::main_loop] exiting function");
			return std::nullopt;
		}

		struct wl_display* obj::get_display(void) {
			return ldisplay->get_display();
		}

		obj::~obj() {
		}
	}
}
