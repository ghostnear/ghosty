#ifndef LIBC_UNUSED_H
#define LIBC_UNUSED_H

/*
* "Uses" a function argument so that there are
* no warnings if it is not actually used.
*/
#define UNUSED(x) (void)(x)

#endif
