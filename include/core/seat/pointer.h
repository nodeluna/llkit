#ifndef LLKIT_POINTER_H
#define LLKIT_POINTER_H

#include <optional>
#include <wayland-client.h>
#include "errors.h"
#include "core/globals.h"

namespace llkit {
	namespace seat {
		namespace pointer {
			void enter(void* data, struct wl_pointer* wl_pointer, uint32_t serial, struct wl_surface* wl_surface,
			    wl_fixed_t surface_x, wl_fixed_t surface_y);

			void leave(void* data, struct wl_pointer* wl_pointer, uint32_t serial, struct wl_surface* wl_surface);

			void motion(void* data, struct wl_pointer* wl_pointer, uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y);

			void button(
			    void* data, struct wl_pointer* wl_pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state);

			void axis(void* data, struct wl_pointer* wl_pointer, uint32_t time, uint32_t axis, wl_fixed_t value);

			void frame(void* data, struct wl_pointer* wl_pointer);

			void axis_source(void* data, struct wl_pointer* wl_pointer, uint32_t axis_source);

			void axis_stop(void* data, struct wl_pointer* wl_pointer, uint32_t time, uint32_t axis);

			void axis_discrete(void* data, struct wl_pointer* wl_pointer, uint32_t axis, int32_t discrete);

			void axis_value120(void* data, struct wl_pointer* wl_pointer, uint32_t axis, int32_t value120);

			void axis_relative_direction(void* data, struct wl_pointer* wl_pointer, uint32_t axis, uint32_t direction);

			enum pointer_event_mask {
				POINTER_EVENT_ENTER		      = 1 << 0,
				POINTER_EVENT_LEAVE		      = 1 << 1,
				POINTER_EVENT_MOTION		      = 1 << 2,
				POINTER_EVENT_BUTTON		      = 1 << 3,
				POINTER_EVENT_AXIS		      = 1 << 4,
				POINTER_EVENT_AXIS_SOURCE	      = 1 << 5,
				POINTER_EVENT_AXIS_STOP		      = 1 << 6,
				POINTER_EVENT_AXIS_DISCRETE	      = 1 << 7,
				POINTER_EVENT_AXIS_VALUE120	      = 1 << 8,
				POINTER_EVENT_AXIS_RELATIVE_DIRECTION = 1 << 9,
			};

			struct pointer_event {
					uint32_t   event_mask;
					wl_fixed_t surface_x, surface_y;
					uint32_t   button;
					uint32_t   state;
					uint32_t   time;
					uint32_t   serial;
					uint32_t   axis_source;

					struct {
							bool	   valid;
							wl_fixed_t value;
							int32_t	   discrete;
							int32_t	   value120;
							uint32_t   relative_direction;
					} axis[2];
			};
		}

		class pointer_t {
			protected:
				struct wl_pointer*	      wl_pointer = nullptr;
				struct pointer::pointer_event pointer_event;
				struct wl_pointer_listener    wl_pointer_listener = {
				       .enter			= llkit::seat::pointer::enter,
				       .leave			= llkit::seat::pointer::leave,
				       .motion			= llkit::seat::pointer::motion,
				       .button			= llkit::seat::pointer::button,
				       .axis			= llkit::seat::pointer::axis,
				       .frame			= llkit::seat::pointer::frame,
				       .axis_source		= llkit::seat::pointer::axis_source,
				       .axis_stop		= llkit::seat::pointer::axis_stop,
				       .axis_discrete		= llkit::seat::pointer::axis_discrete,
				       .axis_value120		= llkit::seat::pointer::axis_value120,
				       .axis_relative_direction = llkit::seat::pointer::axis_relative_direction,
				   };

			public:
				pointer_t();
				struct wl_pointer*		  get_wl_pointer();
				const struct wl_pointer_listener& get_wl_pointer_listener();
				struct pointer::pointer_event&	  get_pointer_event();
				void				  set_wl_pointer(llkit::globals::obj* globals);
				void				  release_wl_pointer(void);
				~pointer_t();
		};

	}
}

#endif // LLKIT_POINTER_H
