#ifndef LLKIT_SEAT_H
#define LLKIT_SEAT_H

#include <optional>
#include <wayland-client.h>
#include "errors.h"
#include "core/globals.h"
#include "core/seat/keyboard.h"
#include "core/seat/pointer.h"
#include "core/seat/touch.h"

namespace llkit {
	namespace seat {
		void capabilities(void* data, struct wl_seat* wl_seat, uint32_t capabilities);

		void name(void* data, struct wl_seat* wl_seat, const char* name);

		inline struct wl_seat_listener wl_seat_listener = {
		    .capabilities = llkit::seat::capabilities,
		    .name	  = llkit::seat::name,
		};

		class obj {
			public:
				obj();
				~obj();
				std::optional<struct llkit_err> error = std::nullopt;

			private:
				llkit::globals::obj* globals = nullptr;
		};
	}
}

#endif // LLKIT_SEAT_H
