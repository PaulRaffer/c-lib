#ifndef CONDITIONAL_H
#define CONDITIONAL_H


// wenn cond_func(src) == true wird src abhängig von dest
// enweder zurückgegeben, oder einer anderen Variable zugewiesen
// als type muss der Datentyp von src übergeben werden
#define IF(cond_func, type, dest, src) \
	{ \
		type const res__ = (src); \
		if (cond_func(res__)) { \
			dest res__; \
		} \
	}


#endif // CONDITIONAL_H
