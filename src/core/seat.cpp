#include <optional>
#include <wayland-client.h>
#include "errors.h"
#include "core/globals.h"
#include "core/seat/keyboard.h"
#include "core/seat/pointer.h"
#include "core/seat/touch.h"

namespace llkit {
	namespace seat {
		void capabilities(void* data, struct wl_seat* wl_seat, uint32_t capabilities) {
			struct llkit::globals::obj* globals = static_cast<llkit::globals::obj*>(data);

			bool have_pointer = capabilities & WL_SEAT_CAPABILITY_POINTER;
			if (have_pointer && globals->pointer == nullptr) {
				globals->pointer = wl_seat_get_pointer(wl_seat);
				wl_pointer_add_listener(globals->pointer, &llkit::seat::pointer::wl_pointer_listener, globals);
			} else if (not have_pointer && globals->pointer != nullptr) {
				wl_pointer_release(globals->pointer);
				globals->pointer = nullptr;
			}

			bool have_keyboard = capabilities & WL_SEAT_CAPABILITY_KEYBOARD;
			if (have_keyboard && globals->seat == nullptr) {
				globals->keyboard = wl_seat_get_keyboard(wl_seat);
				wl_keyboard_add_listener(globals->keyboard, &llkit::seat::keyboard::wl_keyboard_listener, globals);
			} else if (not have_keyboard && globals->keyboard != nullptr) {
				wl_keyboard_release(globals->keyboard);
				globals->keyboard = nullptr;
			}

			bool have_touch = capabilities & WL_SEAT_CAPABILITY_TOUCH;
			if (have_touch && globals->seat == nullptr) {
				globals->touch = wl_seat_get_touch(wl_seat);
				wl_touch_add_listener(globals->touch, &llkit::seat::touch::wl_touch_listener, globals);
			} else if (not have_touch && globals->touch != nullptr) {
				wl_touch_release(globals->touch);
				globals->touch = nullptr;
			}
		}

		void name(void* data, struct wl_seat* wl_seat, const char* name) {
		}
	}
}
