/*
 * See LICENSE file for copyright and license details.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <libgen.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

#include "arg.h"

char *argv0;

void
die(const char *errstr, ...) {
	va_list ap;

	va_start(ap, errstr);
	vfprintf(stderr, errstr, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}

void
usage(void) {
	die("usage: %s [-v] [-e <key>]\n", basename(argv0));
}

int
main(int argc, char *argv[]) {
	Display *dpy;
	char *key = NULL;
	KeySym keysym = NoSymbol;
	KeyCode keycode = 0x0;

	ARGBEGIN {
		case 'e':
			key = EARGF(usage());
			break;
		case 'v':
			die("xkev-"VERSION", © 2014 xkev engineers"
					", see LICENSE for details.\n");
		default:
			usage();
	} ARGEND;

	if (key == NULL)
		usage();

	if (!(dpy = XOpenDisplay(0)))
		die("Cannot open display.\n");

	if ((keysym = XStringToKeysym(key)) == NoSymbol)
		die("keysym doesn't exist: %s\n", key);
	if ((keycode = XKeysymToKeycode(dpy, keysym)) == 0)
		die("unable to find keycode for keysym: %s\n", key);

	XTestFakeKeyEvent(dpy, keycode, True, 0);
	XTestFakeKeyEvent(dpy, keycode, False, 0);

	XCloseDisplay(dpy);

	return 0;
}

