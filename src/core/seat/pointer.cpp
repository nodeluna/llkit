#include <optional>
#include <memory>
#include <cstring>
#include <wayland-client.h>
#include "errors.h"
#include "core/globals.h"
#include "core/seat/pointer.h"

namespace llkit {
	namespace seat {
		pointer_t::pointer_t() {
		}

		struct wl_pointer* pointer_t::get_wl_pointer() {
			return wl_pointer;
		}

		const struct wl_pointer_listener& pointer_t::get_wl_pointer_listener() {
			return wl_pointer_listener;
		}

		struct pointer::pointer_event& pointer_t::get_pointer_event() {
			return pointer_event;
		}

		void pointer_t::set_wl_pointer(llkit::globals::obj* globals) {
			if (wl_pointer != nullptr)
				wl_pointer_release(wl_pointer);
			std::shared_ptr<llkit::seat_t> ll_seat = globals->ll_seat;
			wl_pointer			       = wl_seat_get_pointer(ll_seat->get_wl_seat());
			wl_pointer_add_listener(wl_pointer, &wl_pointer_listener, globals);
		}

		void pointer_t::release_wl_pointer() {
			if (wl_pointer != nullptr)
				wl_pointer_release(wl_pointer);
			wl_pointer = nullptr;
		}

		pointer_t::~pointer_t() {
			if (wl_pointer != nullptr)
				wl_pointer_destroy(wl_pointer);
		}

		namespace pointer {
			void enter(void* data, struct wl_pointer* wl_pointer, uint32_t serial, struct wl_surface* wl_surface,
			    wl_fixed_t surface_x, wl_fixed_t surface_y) {
				std::shared_ptr<llkit::seat_t> ll_seat = static_cast<llkit::globals::obj*>(data)->ll_seat;
				ll_seat->get_pointer_event().event_mask |= POINTER_EVENT_ENTER;
				ll_seat->get_pointer_event().serial    = serial;
				ll_seat->get_pointer_event().surface_x = surface_x;
				ll_seat->get_pointer_event().surface_y = surface_y;
			}

			void leave(void* data, struct wl_pointer* wl_pointer, uint32_t serial, struct wl_surface* wl_surface) {
				std::shared_ptr<llkit::seat_t> ll_seat = static_cast<llkit::globals::obj*>(data)->ll_seat;
				ll_seat->get_pointer_event().event_mask |= POINTER_EVENT_LEAVE;
				ll_seat->get_pointer_event().serial = serial;
			}

			void motion(void* data, struct wl_pointer* wl_pointer, uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y) {
				std::shared_ptr<llkit::seat_t> ll_seat = static_cast<llkit::globals::obj*>(data)->ll_seat;
				ll_seat->get_pointer_event().event_mask |= POINTER_EVENT_MOTION;
				ll_seat->get_pointer_event().time      = time;
				ll_seat->get_pointer_event().surface_x = surface_x;
				ll_seat->get_pointer_event().surface_y = surface_y;
			}

			void button(
			    void* data, struct wl_pointer* wl_pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state) {
				std::shared_ptr<llkit::seat_t> ll_seat = static_cast<llkit::globals::obj*>(data)->ll_seat;
				ll_seat->get_pointer_event().event_mask |= POINTER_EVENT_BUTTON;
				ll_seat->get_pointer_event().serial = serial;
				ll_seat->get_pointer_event().time   = time;
				ll_seat->get_pointer_event().button = button;
				ll_seat->get_pointer_event().state  = state;
			}

			void axis(void* data, struct wl_pointer* wl_pointer, uint32_t time, uint32_t axis, wl_fixed_t value) {
				std::shared_ptr<llkit::seat_t> ll_seat = static_cast<llkit::globals::obj*>(data)->ll_seat;
				ll_seat->get_pointer_event().event_mask |= POINTER_EVENT_AXIS;
				ll_seat->get_pointer_event().time	      = time;
				ll_seat->get_pointer_event().axis[axis].valid = true;
				ll_seat->get_pointer_event().axis[axis].value = value;
			}

			void frame(void* data, struct wl_pointer* wl_pointer) {
				std::shared_ptr<llkit::seat_t> ll_seat	     = static_cast<llkit::globals::obj*>(data)->ll_seat;
				struct pointer_event*	       pointer_event = &ll_seat->get_pointer_event();

				// handling pointer

				memset(pointer_event, 0, sizeof(*pointer_event));
			}

			void axis_source(void* data, struct wl_pointer* wl_pointer, uint32_t axis_source) {
				std::shared_ptr<llkit::seat_t> ll_seat = static_cast<llkit::globals::obj*>(data)->ll_seat;
				ll_seat->get_pointer_event().event_mask |= POINTER_EVENT_AXIS_SOURCE;
				ll_seat->get_pointer_event().axis_source = axis_source;
			}

			void axis_stop(void* data, struct wl_pointer* wl_pointer, uint32_t time, uint32_t axis) {
				std::shared_ptr<llkit::seat_t> ll_seat = static_cast<llkit::globals::obj*>(data)->ll_seat;
				ll_seat->get_pointer_event().event_mask |= POINTER_EVENT_AXIS_STOP;
				ll_seat->get_pointer_event().time	      = time;
				ll_seat->get_pointer_event().axis[axis].valid = true;
			}

			void axis_discrete(void* data, struct wl_pointer* wl_pointer, uint32_t axis, int32_t discrete) {
				std::shared_ptr<llkit::seat_t> ll_seat = static_cast<llkit::globals::obj*>(data)->ll_seat;
				ll_seat->get_pointer_event().event_mask |= POINTER_EVENT_AXIS_DISCRETE;
				ll_seat->get_pointer_event().axis[axis].valid	 = true;
				ll_seat->get_pointer_event().axis[axis].discrete = discrete;
			}

			void axis_value120(void* data, struct wl_pointer* wl_pointer, uint32_t axis, int32_t value120) {
				std::shared_ptr<llkit::seat_t> ll_seat = static_cast<llkit::globals::obj*>(data)->ll_seat;
				ll_seat->get_pointer_event().event_mask |= POINTER_EVENT_AXIS_VALUE120;
				ll_seat->get_pointer_event().axis[axis].valid	 = true;
				ll_seat->get_pointer_event().axis[axis].value120 = value120;
			}

			void axis_relative_direction(void* data, struct wl_pointer* wl_pointer, uint32_t axis, uint32_t direction) {
				std::shared_ptr<llkit::seat_t> ll_seat = static_cast<llkit::globals::obj*>(data)->ll_seat;
				ll_seat->get_pointer_event().event_mask |= POINTER_EVENT_AXIS_RELATIVE_DIRECTION;
				ll_seat->get_pointer_event().axis[axis].valid		   = true;
				ll_seat->get_pointer_event().axis[axis].relative_direction = direction;
			}

		}
	}
}
