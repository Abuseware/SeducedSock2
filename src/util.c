#include <stdint.h>

#include <util.h>


uint64_t strlen(char *str) {
  uint64_t len = 0;

  while(str[len] != '\0') {
    len++;
  }

  return len;
}

void reverse(char *s) {
  uint64_t i, j;
  char c;

  for(i = 0, j = strlen(s) - 1; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

void itoa(int64_t n, char s[]) {
  int64_t i, sign;

  if((sign = n) < 0) { /* record sign */
    n = -n;  /* make n positive */
  }

  i = 0;

  do {       /* generate digits in reverse order */
    s[i++] = n % 10 + '0';   /* get next digit */
  } while((n /= 10) > 0);     /* delete it */

  if(sign < 0) {
    s[i++] = '-';
  }

  s[i] = '\0';
  reverse(s);
}

void itoh(int64_t n, char s[]) {
  char vals[] = "0123456789abcdef";
  int64_t i, sign;

  if((sign = n) < 0) { /* record sign */
    n = -n;  /* make n positive */
  }

  i = 0;

  do {       /* generate digits in reverse order */
    s[i++] = vals[n % 16];   /* get next digit */
  } while((n /= 16) > 0);     /* delete it */

  if(sign < 0) {
    s[i++] = '-';
  }

  s[i++] = 'x';
  s[i++] = '0';
  s[i] = '\0';
  reverse(s);
}
