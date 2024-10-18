#ifndef LLKIT_OUTPUT_H
#define LLKIT_OUTPUT_H

#include <wayland-client.h>
#include "core/globals.h"

namespace llkit::globals {
	struct obj;
}

namespace llkit {
	namespace output {
		void geometry(void* data, struct wl_output*, int32_t x, int32_t y, int32_t physical_width, int32_t physical_height,
		    int32_t subpixel, const char* make, const char* model, int32_t transform);

		void mode(void* data, struct wl_output*, uint32_t flags, int32_t width, int32_t height, int32_t refresh);

		void done(void* data, struct wl_output*);

		void scale(void* data, struct wl_output*, int32_t factor);

		void name(void* data, struct wl_output*, const char* name);

		void description(void* data, struct wl_output*, const char* description);

		// void release(void* data, struct wl_output*);

		inline struct wl_output_listener wl_output_listener = {
		    .geometry	 = llkit::output::geometry,
		    .mode	 = llkit::output::mode,
		    .done	 = llkit::output::done,
		    .scale	 = llkit::output::scale,
		    .name	 = llkit::output::name,
		    .description = llkit::output::description,
		    //.release	 = llkit::output::release,
		};

		struct obj {
				bool			    redraw_frame   = false;
				bool			    output_running = false;
				int32_t			    subpixel;
				int32_t			    scale;
				int32_t			    width, height, refresh_rate;
				std::string		    name, description;
				uint32_t		    global_name;
				struct llkit::globals::obj* globals = nullptr;
				struct wl_output*	    output  = nullptr;
		};
	}
}

#endif // LLKIT_OUTPUT_H
