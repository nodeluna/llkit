#include <string_view>
#include "errors.h"

namespace llkit {
	struct llkit_err set_error(const bool& is_err, const std::string_view& msg, const int& code) {
		struct llkit_err lerr;

		lerr.err  = is_err;
		lerr.msg  = msg;
		lerr.code = code;

		return lerr;
	}
}
