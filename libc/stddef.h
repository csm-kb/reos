//
// Created by amadeus on 1/12/21.
//

#ifndef REOS_STDDEF_H
#define REOS_STDDEF_H

#ifndef NULL
#define NULL            ((void*)0)
#endif /* NULL */

#ifndef _WCHAR_T
#define _WCHAR_T
typedef unsigned int    wchar_t;
#endif /* _WCHAR_T */

#ifndef _PTRDIFF_T
#define _PTRDIFF_T
typedef long            ptrdiff_t;
#endif /* _PTRDIFF_T */

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned long   size_t;
#endif /* _SIZE_T */

#define offsetof(st, m) ((size_t)((char *)&((st *)0)->m - (char *)0))

#endif //REOS_STDDEF_H
