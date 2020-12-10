#ifndef FILE_H
#define FILE_H


#include <string.h>
#include <sys/stat.h>

#include "error.h"
#include "str.h"


typedef int fd_t;


err_t create_file(str_t pathname, mode_t mode);


int const permissions_rwx_len;
int const permissions_rwxrwxrwx_len;
int const permissions_urwxgrwxorwx_len;

extern mode_t const permissions_mode_bits[];
size_t const num_permissions_mode_bits;

err_t permissions_rwxrwxrwx_to_urwxgrwxorwx(str_t perm_rwxrwxrwx, str_t perm_urwxgrwxorwx);
err_t permissions_mode_to_rwxrwxrwx(mode_t perm_mode, str_t perm_rwxrwxrwx);
err_t get_permissions_rwxrwxrwx(str_t pathname, str_t perm_rwxrwxrwx);
err_t set_permissions_rwxrwxrwx(str_t pathname, str_t perm_rwxrwxrwx);
err_t print_permissions_rwxrwxrwx_pathname(str_t pathname);


#endif // FILE_H
