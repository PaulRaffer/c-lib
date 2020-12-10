#include "file.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#include "str.h"
#include "array.h"


err_t create_file(str_t pathname, mode_t mode)
{
        fd_t fd;
	// File erstellen
        is_error(fd = creat(pathname, mode)) ||
	// File schließen
	is_error(fd = close(fd));
        return fd;
}


int const permissions_rwx_len = strlen("rwx");
int const permissions_rwxrwxrwx_len = 3 * permissions_rwx_len;
int const permissions_urwxgrwxorwx_len = permissions_rwxrwxrwx_len + (3 * 2) + 2;

mode_t const permissions_mode_bits[] = {
	S_IRUSR, S_IWUSR, S_IXUSR,
	S_IRGRP, S_IWGRP, S_IXGRP,
	S_IROTH, S_IWOTH, S_IXOTH,
};
size_t const num_permissions_mode_bits = LENGTH(permissions_mode_bits);


// konvertiert eine Rechtestruktur vom Format "rwxrwxrwx" ins Format "u=rwx,g=rwx,o=rwx"
err_t permissions_rwxrwxrwx_to_urwxgrwxorwx(str_t perm_rwxrwxrwx, str_t perm_urwxgrwxorwx)
{
        enum { usr, grp, oth, num_blocks };

        char perm_urwxgrwxorwx_block[permissions_rwx_len][num_blocks];
	// Input String in 3 Blöcke (usr, grp, oth) teilen:
        for (size_t b = 0; b < num_blocks; ++b) {
                perm_urwxgrwxorwx_block[b][permissions_rwx_len] = '\0';
                // 3 Zeichen (rwx) in neuen String kopieren
		memcpy(
                        perm_urwxgrwxorwx_block[b],
                        &perm_rwxrwxrwx[b*permissions_rwx_len],
                        permissions_rwx_len);
		// '-' entfernen
                rmchar(perm_urwxgrwxorwx_block[b], '-');
        }

	// 3 Blöcke zusammenfügen
        return snprintf(perm_urwxgrwxorwx, permissions_urwxgrwxorwx_len + 1,
                "u=%s,g=%s,o=%s",
                perm_urwxgrwxorwx_block[usr], perm_urwxgrwxorwx_block[grp], perm_urwxgrwxorwx_block[oth]);
}

// konvertiert Variable vom Typ mode_t zu einem String im Format "rwxrwxrwx"
err_t permissions_mode_to_rwxrwxrwx(mode_t perm_mode, str_t perm_rwxrwxrwx)
{
	for (size_t i = 0; i < num_permissions_mode_bits; ++i) {
		int const rem = i % permissions_rwx_len;
		// wenn Bit gesetzt ist, wird entsprechendes Zeichen (rwx)
		// im String gesetzt, sonst '-'
		perm_rwxrwxrwx[i] = (perm_mode & permissions_mode_bits[i]) ?
			(rem == 0 ? 'r' :
			(rem == 1 ? 'w' :
			(rem == 2 ? 'x' :
			'?' ))) : '-';
	}
	// Ende des Strings
	perm_rwxrwxrwx[num_permissions_mode_bits] = '\0';
	return perm_rwxrwxrwx ? ok : error;
}

// befüllt perm_rwxrwxrwx mit der Rechtestruktur eines Files
err_t get_permissions_rwxrwxrwx(str_t pathname, str_t perm_rwxrwxrwx)
{
	struct stat file_info;
	err_t res;
	// Mode auslesen
	is_error(res = stat(pathname, &file_info)) ||
	// Mode ins Format "rwxrwxrwx" konvertieren
	is_error(res = permissions_mode_to_rwxrwxrwx(file_info.st_mode, perm_rwxrwxrwx));
	return res;
}

// setzt dei Rechtestruktur eines Files auf perm_rwxrwxrwx
err_t set_permissions_rwxrwxrwx(str_t pathname, str_t perm_rwxrwxrwx)
{
        char perm_urwxgrwxorwx[permissions_urwxgrwxorwx_len + 1];
	err_t res;
	// Rechtestruktur vom Format "rwxrwxrwx" ins Format "u=rwx,g=rwx,o=rwx" konvertierten
        is_error(res = permissions_rwxrwxrwx_to_urwxgrwxorwx(perm_rwxrwxrwx, perm_urwxgrwxorwx)) ||
	// Rechtestruktur mit chmod setzen
        is_error(res = execlp("chmod", "chmod", perm_urwxgrwxorwx, pathname, NULL));
        return res;
}

// gibt die REchtestruktur einse Files in Kombination mit dem Pfad aus
err_t print_permissions_rwxrwxrwx_pathname(str_t pathname)
{
	char perm_rwxrwxrwx[permissions_rwxrwxrwx_len + 1];
	err_t res;
	// Rechtestruktur auslesen
	is_error(res = get_permissions_rwxrwxrwx(pathname, perm_rwxrwxrwx)) ||
	// Rechtestruktur ausgeben
	is_error(res = printf("%s %s\n", perm_rwxrwxrwx, pathname));
	return res;
}
