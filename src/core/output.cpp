#include <wayland-client.h>
#include "core/globals.h"


namespace llkit {
	namespace output {
		void geometry(void* data, struct wl_output*, int32_t x, int32_t y, int32_t physical_width, int32_t physical_height,
		    int32_t subpixel, const char* make, const char* model, int32_t transform) {
			struct llkit::output::obj* ll_output = static_cast<llkit::output::obj*>(data);
			ll_output->subpixel		     = subpixel;

			if (ll_output->globals->output_running) {
				ll_output->redraw_frame = true;
			}
		}

		void mode(void* data, struct wl_output*, uint32_t flags, int32_t width, int32_t height, int32_t refresh) {
			struct llkit::output::obj* ll_output = static_cast<llkit::output::obj*>(data);
			ll_output->refresh_rate		     = refresh;
		}

		void done(void* data, struct wl_output*) {
			struct llkit::output::obj* ll_output = static_cast<llkit::output::obj*>(data);
		}

		void scale(void* data, struct wl_output*, int32_t factor) {
			struct llkit::output::obj* ll_output = static_cast<llkit::output::obj*>(data);
			ll_output->scale		     = factor;

			if (ll_output->globals->output_running) {
				ll_output->redraw_frame = true;
			}
		}

		void name(void* data, struct wl_output*, const char* name) {
			struct llkit::output::obj* ll_output = static_cast<llkit::output::obj*>(data);
			ll_output->name			     = name;
		}

		void description(void* data, struct wl_output*, const char* description) {
			struct llkit::output::obj* ll_output = static_cast<llkit::output::obj*>(data);
			ll_output->description		     = description;
		}

		// void release(void* data, struct wl_output*);
	}
}
