#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int _vasprintf_c(char **__restrict p_str, const char *__restrict format, va_list va) {
	*p_str = NULL;
	int str_len = vsnprintf(NULL, 0, format, va);
	if (str_len <= 0) {
		return str_len;
	}
	size_t buf_len = (size_t)str_len + 1;
	char* buf = (char*)malloc(buf_len);
	if (buf == NULL) {
		// malloc failure
		return -1;
	}
	int ret = vsnprintf(buf, buf_len, format, va);
	if (ret <= 0) {
		free(buf);
		return ret;
	}
	*p_str = buf;
	return ret;
}

int _asprintf_c(char **__restrict p_str, const char *__restrict format, ...) {
	int ret;
	va_list va;
	va_start(va, format);
	ret = _vasprintf_c(p_str, format, va);
	va_end(va);
	return ret;
}
