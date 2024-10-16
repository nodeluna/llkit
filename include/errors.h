#ifndef LLKIT_ERRS_H
#define LLKIT_ERRS_H

#include <string>

#define LLKIT_OK 0
#define LLKIT_WARN 1
#define LLKIT_ERR 1

struct llkit_err {
		bool	    err = false;
		std::string msg;
		int	    code = 0;
};

namespace llkit {
	struct llkit_err set_error(const bool& is_err, const std::string_view& msg, const int& code);
}

#endif // LLKIT_ERRS_H
