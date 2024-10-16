#include <cstring>
#include <wayland-client.h>
#include "core/registry.h"
#include "core/display.h"
#include "core/globals.h"
#include "errors.h"

namespace llkit {
	namespace registry {
		void global(void* data, struct wl_registry* registry, uint32_t name, const char* interface, uint32_t version) {
			struct llkit::globals::obj* globals = static_cast<llkit::globals::obj*>(data);

			if (strcmp(interface, wl_compositor_interface.name) == 0) {
				globals->compositor =
				    static_cast<wl_compositor*>(wl_registry_bind(registry, name, &wl_compositor_interface, version));
			} else if (strcmp(interface, wl_shm_interface.name) == 0) {
				globals->shm = static_cast<wl_shm*>(wl_registry_bind(registry, name, &wl_shm_interface, version));
			}
		}

		void global_remove(void* data, struct wl_registry* registry, uint32_t name) {
			struct llkit::globals::obj* globals = static_cast<llkit::globals::obj*>(data);
			// wl_shm_destroy(globals->shm);
		}

		obj::obj(std::shared_ptr<llkit::display::obj> display, std::shared_ptr<llkit::globals::obj> globals) {
			registry = wl_display_get_registry(display.get()->get_display());
			if (registry == NULL) {
				error = llkit::set_error(true, "couldn't get registry", WL_GET_REGISTRY_FAIL);
				return;
			}

			wl_registry_add_listener(registry, &registry_listener, globals.get());
			wl_display_roundtrip(display.get()->get_display());
		}

		struct wl_registry* obj::get_registry(void) {
			return registry;
		}

		obj::~obj() {
			if (registry != nullptr)
				wl_registry_destroy(registry);
		}
	}
}
