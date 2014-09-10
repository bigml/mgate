LOCALBASE = ../
include $(LOCALBASE)Make.env
include $(BASEDIR)Make.env

UNAME := $(shell uname)

# prefix for installing the binaries
PREFIX=/usr/local
PREFIX_CONF=/etc/mevent

CFLAGS += -shared -fPIC

CFLAGS += -I$(LOCALBASE)mevent/server/daemon/ -I$(LOCALBASE)mevent/client/ $(INC_MOON) $(INC_LOCAL) -Iprivate

SOURCES = $(wildcard *.c)
LIB_PLUGIN = $(patsubst %.c, %.so, $(SOURCES))
OBJ_PLUGIN = $(patsubst %.c, %.o, $(SOURCES))

all: $(LIB_PLUGIN)

mevent_%.so: private/%_internal.c private/%_timer.c mevent_%.c
	@echo "$(CC) -o $@"
ifeq ($(UNAME), Darwin)
	@$(CC) $(CFLAGS) -c $< -o $(patsubst %.c, %.o, $<)
	@$(CC) -dynamiclib -undefined suppress -flat_namespace $(patsubst %.c, %.o, $<) -o $@
else
	@$(CC) $(CFLAGS) -o $@ $^
endif

install:
	install -d $(PREFIX)/lib
	install -d $(PREFIX_CONF)
	install -m 0644 $(LIB_PLUGIN) $(PREFIX)/lib/
	install -m 0644 server.hdf $(PREFIX_CONF)
	install -m 0644 client.hdf $(PREFIX_CONF)

uninstall:
	rm -f $(PREFIX)/lib/$(LIB_PLUGIN)
	rm -f $(PREFIX_CONF)/server.hdf
	rm -f $(PREFIX_CONF)/client.hdf

clean:
	rm -f $(LIB_PLUGIN)
	rm -f $(OBJ_PLUGIN)
