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
		keyboard_t::keyboard_t() {
			xkb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
			if (not xkb_context) {
				/// error = llkit::set_error(true, "failed to initialize xkb_context", WL_MAIN_LOOP_FAIL);
				/// return;
				llkit::log_error("xkb_context wasn't set");
			}
		}

		struct wl_keyboard* keyboard_t::get_wl_keyboard() {
			return wl_keyboard;
		}

		const struct wl_keyboard_listener& keyboard_t::get_wl_keyboard_listener() {
			return wl_keyboard_listener;
		}

		struct xkb_context* keyboard_t::get_xkb_context() {
			return xkb_context;
		}

		struct xkb_state* keyboard_t::get_xkb_state() {
			return xkb_state;
		}

		struct xkb_keymap* keyboard_t::get_xkb_keymap() {
			return xkb_keymap;
		}

		void keyboard_t::set_xkb_context(struct xkb_context* xkb_context_) {
			if (xkb_context != nullptr)
				xkb_context_unref(xkb_context);
			xkb_context = xkb_context_;
		}

		void keyboard_t::set_xkb_state(struct xkb_state* xkb_state_) {
			if (xkb_state != nullptr)
				xkb_state_unref(xkb_state);
			xkb_state = xkb_state_;
		}

		void keyboard_t::set_xkb_keymap(struct xkb_keymap* xkb_keymap_) {
			if (xkb_keymap != nullptr)
				xkb_keymap_unref(xkb_keymap);
			xkb_keymap = xkb_keymap_;
		}

		void keyboard_t::set_wl_keyboard(llkit::globals::obj* globals) {
			if (wl_keyboard != nullptr)
				wl_keyboard_release(wl_keyboard);
			std::shared_ptr<llkit::seat_t> ll_seat = globals->ll_seat;
			wl_keyboard			       = wl_seat_get_keyboard(ll_seat->get_wl_seat());
			wl_keyboard_add_listener(wl_keyboard, &wl_keyboard_listener, globals);
		}

		void keyboard_t::release_wl_keyboard() {
			if (wl_keyboard != nullptr)
				wl_keyboard_release(wl_keyboard);
			wl_keyboard = nullptr;
		}

		keyboard_t::~keyboard_t() {
			if (wl_keyboard != nullptr) {
				wl_keyboard_destroy(wl_keyboard);
				wl_keyboard = nullptr;
			}
			if (xkb_context != nullptr) {
				xkb_context_unref(xkb_context);
				xkb_context = nullptr;
			}
			if (xkb_state != nullptr) {
				xkb_state_unref(xkb_state);
				xkb_state = nullptr;
			}
			if (xkb_keymap != nullptr) {
				xkb_keymap_unref(xkb_keymap);
				xkb_keymap = nullptr;
			}
		}

		namespace keyboard {
			void keymap(void* data, struct wl_keyboard* wl_keyboard, uint32_t format, int32_t fd, uint32_t size) {
				std::shared_ptr<llkit::seat_t> ll_seat = static_cast<llkit::globals::obj*>(data)->ll_seat;

				char* map_shm = static_cast<char*>(mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0));
				if (map_shm == MAP_FAILED)
					return;

				struct xkb_keymap* xkb_keymap = xkb_keymap_new_from_string(
				    ll_seat->get_xkb_context(), map_shm, XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
				munmap(map_shm, size);
				close(fd);
				map_shm = nullptr;

				struct xkb_state* xkb_state = xkb_state_new(xkb_keymap);
				ll_seat->set_xkb_keymap(xkb_keymap);
				ll_seat->set_xkb_state(xkb_state);
			}

			void enter(void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, struct wl_surface* wl_surface,
			    struct wl_array* keys) {
				std::shared_ptr<llkit::seat_t> ll_seat = static_cast<llkit::globals::obj*>(data)->ll_seat;

				auto func = [&](const uint32_t* key) {
					std::array<char, 124> buffer;
					xkb_keysym_t	      sym = xkb_state_key_get_one_sym(ll_seat->get_xkb_state(), *key + 8);
					xkb_keysym_get_name(sym, buffer.data(), buffer.size());
					// buffer has sym key
					xkb_state_key_get_utf8(ll_seat->get_xkb_state(), *key + 8, buffer.data(), buffer.size());
					// buffer has utf8 key
				};

				llkit::wl_array_loop<const uint32_t*>(keys, func);
			}

			void leave(void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, struct wl_surface* wl_surface) {
			}

			void key(
			    void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state) {
				std::shared_ptr<llkit::seat_t> ll_seat = static_cast<llkit::globals::obj*>(data)->ll_seat;
				uint32_t		       keycode = key + 8;
				xkb_keysym_t		       keysym  = xkb_state_key_get_one_sym(ll_seat->get_xkb_state(), keycode);

				if (keysym == XKB_KEY_Return && state == WL_KEYBOARD_KEY_STATE_PRESSED) {
					std::println("enter is pressed");
				}

				// if (auto itr = globals->ll_keyboard->keybind.find(keysym); itr != globals->ll_keyboard->keybind.end()) {
				// 	if (itr->second.state == state) {
				// 		itr->second.func(serial, time);
				// 	}
				// }
			}

			void modifiers(void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, uint32_t mods_depressed,
			    uint32_t mods_latched, uint32_t mods_locked, uint32_t group) {
				std::shared_ptr<llkit::seat_t> ll_seat = static_cast<llkit::globals::obj*>(data)->ll_seat;
				xkb_state_update_mask(ll_seat->get_xkb_state(), mods_depressed, mods_latched, mods_locked, 0, 0, group);
			}

			void repeat_info(void* data, struct wl_keyboard* wl_keyboard, int32_t rate, int32_t delay) {
			}

		}
	}
}
