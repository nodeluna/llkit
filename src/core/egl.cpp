#include <memory>
#include <wayland-client-core.h>
#include <wayland-client.h>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "core/egl.h"
#include "core/globals.h"
#include "core/registry.h"
#include "errors.h"

namespace llkit {
	egl::egl(const struct llkit::registry::data& rd, llkit::globals::obj* globals)
	    : llkit::xdg_wm::base(rd.wl_registry, rd.name, rd.interface, rd.version), llkit::xdg_wm::surface(globals, xdg_wm_base),
	      llkit::xdg_wm::toplevel(xdg_surface, globals), wl_display(globals->wl_display) {
		llkit::print("[llkit::egl::constructor] exiting function");
	}

	void egl::init(void) {
		llkit::print("[llkit::egl::init] entered function");
		wl_surface_commit(wl_surface);
		wl_display_roundtrip(wl_display);
		llkit::print("done roundtrip");

		egl_display = eglGetDisplay(( EGLNativeDisplayType ) wl_display);
		if (egl_display == EGL_NO_DISPLAY) {
			llkit::log_error("[llkit::egl::init] couldn't create egl_display");
			return;
		}

		if (eglInitialize(egl_display, &major, &minor) != EGL_TRUE) {
			llkit::log_error("[llkit::egl::init] couldn't initialize egl_display");
			return;
		}

		if (eglGetConfigs(egl_display, NULL, 0, &num_config) != EGL_TRUE) {
			llkit::log_error("[llkit::egl::init] couldn't get egl config");
			return;
		}

		EGLint config_attributes[] = {EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_RED_SIZE, 8,
		    EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_NONE};

		if (eglChooseConfig(egl_display, config_attributes, &egl_config, 1, &num_config) != EGL_TRUE) {
			llkit::log_error("[llkit::egl::init] couldn't choose egl config");
			return;
		}
		this->get_last_error();
		EGLint context_attributes[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
		egl_context		    = eglCreateContext(egl_display, egl_config, EGL_NO_CONTEXT, context_attributes);
		if (egl_context == EGL_NO_CONTEXT) {
			llkit::log_error("[llkit::egl::init] couldn't create egl context");
			return;
		}
		this->get_last_error();

		auto xy = this->get_dimensions();
		llkit::print("[llkit::egl::init] width: {}, height: {}", xy.first, xy.second);
		egl_window = wl_egl_window_create(wl_surface, xy.first, xy.second);
		if (egl_window == EGL_NO_SURFACE || egl_window == nullptr) {
			llkit::log_error("[llkit::egl::init] couldn't create egl_window");
			return;
		}

		egl_surface = eglCreateWindowSurface(egl_display, egl_config, egl_window, NULL);
		if (egl_surface == EGL_NO_SURFACE) {
			llkit::log_error("[llkit::egl::init] couldn't create egl_surface");
			this->get_last_error();
			return;
		}
		this->get_last_error();

		if (eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context) != EGL_TRUE) {
			llkit::log_error("[llkit::egl::init] eglMakeCurrent failed");
			return;
		}

		this->render();

		llkit::print("[llkit::egl::init] exiting function");
	}

	void egl::render(void) {
		glClearColor(1.0, 0.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glFlush();

		if (eglSwapBuffers(egl_display, egl_surface) != EGL_TRUE) {
			llkit::log_error("[llkit::egl::render()] failed to swap buffers");
		} else {
			llkit::print("[llkit::egl::render()] swaped buffers successfully");
		}
	}

	struct wl_egl_window* egl::get_egl_window(void) {
		return egl_window;
	}

	bool egl::egl_running(void) {
		if (egl_display == EGL_NO_DISPLAY || egl_context == EGL_NO_CONTEXT || egl_window == EGL_NO_SURFACE ||
		    egl_surface == EGL_NO_SURFACE)
			return false;
		return true;
	}

	EGLDisplay& egl::get_egl_display(void) {
		return egl_display;
	}

	EGLContext& egl::get_egl_context(void) {
		return egl_context;
	}

	EGLSurface& egl::get_egl_surface(void) {
		return egl_surface;
	}

	EGLConfig& egl::get_egl_config(void) {
		return egl_config;
	}

	void egl::get_last_error(void) {
		EGLint error = eglGetError();
		switch (error) {
			case EGL_SUCCESS:
				llkit::print("[egl success]");
				break;
			case EGL_BAD_CONFIG:
				llkit::log_error("[egl error] bad config error");
				break;
			case EGL_BAD_SURFACE:
				llkit::log_error("[egl error] bad surface error");
				break;
			case EGL_BAD_CONTEXT:
				llkit::log_error("[egl error] bad context error");
				break;
			case EGL_BAD_CURRENT_SURFACE:
				llkit::log_error("[egl error] bad current surface");
				break;
			case EGL_BAD_ATTRIBUTE:
				llkit::log_error("[egl error] bad attribute");
				break;
			case EGL_BAD_DISPLAY:
				llkit::log_error("[egl error] bad display error");
				break;
			case EGL_BAD_ALLOC:
				llkit::log_error("[egl error] failed to allocate resources for the last operation");
				break;
			case EGL_BAD_ACCESS:
				llkit::log_error("[egl error] egl bad access. cannot access a resource whice is bound to another thread");
				break;
			case EGL_NOT_INITIALIZED:
				llkit::log_error(
				    "[egl error] egl not initialized. egl couldn't be initialized for the specified egl display");
				break;
			case EGL_CONTEXT_LOST:
				llkit::log_error("[egl error] egl context lost");
				break;
			case EGL_BAD_MATCH:
				llkit::log_error("[egl error] egl bad match, arguments are inconsistent");
				break;
			case EGL_BAD_PARAMETER:
				llkit::log_error("[egl error] egl bad parameteres, one or more arguments are invalid");
				break;
			default:
				llkit::log_error("[egl error] error");
				break;
		}
	}

	egl::~egl() {
		llkit::print("[llkig::egl::destructor] entered function");
		if (not egl_window)
			wl_egl_window_destroy(egl_window);
		if (egl_context != EGL_NO_CONTEXT)
			eglDestroyContext(egl_display, egl_context);
		if (egl_surface != EGL_NO_SURFACE)
			eglDestroySurface(egl_display, egl_surface);
		if (egl_display != EGL_NO_DISPLAY)
			eglTerminate(egl_display);
		llkit::print("[llkig::egl::destructor] exiting function");
	}
}
