#ifndef LLKIT_WM_XDG_H
#define LLKIT_WM_XDG_H

#include <optional>
#include <wayland-client-protocol.h>
#include <wayland-client.h>
#include <atomic>
#include "protocols/xdg-shell-client.h"
#include "core/globals.h"

namespace llkit {
	namespace xdg_wm {
		void ping(void* data, struct xdg_wm_base* xdg_wm_base, uint32_t serial);

		class base {
			protected:
				struct xdg_wm_base*	    xdg_wm_base		 = nullptr;
				struct xdg_wm_base_listener xdg_wm_base_listener = {
				    .ping = llkit::xdg_wm::ping,
				};

			public:
				explicit base(struct wl_registry* registry, uint32_t name, const wl_interface* interface, uint32_t version);
				struct xdg_wm_base*		   get_xdg_wm_base();
				const struct xdg_wm_base_listener& get_xdg_wm_base_listener();
				~base();
		};

		void surface_configure(void* data, struct xdg_surface* xdg_surface, uint32_t serial);

		class surface {
			protected:
				std::atomic<uint32_t>	    current_surface_serial = 0;
				std::atomic<bool>	    xdg_surface_configured = false;
				struct wl_surface*	    wl_surface		   = nullptr;
				struct xdg_surface*	    xdg_surface		   = nullptr;
				struct xdg_surface_listener xdg_surface_listener   = {
				      .configure = llkit::xdg_wm::surface_configure,
				  };

			public:
				explicit surface(llkit::globals::obj* globals, struct xdg_wm_base* xdg_wm_base);
				struct wl_surface*		   get_wl_surface();
				struct xdg_surface*		   get_xdg_surface();
				const struct xdg_surface_listener& get_xdg_surface_listener();
				void				   set_current_surface_serial(const uint32_t& serial);
				uint32_t			   get_current_surface_serial(void);
				void				   set_xdg_surface_configured(const bool& state);
				bool				   get_xdg_surface_configured(void);
				~surface(void);
		};

		void toplevel_configure(
		    void* data, struct xdg_toplevel* xdg_toplevel, int32_t width, int32_t height, struct wl_array* window_states);

		void toplevel_close(void* data, struct xdg_toplevel* xdg_toplevel);

		void toplevel_configure_bounds(void* data, struct xdg_toplevel* xdg_toplevel, int32_t width, int32_t height);

		void toplevel_capabilities(void* data, struct xdg_toplevel* xdg_toplevel, struct wl_array* capability);

		class toplevel {
			protected:
				struct xdg_toplevel*	     xdg_toplevel	     = nullptr;
				std::atomic<bool>	     close_window	     = false;
				std::atomic<bool>	     xdg_toplevel_configured = false;
				std::string		     title		     = "llkit application";
				int32_t			     width = 800, height = 1200;
				struct xdg_toplevel_listener xdg_toplevel_listener = {
				    .configure	      = llkit::xdg_wm::toplevel_configure,
				    .close	      = llkit::xdg_wm::toplevel_close,
				    .configure_bounds = llkit::xdg_wm::toplevel_configure_bounds,
				    .wm_capabilities  = llkit::xdg_wm::toplevel_capabilities,
				};

			public:
				explicit toplevel(struct xdg_surface* xdg_surface, llkit::globals::obj* globals);
				struct xdg_toplevel*		    get_xdg_toplevel();
				const struct xdg_toplevel_listener& get_xdg_toplevel_listener();
				bool				    get_close_window();
				std::pair<int32_t, int32_t>	    get_dimensions();
				void				    set_xdg_toplevel_configured(const bool& state);
				bool				    get_xdg_toplevel_configured(void);
				bool				    get_xdg_configuration_state(void);
				void				    set_dimensions(const std::pair<int32_t, int32_t>& xy);
				void				    set_close_window(const bool& close);
				~toplevel(void);
		};

	}
}

#endif // LLKIT_WM_XDG_H
