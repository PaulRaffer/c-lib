#include "str.h"

#include <string.h>


// entfernt alle Zeichen c aus String str
str_t rmchar(str_t str, char c)
{
	// Ende suchen
        str_t end = strchr(str, '\0');
        str_t pos;
	// Solange noch Zeichen vorhanden sind: nächstes suchen
        while (pos = strchr(str, c)) {
		// alle Zeichen danach um ein Zeichen nach links verschieben
                memmove(pos, pos + 1, end-- - pos + 1);
        }
        return str;
}
