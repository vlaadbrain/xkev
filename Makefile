# See LICENSE file for copyright and license details.

include config.mk

SRC = xkev.c
OBJ = ${SRC:.c=.o}

all: options xkev

options:
	@echo xkev build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

${OBJ}: config.mk

xkev: xkev.o
	@echo CC -o $@
	@${CC} -o $@ xkev.o ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f xkev ${OBJ} xkev-${VERSION}.tar.gz

dist: clean
	@echo creating dist tarball
	@mkdir -p xkev-${VERSION}
	@cp -R LICENSE README Makefile config.mk \
		xkev.1 arg.h ${SRC} xkev-${VERSION}
	@tar -cf xkev-${VERSION}.tar xkev-${VERSION}
	@gzip xkev-${VERSION}.tar
	@rm -rf xkev-${VERSION}

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f xkev ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/xkev
	@echo installing manual page to ${DESTDIR}${MANPREFIX}/man1
	@mkdir -p ${DESTDIR}${MANPREFIX}/man1
	@sed "s/VERSION/${VERSION}/g" < xkev.1 > ${DESTDIR}${MANPREFIX}/man1/xkev.1
	@chmod 644 ${DESTDIR}${MANPREFIX}/man1/xkev.1

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/xkev
	@echo removing manual page from ${DESTDIR}${MANPREFIX}/man1
	@rm -f ${DESTDIR}${MANPREFIX}/man1/xkev.1

.PHONY: all options clean dist install uninstall
