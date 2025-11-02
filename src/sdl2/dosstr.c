#include <stdlib.h>

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
