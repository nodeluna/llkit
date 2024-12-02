#include <wayland-client.h>
#include "core/compositor.h"

namespace llkit {
	compositor::compositor() {
	}

	struct wl_compositor* compositor::get_wl_compositor() {
		return wl_compositor;
	}

	struct wl_subcompositor* compositor::get_wl_subcompositor() {
		return wl_subcompositor;
	}

	bool compositor::set_wl_compositor(struct wl_compositor* wl_compositor_) {
		if (wl_compositor != nullptr)
			return false;
		wl_compositor = wl_compositor_;
		return true;
	}

	bool compositor::set_wl_subcompositor(struct wl_subcompositor* wl_subcompositor_) {
		if (wl_subcompositor != nullptr)
			return false;
		wl_subcompositor = wl_subcompositor_;
		return true;
	}

	compositor::~compositor() {
		if (wl_compositor != nullptr)
			wl_compositor_destroy(wl_compositor);
		if (wl_subcompositor != nullptr)
			wl_subcompositor_destroy(wl_subcompositor);
	}
}
