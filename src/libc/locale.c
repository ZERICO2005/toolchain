#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

const struct lconv __c_locale = {
    .decimal_point =       ".",
    .thousands_sep =       "",
    .grouping =            "",
    .mon_decimal_point =   "",
    .mon_thousands_sep =   "",
    .mon_grouping =        "",
    .positive_sign =       "",
    .negative_sign =       "",
    .currency_symbol =     "",
    .frac_digits =         CHAR_MAX,
    .p_cs_precedes =       CHAR_MAX,
    .n_cs_precedes =       CHAR_MAX,
    .p_sep_by_space =      CHAR_MAX,
    .n_sep_by_space =      CHAR_MAX,
    .p_sign_posn =         CHAR_MAX,
    .n_sign_posn =         CHAR_MAX,
    .int_curr_symbol =     "",
    .int_frac_digits =     CHAR_MAX,
    .int_p_cs_precedes =   CHAR_MAX,
    .int_n_cs_precedes =   CHAR_MAX,
    .int_p_sep_by_space =  CHAR_MAX,
    .int_n_sep_by_space =  CHAR_MAX,
    .int_p_sign_posn =     CHAR_MAX,
    .int_n_sign_posn =     CHAR_MAX,
};

static struct __locale_struct __global_locale;

//------------------------------------------------------------------------------
// LIBC locale.h
//------------------------------------------------------------------------------

static const char *__get_locale(const char *locale) {
    // query the current locale without modifiying it
    if (locale == NULL) {
        return "C";
    }
    // use preferred locale (the "C" locale)
    if (locale[0] == '\0') {
        return "C";
    }
    // use the "C" locale
    if (locale[0] == 'C' && locale[1] == '\0') {
        return "C";
    }
    // The "POSIX" locale is the same as the "C" locale
    if (strcmp(locale, "POSIX") == 0) {
        return "C";
    }
    // unknown locale
    return NULL;
}

char *setlocale(__attribute__((__unused__)) int category, const char *locale) {
    return (char*)__get_locale(locale);
}

struct lconv *localeconv(void) {
    // it is undefined behavior to modify the returned object
    return (struct lconv*)&__c_locale;
}

//------------------------------------------------------------------------------
// POSIX locale.h
//------------------------------------------------------------------------------

locale_t uselocale(locale_t new_locale) {
    // query the current locale_t without modifiying it
    if (new_locale == (locale_t)0) {
        return LC_GLOBAL_LOCALE;
    }
    if (new_locale == LC_GLOBAL_LOCALE) {
        return LC_GLOBAL_LOCALE;
    }
    if (new_locale == &__global_locale) {
        return LC_GLOBAL_LOCALE;
    }
    // EINVAL: new_locale does not refer to a valid locale object
    errno = EINVAL;
    return (locale_t)0;
}

locale_t duplocale(locale_t locale_obj) {
    if (locale_obj == NULL) {
        return (locale_t)0;
    }
    if (locale_obj == LC_GLOBAL_LOCALE) {
        locale_obj = &__global_locale;
    }
    locale_t locale_dup = (locale_t)malloc(sizeof(struct __locale_struct));
    // ENOMEM: insufficient memory
    if (locale_dup == NULL) {
        errno = ENOMEM;
        return (locale_t)0;
    }
    locale_dup->__name = locale_obj->__name;
    locale_dup->__data = locale_obj->__data;
    return locale_dup;
}

void freelocale(locale_t locale_obj) {
    if (locale_obj == LC_GLOBAL_LOCALE) {
        return;
    }
    if (locale_obj == &__global_locale) {
        return;
    }
    free(locale_obj);
}

locale_t newlocale(int category_mask, const char *locale, locale_t base) {
    // EINVAL: locale is NULL
    if (locale == NULL) {
        errno = EINVAL;
        return (locale_t)0;
    }

    // EINVAL: category_mask does not correspond to a valid locale category
    if (((unsigned int)category_mask > (unsigned int)LC_ALL_MASK)) {
        errno = EINVAL;
        return (locale_t)0;
    }

    // ENOENT: not a valid locale
    if (__get_locale(locale) == NULL) {
        errno = ENOENT;
        return (locale_t)0;
    }

    locale_t locale_obj = (locale_t)malloc(sizeof(struct __locale_struct));
    // ENOMEM: insufficient memory
    if (locale_obj == NULL) {
        errno = ENOMEM;
        return (locale_t)0;
    }
    if (base == (locale_t)0) {
        locale_obj->__name = __global_locale.__name;
        locale_obj->__data = __global_locale.__data;
        return locale_obj;
    }
    if (base == LC_GLOBAL_LOCALE) {
        base = &__global_locale;
    }
    locale_obj->__name = base->__name;
    locale_obj->__data = base->__data;
    freelocale(base);
    return locale_obj;
}
