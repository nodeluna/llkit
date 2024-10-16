#ifndef LLKIT_GLOBALS_H
#define LLKIT_GLOBALS_H

#include <wayland-client.h>
#include "protocols/xdg-shell-client.h"
#include "core/display.h"
#include "errors.h"

#define WL_GET_GLOBALS_FAIL -1

namespace llkit {
	namespace globals {
		struct obj {
				struct wl_compositor* compositor  = nullptr;
				struct wl_shm*	      shm	  = nullptr;
				struct xdg_wm_base*   xdg_wm_base = nullptr;
				struct wl_seat*	      seat	  = nullptr;
				struct wl_keyboard*   keyboard	  = nullptr;
				struct wl_pointer*    pointer	  = nullptr;
				struct wl_touch*      touch	  = nullptr;
		};
	}
}

#endif // LLKIT_GLOBALS_H
