libhosts - A simple LD\_PRELOAD library
===============================

<pre>
[maycon@darkside libhosts]$ ./resolveip a.b.c.d
./resolveip: Unable to find hostid for 'a.b.c.d': host not found
</pre>

<pre>
[maycon@darkside libhosts]$ make
gcc -Wall -Wl,-soname,libhosts -shared -ldl -fPIC -o libhosts.o libhosts.c
[maycon@darkside libhosts]$ echo "a.b.c.d 8.8.8.8" >> ~/.libhosts/hosts
[maycon@darkside libhosts]$
</pre>

<pre>
[maycon@darkside libhosts]$ LD\_PRELOAD=./libhosts.o ./resolveip a.b.c.d
IP address of a.b.c.d is 8.8.8.8
</pre>

<pre>
[maycon@darkside libhosts]$ LIBHOSTS\_DEBUG=1 LD\_PRELOAD=./libhosts.o ./resolveip a.b.c.d
[+] libhosts: libhosts.c:30 gethostbyname(): First call
[+] libhosts: libhosts.c:34 gethostbyname(): looking for a.b.c.d
[+] libhosts: libhosts.c:45 gethostbyname(): read from local host a.b.c.d -> 8.8.8.8
[+] libhosts: libhosts.c:49 gethostbyname(): matched
IP address of a.b.c.d is 8.8.8.8
  
[maycon@darkside libhosts]$ LIBHOSTS\_DEBUG=1 LD\_PRELOAD=./libhosts.o ./resolveip a.b.c.d.e
[+] libhosts: libhosts.c:30 gethostbyname(): First call
[+] libhosts: libhosts.c:34 gethostbyname(): looking for a.b.c.d.e
[+] libhosts: libhosts.c:45 gethostbyname(): read from local host a.b.c.d -> 8.8.8.8
[+] libhosts: libhosts.c:65 gethostbyname(): Not found in local hosts file
./resolveip: Unable to find hostid for 'a.b.c.d.e': host not found
[maycon@darkside libhosts]$
</pre>
