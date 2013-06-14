PROG=libhosts

all:
	gcc -Wall -Wl,-soname,libhosts -shared -ldl -fPIC -o $(PROG).o $(PROG).c


clean:
	rm -rf $(PROG).o
