#include <print>
#include <optional>
#include <llkit/core/window.h>


int main(){
	auto window = llkit::window::obj(DEFAULT_DISPLAY);
	if(window.error)
		std::println("{}", window.error.value().msg);
	else
		std::println("llkit is running");

	std::optional<struct llkit_err> loop = window.main_loop();
	if(loop)
		std::println("{}", loop.value().msg);

	return 0;
}
