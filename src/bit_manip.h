/* utility macros for bit manipulation */

#ifndef __BIT_MANIP_H
#define __BIT_MANIP_H

// These are just bare minimum. Of course you can write more of them; don't do that. You will get to the point where it
// no longer simplifies (and shortens) things, just the opposite.

#define bit(x) (1 << (x))

#define is_set(v, i) ((v) & (i))

#endif