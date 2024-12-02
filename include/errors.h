#ifndef LLKIT_ERRS_H
#define LLKIT_ERRS_H

#include <string>
#include <format>
#include <print>
#include <utility>

#define LLKIT_OK 0
#define LLKIT_WARN 1
#define LLKIT_ERR 2

#define PRINT_LOGS_TO_STDOUT

struct llkit_err {
		bool	    err = false;
		std::string msg;
		int	    code = 0;
};

namespace llkit {
	template<typename... args_t> void log_error(std::format_string<args_t...> fmt, args_t&&... args) {
#ifdef PRINT_LOGS_TO_STDOUT
		std::string output = "\x1b[1;31m-[X]" + std::format(fmt, std::forward<args_t>(args)...) + " \x1b[0m";
		std::println("{}", output);
#endif
	}

	template<typename... args_t> void print(std::format_string<args_t...> fmt, args_t&&... args) {
#ifdef PRINT_LOGS_TO_STDOUT
		std::println(fmt, std::forward<args_t>(args)...);
#endif
	}

	struct llkit_err set_error(const bool& is_err, const std::string_view& msg, const int& code);
}

#endif // LLKIT_ERRS_H
