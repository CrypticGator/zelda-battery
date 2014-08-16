#include <stdio.h>
#include <stdlib.h>

#include "zbb.h"
#ifndef _ZB_ACBLINK
#define _ZB_ACBLINK 0 /* disabled by default */
#endif
#ifndef _ZB_NOBLINK
#define _ZB_NOBLINK 0 /* blinking is enabled by default */
#endif

int
main(void)
{
  struct power_t power;
  power = zbb__init();

  if (power.charge <= 3 && !_ZB_NOBLINK) {
    if (power.on_acpower)
      printf("%s", _ZB_ACBLINK ? "\033[5;31m" : "\033[0;31m");
  } else {
    printf("%s", "\033[0;31m");
  }
  return EXIT_SUCCESS;
}
