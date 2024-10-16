#include <memory>
#include <optional>
#include <xkbcommon/xkbcommon.h>
#include "core/window.h"
#include "core/registry.h"
#include "core/display.h"
#include "core/globals.h"
#include "errors.h"

namespace llkit {
	namespace window {
		obj::obj(const std::string_view& display_name)
		    : ldisplay(std::make_shared<llkit::display::obj>(display_name)), lglobals(std::make_shared<llkit::globals::obj>()) {

			if (ldisplay.get()->error) {
				error = ldisplay.get()->error;
				return;
			}

			lregistry = std::make_unique<llkit::registry::obj>(ldisplay, lglobals);
			if (lregistry.get()->error) {
				error = lregistry.get()->error;
				return;
			}

			lglobals->xkb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
			if (not lglobals->xkb_context) {
				error = llkit::set_error(true, "failed to initialize xkb_context", WL_MAIN_LOOP_FAIL);
				return;
			}

			if (not lglobals->compositor) {
				error = llkit::set_error(true, "failed to initialize compositor", WL_MAIN_LOOP_FAIL);
				return;
			}
		}

		std::optional<struct llkit_err> obj::main_loop(void) {
			if (ldisplay.get()->get_display() == nullptr)
				return llkit::set_error(
				    true, "failed to initialize main loop, display isn't initialized", WL_MAIN_LOOP_FAIL);

			while (wl_display_dispatch(ldisplay.get()->get_display())) {
			}

			return std::nullopt;
		}

		struct wl_display* obj::get_display(void) {
			return ldisplay.get()->get_display();
		}

		obj::~obj() {
		}
	}
}
