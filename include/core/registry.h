#ifndef LLKIT_REGISTRY_H
#define LLKIT_REGISTRY_H

#include <memory>
#include <wayland-client.h>
#include "core/display.h"
#include "core/globals.h"
#include "errors.h"

#define WL_GET_REGISTRY_FAIL -1

namespace llkit {
	namespace registry {
		void global(void* data, struct wl_registry* registry, uint32_t name, const char* interface, uint32_t version);

		void global_remove(void* data, struct wl_registry* registry, uint32_t name);

		class obj {
			public:
				obj(std::shared_ptr<llkit::display::obj> display, std::shared_ptr<llkit::globals::obj> globals);
				struct wl_registry* get_registry(void);
				~obj();

				std::optional<struct llkit_err> error = std::nullopt;

			private:
				struct wl_registry*	    registry	      = nullptr;
				struct wl_registry_listener registry_listener = {
				    .global	   = llkit::registry::global,
				    .global_remove = llkit::registry::global_remove,
				};
		};

	}
}

#endif // LLKIT_REGISTRY_H
