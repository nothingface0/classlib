#define __STDC_LIMIT_MACROS 1
#include <stdint.h>
#include <stdio.h>

#define STR(x) #x
#define DUMP(x) fprintf(out, "CLASSLIB_" #x " %s\n", STR(x))

int main (int, char **)
{
  FILE *out = fopen("check-limits.txt", "w");
  DUMP(INT8_MIN);
  DUMP(INT8_MAX);
  DUMP(UINT8_MAX);
  DUMP(INT16_MIN);
  DUMP(INT16_MAX);
  DUMP(UINT16_MAX);
  DUMP(INT32_MIN);
  DUMP(INT32_MAX);
  DUMP(UINT32_MAX);
  DUMP(INT64_MIN);
  DUMP(INT64_MAX);
  DUMP(UINT64_MAX);
  fclose(out);
  return 0;
}
