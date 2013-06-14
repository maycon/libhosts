#define _GNU_SOURCE
#include <stdio.h>      // fopen, feof, fclose, perror
#include <stdlib.h>     // exit, free
#include <string.h>     // strcmp
#include <dlfcn.h>      // dlsym
#include <arpa/inet.h>  // inet_addr
#include <netdb.h>      // gethostbyaddr

#define LOCAL_HOSTS "/home/maycon/.libhosts/hosts"

typedef struct hostent*(gethostbyname_t)(const char *name);

static gethostbyname_t *sys_gethostbyname;

struct hostent *gethostbyname(const char *name)
{
    FILE *fd;
    in_addr_t addr;
    char *hostname, *ip;
    int resolved;

    if (!sys_gethostbyname)
        sys_gethostbyname = (gethostbyname_t *)dlsym(RTLD_NEXT, "gethostbyname");

    fd = fopen(LOCAL_HOSTS, "r");
    if (!fd) {
        perror("fopen");
        exit(-1);
    }

    resolved = 0;
    while (!feof(fd)) {
        fscanf(fd, "%as %as", &hostname, &ip);
        printf ("read [%s][%s]\n", hostname, ip);

        if (hostname && ip) {
            if (!strcmp(hostname, name)) {
                printf ("Match host %s with IP address %s.\n", hostname, ip);
                addr = inet_addr(ip);
                resolved = 1;
            }
        }

        if (hostname) free(hostname);
        if (ip) free(ip);

        if (resolved) {
            fclose(fd);
            return gethostbyaddr(&addr, sizeof(struct in_addr), AF_INET);
        }
    }

    fclose(fd);

    return sys_gethostbyname(name);
}


