#include <cstring>
#include <wayland-client-protocol.h>
#include <wayland-client.h>
#include "core/registry.h"
#include "core/display.h"
#include "core/globals.h"
#include "core/output.h"
#include "core/seat.h"
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
			} else if (strcmp(interface, wl_seat_interface.name) == 0) {
				globals->seat = static_cast<wl_seat*>(wl_registry_bind(registry, name, &wl_seat_interface, version));
				wl_seat_add_listener(globals->seat, &llkit::seat::wl_seat_listener, globals);
			} else if (strcmp(interface, wl_output_interface.name) == 0) {
				struct llkit::output::obj* ll_output = new llkit::output::obj;
				ll_output->output =
				    static_cast<wl_output*>(wl_registry_bind(registry, name, &wl_output_interface, version));
				ll_output->global_name = name;
				ll_output->globals     = globals;
				globals->ll_outputs.push_back(ll_output);
				wl_output_add_listener(ll_output->output, &llkit::output::wl_output_listener, ll_output);
			}
		}

		void global_remove(void* data, struct wl_registry* registry, uint32_t name) {
			struct llkit::globals::obj* globals = static_cast<llkit::globals::obj*>(data);
			for (auto itr = globals->ll_outputs.begin(); itr != globals->ll_outputs.end();) {
				if ((*itr)->global_name == name) {
					wl_output_release((*itr)->output);
					delete[] (*itr);
					itr = globals->ll_outputs.erase(itr);
				} else
					++itr;
			}
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
