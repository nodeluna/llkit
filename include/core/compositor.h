#ifndef LLKIT_COMPOSITOR_H
#define LLKIT_COMPOSITOR_H

#include <wayland-client.h>

namespace llkit {
	class compositor {
		private:
			struct wl_compositor*	 wl_compositor	  = nullptr;
			struct wl_subcompositor* wl_subcompositor = nullptr;

		public:
			compositor();
			struct wl_compositor*	 get_wl_compositor();
			struct wl_subcompositor* get_wl_subcompositor();
			bool			 set_wl_compositor(struct wl_compositor* wl_compositor);
			bool			 set_wl_subcompositor(struct wl_subcompositor* wl_subcompositor);
			~compositor();
	};
}

#endif // LLKIT_COMPOSITOR_H
