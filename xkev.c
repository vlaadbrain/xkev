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
#include <X11/keysym.h>
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
	die("usage: %s [-vcC] [-e <key>]\n", basename(argv0));
}

int
main(int argc, char *argv[]) {
	Display *dpy;
	char *key = NULL;
	KeySym control = NoSymbol;

	KeySym keysym = NoSymbol;
	KeyCode keycode = 0x0;

	ARGBEGIN {
		case 'e':
			key = EARGF(usage());
			break;
		case 'c':
			control = XK_Control_L;
			break;
		case 'C':
			control = XK_Control_R;
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

	if (control != NoSymbol) {
		XTestFakeKeyEvent(dpy, XKeysymToKeycode(dpy, control), True, 0);
	}

	XTestFakeKeyEvent(dpy, keycode, True, 0);
	XTestFakeKeyEvent(dpy, keycode, False, 0);

	if (control != NoSymbol) {
		XTestFakeKeyEvent(dpy, XKeysymToKeycode(dpy, control), False, 0);
	}

	XCloseDisplay(dpy);

	return 0;
}

