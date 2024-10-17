#include <optional>
#include <array>
#include <print>
#include <unistd.h>
#include <sys/mman.h>
#include <wayland-client.h>
#include <wayland-util.h>
#include <xkbcommon/xkbcommon.h>
#include "errors.h"
#include "core/globals.h"
#include "core/seat/keyboard.h"
#include "core/utils.h"

namespace llkit {
	namespace seat {
		namespace keyboard {
			void keymap(void* data, struct wl_keyboard* wl_keyboard, uint32_t format, int32_t fd, uint32_t size) {
				struct llkit::globals::obj* globals = static_cast<llkit::globals::obj*>(data);

				char* map_shm = static_cast<char*>(mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0));
				if (map_shm == MAP_FAILED)
					return;

				struct xkb_keymap* xkb_keymap = xkb_keymap_new_from_string(
				    globals->xkb_context, map_shm, XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
				munmap(map_shm, size);
				close(fd);

				struct xkb_state* xkb_state = xkb_state_new(xkb_keymap);
				if (not globals->xkb_keymap)
					xkb_keymap_unref(globals->xkb_keymap);
				if (not globals->xkb_state)
					xkb_state_unref(globals->xkb_state);

				globals->xkb_keymap = xkb_keymap;
				globals->xkb_state  = xkb_state;
			}

			void enter(void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, struct wl_surface* wl_surface,
			    struct wl_array* keys) {
				struct llkit::globals::obj* globals = static_cast<llkit::globals::obj*>(data);

				auto func = [&](uint32_t* key) {
					std::array<char, 124> buffer;
					xkb_keysym_t	      sym = xkb_state_key_get_one_sym(globals->xkb_state, *key + 8);
					xkb_keysym_get_name(sym, buffer.data(), buffer.size());
					// buffer has sym key
					xkb_state_key_get_utf8(globals->xkb_state, *key + 8, buffer.data(), buffer.size());
					// buffer has utf8 key
				};

				wl_array_loop<uint32_t*>(keys, func);
			}

			void leave(void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, struct wl_surface* wl_surface);

			void key(
			    void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state) {
				struct llkit::globals::obj* globals = static_cast<llkit::globals::obj*>(data);
				uint32_t		    keycode = key + 8;
				xkb_keysym_t		    keysym  = xkb_state_key_get_one_sym(globals->xkb_state, keycode);

				if (keysym == XKB_KEY_Return && state == WL_KEYBOARD_KEY_STATE_PRESSED) {
					std::println("enter is pressed");
				}

				if (auto itr = globals->ll_keyboard->keybind.find(keysym); itr != globals->ll_keyboard->keybind.end()) {
					if (itr->second.state == state) {
						itr->second.func(serial, time);
					}
				}
			}

			void modifiers(void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, uint32_t mods_depressed,
			    uint32_t mods_latched, uint32_t mods_locked, uint32_t group) {
				struct llkit::globals::obj* globals = static_cast<llkit::globals::obj*>(data);
				xkb_state_update_mask(globals->xkb_state, mods_depressed, mods_latched, mods_locked, 0, 0, group);
			}

			void repeat_info(void* data, struct wl_keyboard* wl_keyboard, int32_t rate, int32_t delay);

		}
	}
}
