#include <wchar.h>
#include <stdlib.h>
#include <limits.h>

int __mb_cur_max(void) {
	return MB_LEN_MAX;
}
