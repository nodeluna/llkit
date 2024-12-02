#include <utility>
#include <wayland-client-protocol.h>
#include <wayland-client.h>
#include <wayland-egl-core.h>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "protocols/xdg-shell-client.h"
#include "core/globals.h"
#include "core/xdg_wm.h"
#include "core/egl.h"
#include "errors.h"

namespace llkit {
	namespace xdg_wm {
		void ping(void* data, struct xdg_wm_base* xdg_wm_base, uint32_t serial) {
			llkit::print("[xdg_wm_base_listener::ping] entered function");
			xdg_wm_base_pong(xdg_wm_base, serial);
			llkit::print("[xdg_wm_base_listener::ping] exiting function");
		}

		base::base(struct wl_registry* registry, uint32_t name, const wl_interface* interface, uint32_t version) {
			llkit::print("[llkit::xdg_wm::base::constructor] entered function");
			xdg_wm_base = static_cast<struct xdg_wm_base*>(wl_registry_bind(registry, name, &xdg_wm_base_interface, version));
			if (not xdg_wm_base) {
				llkit::log_error("llkit::xdg_wm::base::constructor] xdg_wm_base is null");
				return;
			}
			xdg_wm_base_add_listener(xdg_wm_base, &xdg_wm_base_listener, NULL);
			llkit::print("[llkit::xdg_wm::base::constructor] exiting function");
		}

		struct xdg_wm_base* base::get_xdg_wm_base() {
			return xdg_wm_base;
		}

		void xdg_surface_done_configuration(void* data, std::shared_ptr<llkit::egl> ll_egl) {
			xdg_surface_ack_configure(ll_egl->get_xdg_surface(), ll_egl->get_current_surface_serial());
			wl_surface_commit(ll_egl->get_wl_surface());
			ll_egl->set_xdg_toplevel_configured(false);
			ll_egl->set_xdg_surface_configured(false);
			llkit::print("[xdg_surface_done_configuration()] ack and commit wl_surface done");
		}

		const struct xdg_wm_base_listener& base::get_xdg_wm_base_listener() {
			llkit::print("[llkit::xdg_wm::base::get_listener()]");
			return xdg_wm_base_listener;
		}

		base::~base() {
			llkit::print("[llkit::xdg_wm::base::destructor] entered function");
			if (not xdg_wm_base)
				xdg_wm_base_destroy(xdg_wm_base);
			llkit::print("[llkit::xdg_wm::base::destructor] exiting function");
		}

		void surface_configure(void* data, struct xdg_surface* xdg_surface, uint32_t serial) {
			llkit::print("[xdg_surface_listener::configure] entered function");
			std::shared_ptr<llkit::egl> ll_egl = static_cast<llkit::globals::obj*>(data)->ll_egl;
			ll_egl->set_current_surface_serial(serial);

			ll_egl->set_xdg_surface_configured(true);
			if (ll_egl->get_xdg_surface_configured() && ll_egl->get_xdg_toplevel_configured()) {
				xdg_surface_done_configuration(data, ll_egl);
			}

			llkit::print("[xdg_surface_listener::configure] exiting function");
		}

		surface::surface(llkit::globals::obj* globals, struct xdg_wm_base* xdg_wm_base)
		    : wl_surface(wl_compositor_create_surface(globals->ll_compositor->get_wl_compositor())) {
			llkit::print("[llkit::xdg_wm::surface::constructor] entered function");
			if (wl_surface == nullptr) {
				llkit::log_error("couldn't initialize wl_surface");
				return;
			}

			xdg_surface = xdg_wm_base_get_xdg_surface(xdg_wm_base, wl_surface);
			if (xdg_surface == nullptr) {
				llkit::log_error("[surface class constructor] couldn't get xdg_surface");
				return;
			}
			xdg_surface_add_listener(xdg_surface, &xdg_surface_listener, globals);

			llkit::print("[llkit::xdg_wm::surface::constructor] exiting function");
		}

		void surface::set_xdg_surface_configured(const bool& state) {
			xdg_surface_configured = state;
		}

		bool surface::get_xdg_surface_configured(void) {
			return xdg_surface_configured;
		}

		const struct xdg_surface_listener& surface::get_xdg_surface_listener(void) {
			return xdg_surface_listener;
		}

		struct wl_surface* surface::get_wl_surface(void) {
			return wl_surface;
		}

		struct xdg_surface* surface::get_xdg_surface(void) {
			return xdg_surface;
		}

		void surface::set_current_surface_serial(const uint32_t& serial) {
			llkit::print("[llkit::xdg_wm::surface::set_current_surface_serial] entered function");
			current_surface_serial.store(serial);
			llkit::print("[llkit::xdg_wm::surface::set_current_surface_serial] exiting function");
		}

