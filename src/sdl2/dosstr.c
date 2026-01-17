#include <stdlib.h>

extern char toupper(int ch);
extern char tolower(int ch);

char* strupr(char *n) {
      int i;
      for (i=0;n[i];i++) {
	       n[i] = toupper(n[i]);
      }
      return n;
}

char* strlwr(char *n) {
      int i;
      for (i=0;n[i];i++) {
	       n[i] = tolower(n[i]);
      }
      return n;
}
