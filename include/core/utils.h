#ifndef LLKIT_UTILS_H
#define LLKIT_UTILS_H

#include <wayland-client.h>

template<typename type>
auto wl_array_loop = [](wl_array* wl_array, auto func) {
	const char* wl_array_data = static_cast<const char*>(wl_array->data);
	for (const char* pos = wl_array_data; wl_array->size != 0 && pos < wl_array_data; pos++)
		func(( type ) pos);
};

#endif // LLKIT_UTILS_H