		uint32_t surface::get_current_surface_serial(void) {
			llkit::print("[llkit::xdg_wm::surface::get_current_surface_serial] entered function");
			llkit::print("[llkit::xdg_wm::surface::get_current_surface_serial] exiting function");
			return current_surface_serial.load();
		}

		surface::~surface(void) {
			llkit::print("[llkit::xdg_wm::surface::destructor] entering function");
			if (xdg_surface != nullptr)
				xdg_surface_destroy(xdg_surface);
			if (wl_surface != nullptr)
				wl_surface_destroy(wl_surface);
			llkit::print("[llkit::xdg_wm::surface::destructor] exiting function");
		}

		toplevel::toplevel(struct xdg_surface* xdg_surface, llkit::globals::obj* globals)
		    : xdg_toplevel(xdg_surface_get_toplevel(xdg_surface)) {
			llkit::print("[llkit::xdg_wm::toplevel::constructor] entered function");
			if (not xdg_toplevel) {
				llkit::log_error("couldn't initialize xdg_toplevel");
				return;
			}
			xdg_toplevel_set_title(xdg_toplevel, title.c_str());
			xdg_toplevel_add_listener(xdg_toplevel, &xdg_toplevel_listener, globals);
			llkit::print("[llkit::xdg_wm::toplevel::constructor] exiting function");
		}

		void toplevel::set_xdg_toplevel_configured(const bool& state) {
			xdg_toplevel_configured = state;
		}

		bool toplevel::get_xdg_toplevel_configured(void) {
			return xdg_toplevel_configured;
		}

		struct xdg_toplevel* toplevel::get_xdg_toplevel(void) {
			return xdg_toplevel;
		}

		const struct xdg_toplevel_listener& toplevel::get_xdg_toplevel_listener(void) {
			return xdg_toplevel_listener;
		}

		bool toplevel::get_close_window(void) {
			return close_window.load();
		}

		std::pair<int32_t, int32_t> toplevel::get_dimensions(void) {
			return std::make_pair(width, height);
		}

		void toplevel::set_dimensions(const std::pair<int32_t, int32_t>& xy) {
			width  = xy.first;
			height = xy.second;
		}

		void toplevel::set_close_window(const bool& close) {
			close_window.store(close);
		}

		toplevel::~toplevel(void) {
			if (xdg_toplevel != nullptr)
				xdg_toplevel_destroy(xdg_toplevel);
		}

		void toplevel_configure(
		    void* data, struct xdg_toplevel* xdg_toplevel, int32_t width, int32_t height, struct wl_array* window_states) {
			llkit::print("[xdg_toplevel_listener::configure] entered function");
			std::shared_ptr<llkit::egl> ll_egl = static_cast<llkit::globals::obj*>(data)->ll_egl;
			if (not ll_egl) {
				llkit::log_error("ll_egl class object is null");
				return;
			}
			auto xy = ll_egl->get_dimensions();

			llkit::print("toplevel width: {}, height: {}", width, height);

			ll_egl->set_xdg_toplevel_configured(true);
			if (ll_egl->egl_running() && (xy.first != width || xy.second != height)) {
				ll_egl->set_dimensions(std::make_pair(width, height));
				llkit::print("[callback::toplevel_configure()] changing toplevel window dimensions");
				wl_egl_window_resize(ll_egl->get_egl_window(), width, height, 0, 0);
				glViewport(0, 0, width, height);

				ll_egl->render();
			} else if (not ll_egl->egl_running()) {
				llkit::log_error("!! egl not running");
			}

			ll_egl->set_xdg_toplevel_configured(true);
			if (ll_egl->get_xdg_surface_configured() && ll_egl->get_xdg_toplevel_configured()) {
				xdg_surface_done_configuration(data, ll_egl);
			}
			llkit::print("[xdg_toplevel_listener::configure] exiting function");
		}

		void toplevel_close(void* data, struct xdg_toplevel* xdg_toplevel) {
			llkit::print("[xdg_toplevel_listener::close] entered function");
			struct llkit::globals::obj* globals = static_cast<llkit::globals::obj*>(data);
			globals->ll_egl->set_close_window(true);
			llkit::print("[xdg_toplevel_listener::close] exiting function");
		}

		void toplevel_configure_bounds(void* data, struct xdg_toplevel* xdg_toplevel, int32_t width, int32_t height) {
			llkit::print("[xdg_toplevel_listener::configure_bounds] entered function");
			llkit::print("[xdg_toplevel_listener::configure_bounds] exiting function");
		}

		void toplevel_capabilities(void* data, struct xdg_toplevel* xdg_toplevel, struct wl_array* capability) {
			llkit::print("[xdg_toplevel_listener::capabilities] entered function");
			llkit::print("[xdg_toplevel_listener::capabilities] exiting function");
		}
	};
}
