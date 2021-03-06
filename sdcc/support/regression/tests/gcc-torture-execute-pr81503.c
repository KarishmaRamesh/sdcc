/*
pr81503.c from the execute part of the gcc torture tests.
*/

#include <testfwk.h>

#include <stdint.h>

unsigned short a = 41461;
unsigned short b = 3419;
#if __SIZEOF_INT__ >= 4
int c = 0;

void foo() {
  if (a + b * ~(0 != 5))
    c = -~(b * ~(0 != 5)) + 2147483647;
}
#else
int32_t c = 0;

void foo() {
  if (a + b * ~((int32_t)(0 != 5)))
    c = -~(b * ~((int32_t)(0 != 5))) + 2147483647;
}
#endif

void
testTortureExecute (void) {
  foo();
  if (c != 2147476810)
    ASSERT (0);
  return;
}
