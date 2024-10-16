#ifndef LLKIT_KEYBOARD_H
#define LLKIT_KEYBOARD_H

#include <optional>
#include <wayland-client.h>
#include <wayland-util.h>
#include "errors.h"
#include "core/globals.h"

namespace llkit {
	namespace seat {
		namespace keyboard {
			void keymap(void* data, struct wl_keyboard* wl_keyboard, uint32_t format, int32_t fd, uint32_t size);

			void enter(void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, struct wl_surface* wl_surface,
			    struct wl_array* keys);

			void leave(void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, struct wl_surface* wl_surface);

			void key(void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state);

			void modifiers(void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, uint32_t mods_depressed,
			    uint32_t mods_latched, uint32_t mods_locked, uint32_t group);

			void repeat_info(void* data, struct wl_keyboard* wl_keyboard, int32_t rate, int32_t delay);

			inline struct wl_keyboard_listener wl_keyboard_listener = {
			    .keymap	 = llkit::seat::keyboard::keymap,
			    .enter	 = llkit::seat::keyboard::enter,
			    .leave	 = llkit::seat::keyboard::leave,
			    .key	 = llkit::seat::keyboard::key,
			    .modifiers	 = llkit::seat::keyboard::modifiers,
			    .repeat_info = llkit::seat::keyboard::repeat_info,
			};

			class obj {
				public:
					obj();
					~obj();
					std::optional<struct llkit_err> error = std::nullopt;

				private:
					llkit::globals::obj* globals = nullptr;
			};
		}
	}
}

#endif // LLKIT_KEYBOARD_H
