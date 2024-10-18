#ifndef LLKIT_GLOBALS_H
#define LLKIT_GLOBALS_H

#include <wayland-client.h>
#include <xkbcommon/xkbcommon.h>
#include "protocols/xdg-shell-client.h"
#include "core/display.h"
#include "core/seat/keyboard.h"
#include "core/seat/pointer.h"
#include "errors.h"

#define WL_GET_GLOBALS_FAIL -1

namespace llkit::seat::touch {
	struct obj;
}

namespace llkit {
	namespace globals {
		struct obj {
				struct wl_compositor*	    compositor	= nullptr;
				struct wl_shm*		    shm		= nullptr;
				struct xdg_wm_base*	    xdg_wm_base = nullptr;
				struct wl_seat*		    seat	= nullptr;
				struct wl_keyboard*	    keyboard	= nullptr;
				struct wl_pointer*	    pointer	= nullptr;
				struct wl_touch*	    touch	= nullptr;
				struct xkb_state*	    xkb_state	= nullptr;
				struct xkb_context*	    xkb_context = nullptr;
				struct xkb_keymap*	    xkb_keymap	= nullptr;
				llkit::seat::keyboard::obj* ll_keyboard = nullptr;
				llkit::seat::pointer::obj*  ll_pointer	= nullptr;
				llkit::seat::touch::obj*    ll_touch	= nullptr;
		};
	}
}

#endif // LLKIT_GLOBALS_H
