#include <memory>
#include <optional>
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
		}

		struct wl_display* obj::get_display(void) {
			return ldisplay.get()->get_display();
		}

		obj::~obj() {
		}
	}
}
