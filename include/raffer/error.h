#ifndef ERROR_H
#define ERROR_H


#include <stdbool.h>


typedef int err_t;

enum {
	ok = 0,
	error = -1,
};	

bool is_error(err_t err);


#endif // ERROR_H
