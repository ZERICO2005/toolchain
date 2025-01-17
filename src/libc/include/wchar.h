#ifndef _WCHAR_H
#define _WCHAR_H

#include <cdefs.h>

#ifndef _WCHAR_T_DEFINED
#define _WCHAR_T_DEFINED
#ifndef __cplusplus
typedef __WCHAR_TYPE__ wchar_t;
#endif /* __cplusplus */
#endif /* _WCHAR_T_DEFINED */

#ifndef _WCHAR_LIMITS_DEFINED
#define _WCHAR_LIMITS_DEFINED
#define WCHAR_WIDTH __WCHAR_WIDTH__
#define WCHAR_MIN (~__WCHAR_MAX__)
#define WCHAR_MAX   __WCHAR_MAX__
#endif

#define WEOF -1

__BEGIN_DECLS

wchar_t* wmemset( wchar_t *dst, wchar_t ch, size_t count );
wchar_t* wmemcpy( wchar_t *__restrict dst, const wchar_t *__restrict src, size_t count );

__END_DECLS

#endif /* _WCHAR_H */
