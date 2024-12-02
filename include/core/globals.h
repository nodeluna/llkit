#ifndef LLKIT_GLOBALS_H
#define LLKIT_GLOBALS_H

#include <list>
#include <memory>
#include <wayland-client.h>
#include "core/display.h"
#include "core/seat/keyboard.h"
#include "core/seat/pointer.h"
#include "core/output.h"
#include "core/egl.h"
#include "core/seat.h"
#include "errors.h"

#define WL_GET_GLOBALS_FAIL -1

namespace llkit::seat::touch {
	struct obj;
}

namespace llkit {
	class egl;
}

namespace llkit {
	namespace globals {
		struct obj {
				struct wl_compositor*	       compositor     = nullptr;
				struct wl_display*	       wl_display     = nullptr;
				struct wl_shm*		       shm	      = nullptr;
				struct wl_output*	       output	      = nullptr;
				bool			       output_running = false;
				std::shared_ptr<llkit::seat_t> ll_seat;
				std::shared_ptr<llkit::egl>    ll_egl;
				std::list<llkit::output::obj*> ll_outputs;
		};
	}
}

#endif // LLKIT_GLOBALS_H
