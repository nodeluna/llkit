#ifndef LLKIT_SEAT_H
#define LLKIT_SEAT_H

#include <optional>
#include <wayland-client.h>
#include "errors.h"
#include "core/globals.h"
#include "core/seat/keyboard.h"
#include "core/seat/pointer.h"
#include "core/seat/touch.h"

namespace llkit::globals {}

namespace llkit {
	namespace seat {
		void capabilities(void* data, struct wl_seat* wl_seat, uint32_t capabilities);

		void name(void* data, struct wl_seat* wl_seat, const char* name);
	}

	class seat_t : public llkit::seat::keyboard_t, public llkit::seat::pointer_t, public llkit::seat::touch_t {
		private:
			struct wl_seat*		wl_seat		 = nullptr;
			struct wl_seat_listener wl_seat_listener = {
			    .capabilities = llkit::seat::capabilities,
			    .name	  = llkit::seat::name,
			};

		public:
			seat_t(void);
			const struct wl_seat_listener& get_wl_seat_listener(void);
			struct wl_seat*		       get_wl_seat(void);
			void			       set_wl_seat(struct wl_seat* wl_seat);
			~seat_t();
			// std::optional<struct llkit_err> error = std::nullopt;
	};
}

#endif // LLKIT_SEAT_H
