#ifndef UTILITIES_H
#define UTILITIES_H

struct Error {
	Error(const char * in_msg) : msg(in_msg) {}
	const char * msg;
};

/* add any of your own declarations here */

#endif
