#ifndef LLKIT_EGL_H
#define LLKIT_EGL_H

#include <memory>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "core/registry.h"
#include "core/xdg_wm.h"
#include "core/globals.h"

namespace llkit::registry {
	struct data;
}

namespace llkit {
	class egl : public llkit::xdg_wm::base, public llkit::xdg_wm::surface, public llkit::xdg_wm::toplevel {
		protected:
			struct wl_egl_window* egl_window = nullptr;
			struct wl_display*    wl_display = nullptr;
			EGLConfig	      egl_config;
			EGLDisplay	      egl_display = EGL_NO_DISPLAY;
			EGLContext	      egl_context = EGL_NO_CONTEXT;
			EGLSurface	      egl_surface = EGL_NO_SURFACE;
			EGLint		      num_config, major, minor;

		public:
			explicit egl(const struct llkit::registry::data& rd, llkit::globals::obj* globals);
			void		      init(void);
			void		      render(void);
			struct wl_egl_window* get_egl_window(void);
			EGLDisplay&	      get_egl_display(void);
			EGLContext&	      get_egl_context(void);
			EGLSurface&	      get_egl_surface(void);
			EGLConfig&	      get_egl_config(void);
			void		      get_last_error(void);
			bool		      egl_running(void);
			~egl();
	};
}

#endif // LLKIT_EGL_H
