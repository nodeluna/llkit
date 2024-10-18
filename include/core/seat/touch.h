#ifndef LLKIT_TOUCH_H
#define LLKIT_TOUCH_H

#include <optional>
#include <wayland-client.h>
#include "errors.h"
#include "core/globals.h"

namespace llkit::globals {
	struct obj;
}

namespace llkit {
	namespace seat {
		namespace touch {
			void down(void* data, struct wl_touch* wl_touch, uint32_t serial, uint32_t time, struct wl_surface* wl_surface,
			    int32_t id, wl_fixed_t x, wl_fixed_t y);

			void up(void* data, struct wl_touch* wl_touch, uint32_t serial, uint32_t time, int32_t id);

			void motion(void* data, struct wl_touch* wl_touch, uint32_t time, int32_t id, wl_fixed_t x, wl_fixed_t y);

			void frame(void* data, struct wl_touch* wl_touch);

			void cancel(void* data, struct wl_touch* wl_touch);

			void shape(void* data, struct wl_touch* wl_touch, int32_t id, wl_fixed_t major, wl_fixed_t minor);

			void orientation(void* data, struct wl_touch* wl_touch, int32_t id, wl_fixed_t orientation);

			inline struct wl_touch_listener wl_touch_listener = {
			    .down	 = llkit::seat::touch::down,
			    .up		 = llkit::seat::touch::up,
			    .motion	 = llkit::seat::touch::motion,
			    .frame	 = llkit::seat::touch::frame,
			    .cancel	 = llkit::seat::touch::cancel,
			    .shape	 = llkit::seat::touch::shape,
			    .orientation = llkit::seat::touch::orientation,
			};

			enum touch_event_mask {
				TOUCH_EVENT_DOWN	= 1 << 0,
				TOUCH_EVENT_UP		= 1 << 1,
				TOUCH_EVENT_MOTION	= 1 << 2,
				TOUCH_EVENT_CANCEL	= 1 << 3,
				TOUCH_EVENT_SHAPE	= 1 << 4,
				TOUCH_EVENT_ORIENTATION = 1 << 5,
			};

			struct touch_point {
					bool	   valid;
					int32_t	   id;
					uint32_t   event_mask;
					wl_fixed_t surface_x, surface_y;
					wl_fixed_t major, minor;
					wl_fixed_t orientation;
			};

			struct touch_event {
					uint32_t	   event_mask;
					uint32_t	   time;
					uint32_t	   serial;
					struct touch_point points[10];
			};

			class obj {
				public:
					obj();
					~obj();
					std::optional<struct llkit_err> error = std::nullopt;
					struct touch_event		touch_event;

				private:
					// llkit::globals::obj* globals = nullptr;
			};

			struct touch_point* get_touch_point(llkit::globals::obj* globs, int32_t id);
		}
	}
}

#endif // LLKIT_TOUCH_H
