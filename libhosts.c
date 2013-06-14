#define _GNU_SOURCE
#include <stdio.h>      // fopen, feof, fclose, perror
#include <stdlib.h>     // exit, free
#include <string.h>     // strcmp
#include <dlfcn.h>      // dlsym
#include <arpa/inet.h>  // inet_addr
#include <netdb.h>      // gethostbyaddr


#define DEBUG(fmt, ...) \
    if (getenv("LIBHOSTS_DEBUG")) { \
        fprintf(stderr, "[+] libhosts: %s:%d %s(): "fmt"\n", \
                __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__); \
    }

#define LOCAL_HOSTS "~/.libhosts/hosts"

typedef struct hostent*(gethostbyname_t)(const char *name);

static gethostbyname_t *sys_gethostbyname;

struct hostent *gethostbyname(const char *name)
{
    FILE *fd;
    in_addr_t addr;
    char *hostname, *ip;
    int resolved;

    if (!sys_gethostbyname) {
        DEBUG("First call");
        sys_gethostbyname = (gethostbyname_t *)dlsym(RTLD_NEXT, "gethostbyname");
    }

    DEBUG("looking for %s", name);

    fd = fopen(getenv("LIBHOSTS_FILE"), "r");
    if (!fd) {
        perror("fopen");
        exit(-1);
    }

    resolved = 0;
    hostname = ip = NULL;
    while (fscanf(fd, "%as %as\n", &ip, &hostname) != EOF) {
        DEBUG("read from local host %s -> %s", hostname, ip);

        if (hostname && ip) {
            if (!strcmp(hostname, name)) {
                DEBUG ("matched");
                addr = inet_addr(ip);
                resolved = 1;
            }
        }

        if (hostname) free(hostname);
        if (ip) free(ip);
        hostname = ip = NULL;

        if (resolved) {
            fclose(fd);
            return gethostbyaddr(&addr, sizeof(struct in_addr), AF_INET);
        }
    }

    DEBUG("Not found in local hosts file");
    fclose(fd);

    return sys_gethostbyname(name);
}


