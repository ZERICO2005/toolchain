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

static locale_t __current_locale = NULL;

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
    locale_t old_locale = __current_locale;
    if (new_locale == (locale_t)0) {
        return old_locale;
    }
    __current_locale = new_locale;
    return old_locale;
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

    // ENOENT: not a valid locale.
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
    if (base != (locale_t)0) {
        locale_obj->name = base->name;
        locale_obj->data = base->data;
        return locale_obj;
    }
    locale_obj->name = "C";
    locale_obj->data = (struct lconv*)&__c_locale;
    return locale_obj;
}

void freelocale(locale_t locale_obj) {
    free(locale_obj);
}

locale_t duplocale(locale_t locale_obj) {
    if (locale_obj == NULL) {
        return NULL;
    }
    locale_t dup = malloc(sizeof(struct __locale_struct));
    if (dup == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    dup->name = locale_obj->name;
    dup->data = locale_obj->data;
    return dup;
}
