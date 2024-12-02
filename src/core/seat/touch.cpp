#include <optional>
#include <wayland-client.h>
#include "errors.h"
#include "core/globals.h"
#include "core/seat/touch.h"

namespace llkit {
	namespace seat {
		touch_t::touch_t() {
		}

		struct wl_touch* touch_t::get_wl_touch() {
			return wl_touch;
		}

		const struct wl_touch_listener& touch_t::get_wl_touch_listener() {
			return wl_touch_listener;
		}

		void touch_t::set_wl_touch(llkit::globals::obj* globals) {
			if (wl_touch != nullptr)
				wl_touch_release(wl_touch);
			std::shared_ptr<llkit::seat_t> ll_seat = globals->ll_seat;
			wl_touch			       = wl_seat_get_touch(ll_seat->get_wl_seat());
			wl_touch_add_listener(wl_touch, &wl_touch_listener, globals);
		}

		void touch_t::release_wl_touch() {
			if (wl_touch != nullptr)
				wl_touch_release(wl_touch);
			wl_touch = nullptr;
		}

		struct touch::touch_event& touch_t::get_touch_event() {
			return touch_event;
		}

		touch_t::~touch_t() {
			if (wl_touch != nullptr)
				wl_touch_destroy(wl_touch);
		}

		namespace touch {
			void down(void* data, struct wl_touch* wl_touch, uint32_t serial, uint32_t time, struct wl_surface* wl_surface,
			    int32_t id, wl_fixed_t x, wl_fixed_t y) {
				std::shared_ptr<llkit::seat_t> ll_seat	   = static_cast<llkit::globals::obj*>(data)->ll_seat;
				struct touch_point*	       touch_point = llkit::seat::touch::get_touch_point(ll_seat, id);
				if (touch_point == nullptr)
					return;

				touch_point->event_mask |= TOUCH_EVENT_DOWN;
				touch_point->surface_x		  = wl_fixed_to_double(x);
				touch_point->surface_y		  = wl_fixed_to_double(y);
				ll_seat->get_touch_event().time	  = time;
				ll_seat->get_touch_event().serial = serial;
			}

			void up(void* data, struct wl_touch* wl_touch, uint32_t serial, uint32_t time, int32_t id) {
				std::shared_ptr<llkit::seat_t> ll_seat	   = static_cast<llkit::globals::obj*>(data)->ll_seat;
				struct touch_point*	       touch_point = llkit::seat::touch::get_touch_point(ll_seat, id);
				if (touch_point == nullptr)
					return;

				touch_point->event_mask |= TOUCH_EVENT_UP;
				ll_seat->get_touch_event().time	  = time;
				ll_seat->get_touch_event().serial = serial;
			}

			void motion(void* data, struct wl_touch* wl_touch, uint32_t time, int32_t id, wl_fixed_t x, wl_fixed_t y) {
				std::shared_ptr<llkit::seat_t> ll_seat	   = static_cast<llkit::globals::obj*>(data)->ll_seat;
				struct touch_point*	       touch_point = llkit::seat::touch::get_touch_point(ll_seat, id);
				if (touch_point == nullptr)
					return;

				touch_point->event_mask |= TOUCH_EVENT_MOTION;
				touch_point->surface_x		= x;
				touch_point->surface_y		= y;
				ll_seat->get_touch_event().time = time;
			}

			void frame(void* data, struct wl_touch* wl_touch) {
				// struct llkit::globals::obj* globals	= static_cast<llkit::globals::obj*>(data);
				// struct touch_event* touch_event = &globals->ll_touch->touch_event;
				// const size_t	    points_num	= sizeof(touch_event->points) / sizeof(struct touch_point);

				// for(size_t i = 0; i < points_num; i++){
				// }
			}

			void cancel(void* data, struct wl_touch* wl_touch) {
				std::shared_ptr<llkit::seat_t> ll_seat = static_cast<llkit::globals::obj*>(data)->ll_seat;
				ll_seat->get_touch_event().event_mask |= TOUCH_EVENT_CANCEL;
			}

			void shape(void* data, struct wl_touch* wl_touch, int32_t id, wl_fixed_t major, wl_fixed_t minor) {
				std::shared_ptr<llkit::seat_t> ll_seat	   = static_cast<llkit::globals::obj*>(data)->ll_seat;
				struct touch_point*	       touch_point = llkit::seat::touch::get_touch_point(ll_seat, id);
				if (touch_point == nullptr)
					return;

				touch_point->event_mask |= TOUCH_EVENT_SHAPE;
				touch_point->major = major;
				touch_point->minor = minor;
			}

			void orientation(void* data, struct wl_touch* wl_touch, int32_t id, wl_fixed_t orientation) {
				std::shared_ptr<llkit::seat_t> ll_seat	   = static_cast<llkit::globals::obj*>(data)->ll_seat;
				struct touch_point*	       touch_point = llkit::seat::touch::get_touch_point(ll_seat, id);
				if (touch_point == nullptr)
					return;

				touch_point->event_mask |= TOUCH_EVENT_ORIENTATION;
				touch_point->orientation = orientation;
			}

			struct touch_point* get_touch_point(std::shared_ptr<llkit::seat_t> ll_seat, int32_t id) {
				struct touch_event* touch_event = &ll_seat->get_touch_event();
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
