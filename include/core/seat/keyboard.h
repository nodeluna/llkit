#ifndef LLKIT_KEYBOARD_H
#define LLKIT_KEYBOARD_H

#include <map>
#include <functional>
#include <optional>
#include <utility>
#include <any>
#include <wayland-client.h>
#include <wayland-util.h>
#include <xkbcommon/xkbcommon.h>
#include "errors.h"
#include "core/globals.h"

namespace llkit::globals {
	struct obj;
}

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

			struct key_state_func {
					uint32_t					    state;
					std::function<void(uint32_t serial, uint32_t time)> func;
			};
		}

		class keyboard_t {
			protected:
				struct wl_keyboard*	    wl_keyboard		 = nullptr;
				struct xkb_context*	    xkb_context		 = nullptr;
				struct xkb_state*	    xkb_state		 = nullptr;
				struct xkb_keymap*	    xkb_keymap		 = nullptr;
				struct wl_keyboard_listener wl_keyboard_listener = {
				    .keymap	 = llkit::seat::keyboard::keymap,
				    .enter	 = llkit::seat::keyboard::enter,
				    .leave	 = llkit::seat::keyboard::leave,
				    .key	 = llkit::seat::keyboard::key,
				    .modifiers	 = llkit::seat::keyboard::modifiers,
				    .repeat_info = llkit::seat::keyboard::repeat_info,
				};

			public:
				keyboard_t();
				void				   set_wl_keyboard(llkit::globals::obj* globals);
				void				   release_wl_keyboard();
				struct wl_keyboard*		   get_wl_keyboard();
				const struct wl_keyboard_listener& get_wl_keyboard_listener();
				struct xkb_context*		   get_xkb_context();
				struct xkb_state*		   get_xkb_state();
				struct xkb_keymap*		   get_xkb_keymap();
				void				   set_xkb_context(struct xkb_context*);
				void				   set_xkb_state(struct xkb_state*);
				void				   set_xkb_keymap(struct xkb_keymap*);
				// std::unordered_multimap<xkb_keysym_t, key_state_func> keybind;
				~keyboard_t();
		};
	}
}

#endif // LLKIT_KEYBOARD_H
