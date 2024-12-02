#include <optional>
#include <wayland-client.h>
#include "errors.h"
#include "core/globals.h"
#include "core/seat/keyboard.h"
#include "core/seat/pointer.h"
#include "core/seat/touch.h"

namespace llkit {
	seat_t::seat_t() {
	}

	struct wl_seat* seat_t::get_wl_seat(void) {
		return wl_seat;
	}

	const struct wl_seat_listener& seat_t::get_wl_seat_listener(void) {
		return wl_seat_listener;
	}

	void seat_t::set_wl_seat(struct wl_seat* wl_seat_) {
		if (wl_seat != nullptr)
			wl_seat_destroy(wl_seat);
		wl_seat = wl_seat_;
	}

	seat_t::~seat_t() {
		if (wl_seat != nullptr)
			wl_seat_destroy(wl_seat);
	}

	namespace seat {
		void capabilities(void* data, struct wl_seat* wl_seat, uint32_t capabilities) {
			struct llkit::globals::obj*    globals = static_cast<llkit::globals::obj*>(data);
			std::shared_ptr<llkit::seat_t> ll_seat = globals->ll_seat;

			bool have_pointer = capabilities & WL_SEAT_CAPABILITY_POINTER;
			if (have_pointer && ll_seat->get_wl_pointer() == nullptr) {
				ll_seat->set_wl_pointer(globals);
			} else if (not have_pointer && ll_seat->get_wl_pointer() != nullptr) {
				ll_seat->release_wl_pointer();
			}

			bool have_keyboard = capabilities & WL_SEAT_CAPABILITY_KEYBOARD;
			if (have_keyboard && ll_seat->get_wl_keyboard() == nullptr) {
				ll_seat->set_wl_keyboard(globals);
			} else if (not have_keyboard && ll_seat->get_wl_keyboard() != nullptr) {
				ll_seat->release_wl_keyboard();
			}

			bool have_touch = capabilities & WL_SEAT_CAPABILITY_TOUCH;
			if (have_touch && ll_seat->get_wl_touch() == nullptr) {
				ll_seat->set_wl_touch(globals);
			} else if (not have_touch && ll_seat->get_wl_touch() != nullptr) {
				ll_seat->release_wl_touch();
			}
		}

		void name(void* data, struct wl_seat* wl_seat, const char* name) {
		}
	}
}
