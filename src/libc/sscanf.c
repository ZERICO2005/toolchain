/*
 * mini-scanf - Minimal scanf() implementation for embedded projects. 
 * Copyright (c) 2023 Aleksej Muratov
 */

// conf
// sscanf / scanf
#define C_SSCANF
// %[..]
#define LENSCANS 10

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>

// int c_isspace(const int c);
// int	c_isdigit(int c);
#define c_isspace isspace
#define c_isdigit isdigit

// parodies the standard
#define NEXTCHAR (PointBuf++)
#define CURCHAR (buff[PointBuf])

/**
 * https://github.com/MuratovAS/mini-scanf
 */
int sscanf(const char *__restrict buff, const char *__restrict format, ...)
{
	int count = 0;

	int PointBuf = 0;

	int PointFt = 0;

	va_list ap;
	va_start(ap, format);
	while (format && format[PointFt]) { // Read format
		if (format[PointFt] == '%') {
			PointFt++;
			// for %*
			bool save = true;
			if (format[PointFt] == '*') {
				save = false;
				PointFt++;
			}
			// for %1234567890
			unsigned len = 0;
			bool lenEn = false;
			while (c_isdigit(format[PointFt])) {
				lenEn = true;
				len *= 10;
				len += (format[PointFt] - '0');
				PointFt++;
			}
			// for %[]
			char stop[LENSCANS];
			unsigned stopN = 0;
			if (format[PointFt] == '[') {
				while (format[PointFt] != ']') {
					if (format[PointFt] != '[') {
						stop[stopN] = format[PointFt];
						stopN++;
					}
					PointFt++;
				}
			}
			// %?
			switch (format[PointFt]) {
				case 'c': {
					while (c_isspace(CURCHAR)) { // ignore isspace (std)
						NEXTCHAR; //
					}
					if (save) {
						*(char*)va_arg(ap, char*) = CURCHAR;
					}
					NEXTCHAR;
					//if (save) // ignore %* (std)
						count++;
				} break;
				case 'u':
				case 'd':
				{
					bool sign = false;
					while (!c_isdigit(CURCHAR)) {
						if (CURCHAR == '+' || CURCHAR == '-')
							if (CURCHAR == '-')
								//if(format[PointFt] != 'u') // ignore sign (no std)
									sign = true;
						NEXTCHAR;
					}
					unsigned int value = 0;
					while(c_isdigit(CURCHAR) && (lenEn != true || len > 0)) {
						value *= 10;
						value += ((unsigned char)CURCHAR - '0');
						NEXTCHAR;
						len--;
					}

					if (save) {
						if (sign) {
							value = -value;
						}
						*(int*)va_arg(ap, int*) = value;
					}
					//if (save) // ignore %* (std)
						count++;
				} break;
				case ']':
				case 's':
				{
					char* t = save ? va_arg(ap, char*) : NULL;

					while (c_isspace(CURCHAR)) { // ignor isspace (std)
						NEXTCHAR; //
					}

					while (true) {
						bool con = false;
						if (stopN != 0) {
							bool invert = (stop[0] == '^');
							con = !invert;
							for (unsigned i = (invert ? 1 : 0); i < stopN; i++) {
								if (stop[i] == CURCHAR) {
									con = invert;
									break;
								}
							}

							if (con == true) {
								break;
							}
						}

						if (!c_isspace(CURCHAR) || ((!con && stopN != 0) && (lenEn != true || len > 0))) {
							if (save) {
								*t = CURCHAR;
							}
							NEXTCHAR;
							t++;
							len--;
						} else {
							break;
						}
					}
					// add \0
					{
						if (save) {
							*t = '\0';
						}
						t++;
					}
					//if (save) // ignore %* (std)
						count++;
				} break;
			}
		}
		//else  // drop char in buff (no std)
		//	NEXTCHAR; //
		PointFt++;
	}
	va_end(ap);
	return count;
}
