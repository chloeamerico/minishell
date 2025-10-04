#ifndef HD_HELPER_H
# define HD_HELPER_H

# include <sys/types.h>

# ifdef MSDBG
#  include <stdio.h>
#  include <unistd.h>
#  define DBG(...) printf(__VA_ARGS__)
# else
#  define DBG(...)
# endif

int   xclose(const char *where, int *fd);
int   xdup2(const char *where, int oldfd, int newfd);
int   xpipe(const char *where, int p[2]);
pid_t xfork(const char *where);

#endif
