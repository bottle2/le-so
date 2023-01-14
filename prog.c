#include "prog.h"

#define PRE
#define POS
int const *prog_images[] = {
#include "prog.inc"
};
#undef PRE
#undef POS

#define PRE sizeof
#define POS / sizeof (int)
int const prog_sizes[] = {
#include "prog.inc"
};
#undef PRE
#undef POS

int const n_prog = sizeof (prog_images) / sizeof (*prog_images);
