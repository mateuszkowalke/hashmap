
CC=gcc
CFLAGS=-g -Wall
LIB=libhashmap
BINS=$(LIB).so $(LIB).a test

all: $(BINS)

%: %.c $(LIB).a
	$(CC) $(CFLAGS) $< -o $@ $(LIB).a

%.o: %.c
	$(CC) $(CFLAGS) -c $<

lib%.a: %.o
	ar -cvrs $@ $<

lib%.so: %.o
	$(CC) $(CFLAGS) -shared -fPIC $< -o $@

clean:
	rm -rf %.so %.a %.o $(BINS)
