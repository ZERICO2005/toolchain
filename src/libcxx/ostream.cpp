#include <iostream>
#include <iomanip>
#include <cstdio>

#include <cstdint>


namespace std {

ostream cout = ostream(nullptr);

static ios_base::fmtflags print_flags;

__attribute__((__unused__)) static size_t print_width = 0;
__attribute__((__unused__)) static size_t print_precision = 6;
__attribute__((__unused__)) static char print_fill = ' ';

static int write_cstr(const char* str) {
	return fputs(str, stdout);
}

static int write_integer(ostream& os, unsigned long long x, bool is_signed) {
	char buf[40];
	char format[sizeof("%+-0#*llu")];
	char* fmt = format;
	const basic_ostream<char>::fmtflags flags = print_flags;
	*fmt++ = '%';
	if (flags & ios_base::showpos) {
		*fmt++ = '+';
	}
	if (flags & ios_base::left) {
		*fmt++ = '-';
	}
	if (print_fill == '0') {
		*fmt++ = '0';
	}
	char base_ch = 'u';
	if (flags & ios_base::hex) {
		base_ch = (flags & ios_base::uppercase) ? 'X' : 'x';
		if (flags & ios_base::showbase) {
			*fmt++ = '#';
		}
	} else if (flags & ios_base::oct) {
		base_ch = 'o';
		if (flags & ios_base::showbase) {
			*fmt++ = '#';
		}
	} else if (is_signed) {
		base_ch = 'd';
	}
	*fmt++ = '*';
	*fmt++ = 'l';
	*fmt++ = 'l';
	*fmt++ = base_ch;
	*fmt++ = '\0';
	snprintf(buf, sizeof(buf), format, print_width, x);
	return write_cstr(buf);
}

static int write_float(ostream& os, double x) {
	char buf[40];
	char format[sizeof("%+-0#*.*f")];
	char* fmt = format;
	const basic_ostream<char>::fmtflags flags = os.flags();
	*fmt++ = '%';
	if (flags & ios_base::showpos) {
		*fmt++ = '+';
	}
	if (flags & ios_base::left) {
		*fmt++ = '-';
	}
	if (print_fill == '0') {
		*fmt++ = '0';
	}
	if (flags & ios_base::showpoint) {
		*fmt++ = '#';
	}
	// TODO: implement %g %e %a etc in printf
	char base_ch = 'f'; // supposed to be %g by default
	if (flags & ios_base::uppercase) {
		base_ch = toupper(base_ch);
	}
	*fmt++ = '*';
	*fmt++ = '.';
	*fmt++ = '*';
	*fmt++ = base_ch;
	*fmt++ = '\0';
	snprintf(buf, sizeof(buf), format, print_width, print_precision, x);
	return write_cstr(buf);
}

/* write char */

// implementation file
template<>
ostream& ostream::put(ostream::char_type ch) {
	fputc(ch, stdout);
    return *this;
}

template<>
ostream& ostream::write(const basic_ostream<char>::char_type* str, streamsize n) {
	fwrite(str, sizeof(char), n, stdout);
	return *this;
}

template<>
ostream& ostream::flush() {
	fflush(stdout);
	return *this;
}

/* write bool */

template<>
ostream& ostream::operator<<(bool b) {
	const char* str;
	if (this->flags() & ios_base::boolalpha) {
		str = (b ? "true" : "false");
	} else {
		str = (b ? "1" : "0");
	}
	write_cstr(str);
	return *this;
}

/* write signed */

template<>
ostream& ostream::operator<<(short x) {
	write_integer(*this, static_cast<long long>(x), true);
	return *this;
}

template<>
ostream& ostream::operator<<(int x) {
	write_integer(*this, static_cast<long long>(x), true);
	return *this;
}

template<>
ostream& ostream::operator<<(long x) {
	write_integer(*this, static_cast<long long>(x), true);
	return *this;
}

template<>
ostream& ostream::operator<<(signed __int48 x) {
	write_integer(*this, static_cast<long long>(x), true);
	return *this;
}

template<>
ostream& ostream::operator<<(long long x) {
	write_integer(*this, static_cast<long long>(x), true);
	return *this;
}

/* write unsigned */

template<>
ostream& ostream::operator<<(unsigned short x) {
	write_integer(*this, static_cast<unsigned long long>(x), false);
	return *this;
}

template<>
ostream& ostream::operator<<(unsigned int x) {
	write_integer(*this, static_cast<unsigned long long>(x), false);
	return *this;
}

template<>
ostream& ostream::operator<<(unsigned long x) {
	write_integer(*this, static_cast<unsigned long long>(x), false);
	return *this;
}

template<>
ostream& ostream::operator<<(unsigned __int48 x) {
	write_integer(*this, static_cast<unsigned long long>(x), false);
	return *this;
}

template<>
ostream& ostream::operator<<(unsigned long long x) {
	write_integer(*this, static_cast<unsigned long long>(x), false);
	return *this;
}

template<>
ostream& ostream::operator<<(const void* x) {
	printf("%p", x);
	return *this;
}

template<>
ostream& ostream::operator<<(const volatile void* x) {
	printf("%p", x);
	return *this;
}

template<>
ostream& ostream::operator<<(nullptr_t) {
	printf("%p", nullptr);
	return *this;
}

/* write float */

template<>
ostream& ostream::operator<<(float x) {
	write_float(*this, static_cast<double>(x));
	return *this;
}

template<>
ostream& ostream::operator<<(double x) {
	write_float(*this, static_cast<double>(x));
	return *this;
}

template<>
ostream& ostream::operator<<(long double x) {
	write_float(*this, static_cast<double>(x));
	return *this;
}

} // namespace std
