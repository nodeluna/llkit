#include <optional>
#include <cstring>
#include <wayland-client.h>
#include "errors.h"
#include "core/globals.h"
#include "core/seat/pointer.h"

namespace llkit {
	namespace seat {
		namespace pointer {
			void enter(void* data, struct wl_pointer* wl_pointer, uint32_t serial, struct wl_surface* wl_surface,
			    wl_fixed_t surface_x, wl_fixed_t surface_y) {
				struct llkit::globals::obj* globals = static_cast<llkit::globals::obj*>(data);
				globals->ll_pointer->pointer_event.event_mask |= POINTER_EVENT_ENTER;
				globals->ll_pointer->pointer_event.serial    = serial;
				globals->ll_pointer->pointer_event.surface_x = surface_x;
				globals->ll_pointer->pointer_event.surface_y = surface_y;
			}

			void leave(void* data, struct wl_pointer* wl_pointer, uint32_t serial, struct wl_surface* wl_surface) {
				struct llkit::globals::obj* globals = static_cast<llkit::globals::obj*>(data);
				globals->ll_pointer->pointer_event.event_mask |= POINTER_EVENT_LEAVE;
				globals->ll_pointer->pointer_event.serial = serial;
			}

			void motion(void* data, struct wl_pointer* wl_pointer, uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y) {
				struct llkit::globals::obj* globals = static_cast<llkit::globals::obj*>(data);
				globals->ll_pointer->pointer_event.event_mask |= POINTER_EVENT_MOTION;
				globals->ll_pointer->pointer_event.time	     = time;
				globals->ll_pointer->pointer_event.surface_x = surface_x;
				globals->ll_pointer->pointer_event.surface_y = surface_y;
			}

			void button(
			    void* data, struct wl_pointer* wl_pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state) {
				struct llkit::globals::obj* globals = static_cast<llkit::globals::obj*>(data);
				globals->ll_pointer->pointer_event.event_mask |= POINTER_EVENT_BUTTON;
				globals->ll_pointer->pointer_event.serial = serial;
				globals->ll_pointer->pointer_event.time	  = time;
				globals->ll_pointer->pointer_event.button = button;
				globals->ll_pointer->pointer_event.state  = state;
			}

			void axis(void* data, struct wl_pointer* wl_pointer, uint32_t time, uint32_t axis, wl_fixed_t value) {
				struct llkit::globals::obj* globals = static_cast<llkit::globals::obj*>(data);
				globals->ll_pointer->pointer_event.event_mask |= POINTER_EVENT_AXIS;
				globals->ll_pointer->pointer_event.time		    = time;
				globals->ll_pointer->pointer_event.axis[axis].valid = true;
				globals->ll_pointer->pointer_event.axis[axis].value = value;
			}

			void frame(void* data, struct wl_pointer* wl_pointer) {
				struct llkit::globals::obj* globals	  = static_cast<llkit::globals::obj*>(data);
				struct pointer_event*	    pointer_event = &globals->ll_pointer->pointer_event;

				// handling pointer

				memset(pointer_event, 0, sizeof(*pointer_event));
			}

			void axis_source(void* data, struct wl_pointer* wl_pointer, uint32_t axis_source) {
				struct llkit::globals::obj* globals = static_cast<llkit::globals::obj*>(data);
				globals->ll_pointer->pointer_event.event_mask |= POINTER_EVENT_AXIS_SOURCE;
				globals->ll_pointer->pointer_event.axis_source = axis_source;
			}

			void axis_stop(void* data, struct wl_pointer* wl_pointer, uint32_t time, uint32_t axis) {
				struct llkit::globals::obj* globals = static_cast<llkit::globals::obj*>(data);
				globals->ll_pointer->pointer_event.event_mask |= POINTER_EVENT_AXIS_STOP;
				globals->ll_pointer->pointer_event.time		    = time;
				globals->ll_pointer->pointer_event.axis[axis].valid = true;
			}

			void axis_discrete(void* data, struct wl_pointer* wl_pointer, uint32_t axis, int32_t discrete) {
				struct llkit::globals::obj* globals = static_cast<llkit::globals::obj*>(data);
				globals->ll_pointer->pointer_event.event_mask |= POINTER_EVENT_AXIS_DISCRETE;
				globals->ll_pointer->pointer_event.axis[axis].valid    = true;
				globals->ll_pointer->pointer_event.axis[axis].discrete = discrete;
			}

			void axis_value120(void* data, struct wl_pointer* wl_pointer, uint32_t axis, int32_t value120) {
				struct llkit::globals::obj* globals = static_cast<llkit::globals::obj*>(data);
				globals->ll_pointer->pointer_event.event_mask |= POINTER_EVENT_AXIS_VALUE120;
				globals->ll_pointer->pointer_event.axis[axis].valid    = true;
				globals->ll_pointer->pointer_event.axis[axis].value120 = value120;
			}

			void axis_relative_direction(void* data, struct wl_pointer* wl_pointer, uint32_t axis, uint32_t direction) {
				struct llkit::globals::obj* globals = static_cast<llkit::globals::obj*>(data);
				globals->ll_pointer->pointer_event.event_mask |= POINTER_EVENT_AXIS_RELATIVE_DIRECTION;
				globals->ll_pointer->pointer_event.axis[axis].valid		 = true;
				globals->ll_pointer->pointer_event.axis[axis].relative_direction = direction;
			}

		}
	}
}
