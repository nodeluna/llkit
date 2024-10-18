#include <optional>
#include <wayland-client.h>
#include "errors.h"
#include "core/globals.h"
#include "core/seat/touch.h"

namespace llkit {
	namespace seat {
		namespace touch {
			void down(void* data, struct wl_touch* wl_touch, uint32_t serial, uint32_t time, struct wl_surface* wl_surface,
			    int32_t id, wl_fixed_t x, wl_fixed_t y) {
				struct llkit::globals::obj* globals	= static_cast<llkit::globals::obj*>(data);
				struct touch_point*	    touch_point = llkit::seat::touch::get_touch_point(globals, id);
				if (touch_point == nullptr)
					return;

				touch_point->event_mask |= TOUCH_EVENT_DOWN;
				touch_point->surface_x		      = wl_fixed_to_double(x);
				touch_point->surface_y		      = wl_fixed_to_double(y);
				globals->ll_touch->touch_event.time   = time;
				globals->ll_touch->touch_event.serial = serial;
			}

			void up(void* data, struct wl_touch* wl_touch, uint32_t serial, uint32_t time, int32_t id) {
				struct llkit::globals::obj* globals	= static_cast<llkit::globals::obj*>(data);
				struct touch_point*	    touch_point = llkit::seat::touch::get_touch_point(globals, id);
				if (touch_point == nullptr)
					return;

				touch_point->event_mask |= TOUCH_EVENT_UP;
				globals->ll_touch->touch_event.time   = time;
				globals->ll_touch->touch_event.serial = serial;
			}

			void motion(void* data, struct wl_touch* wl_touch, uint32_t time, int32_t id, wl_fixed_t x, wl_fixed_t y) {
				struct llkit::globals::obj* globals	= static_cast<llkit::globals::obj*>(data);
				struct touch_point*	    touch_point = llkit::seat::touch::get_touch_point(globals, id);
				if (touch_point == nullptr)
					return;

				touch_point->event_mask |= TOUCH_EVENT_MOTION;
				touch_point->surface_x		    = x;
				touch_point->surface_y		    = y;
				globals->ll_touch->touch_event.time = time;
			}

			void frame(void* data, struct wl_touch* wl_touch) {
				// struct llkit::globals::obj* globals	= static_cast<llkit::globals::obj*>(data);
				// struct touch_event* touch_event = &globals->ll_touch->touch_event;
				// const size_t	    points_num	= sizeof(touch_event->points) / sizeof(struct touch_point);

				// for(size_t i = 0; i < points_num; i++){
				// }
			}

			void cancel(void* data, struct wl_touch* wl_touch) {
				struct llkit::globals::obj* globals = static_cast<llkit::globals::obj*>(data);
				globals->ll_touch->touch_event.event_mask |= TOUCH_EVENT_CANCEL;
			}

			void shape(void* data, struct wl_touch* wl_touch, int32_t id, wl_fixed_t major, wl_fixed_t minor) {
				struct llkit::globals::obj* globals	= static_cast<llkit::globals::obj*>(data);
				struct touch_point*	    touch_point = llkit::seat::touch::get_touch_point(globals, id);
				if (touch_point == nullptr)
					return;

				touch_point->event_mask |= TOUCH_EVENT_SHAPE;
				touch_point->major = major;
				touch_point->minor = minor;
			}

			void orientation(void* data, struct wl_touch* wl_touch, int32_t id, wl_fixed_t orientation) {
				struct llkit::globals::obj* globals	= static_cast<llkit::globals::obj*>(data);
				struct touch_point*	    touch_point = llkit::seat::touch::get_touch_point(globals, id);
				if (touch_point == nullptr)
					return;

				touch_point->event_mask |= TOUCH_EVENT_ORIENTATION;
				touch_point->orientation = orientation;
			}

			struct touch_point* get_touch_point(llkit::globals::obj* globals, int32_t id) {
				struct touch_event* touch_event = &globals->ll_touch->touch_event;
				const size_t	    points_num	= sizeof(touch_event->points) / sizeof(struct touch_point);
				int		    valid	= -1;

				for (size_t i = 0; i < points_num; i++) {
					if (touch_event->points[i].id == id)
						return &touch_event->points[i];
					if (valid == -1 && not touch_event->points[i].valid)
						valid = i;
				}

				if (valid == -1)
					return nullptr;

				touch_event->points[valid].valid = true;
				touch_event->points[valid].id	 = valid;
				return &touch_event->points[valid];
			}

		}
	}
}
