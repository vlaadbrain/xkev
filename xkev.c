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
static KeySym mod[6]; /* Shift,Control,Meta,Alt,Super,Hyper */

#define LENGTH(x)               (sizeof x / sizeof x[0])

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
	die("usage: %s [-vsScCmMaAuUhH] [-e <key>]\n", basename(argv0));
}

void
holdmodifiers(Display *dpy, Bool press) {
	for (int i=0;i < LENGTH(mod); i++) {
		if (mod[i] != NoSymbol)
			XTestFakeKeyEvent(dpy, XKeysymToKeycode(dpy, mod[i]), press, 0);
	}
}

int
main(int argc, char *argv[]) {
	Display *dpy;
	char *key = NULL;
	KeySym keysym = NoSymbol;
	KeyCode keycode = 0x0;
	/* init modifiers */
	for(int i=0;i < LENGTH(mod); i++) mod[i] = NoSymbol;

	ARGBEGIN {
		case 'e':
			key = EARGF(usage());
			break;
		case 's':
			mod[0] = XK_Shift_L;
			break;
		case 'S':
			mod[0] = XK_Shift_R;
			break;
		case 'c':
			mod[1] = XK_Control_L;
			break;
		case 'C':
			mod[1] = XK_Control_R;
			break;
		case 'm':
			mod[2] = XK_Meta_L;
			break;
		case 'M':
			mod[2] = XK_Meta_R;
			break;
		case 'a':
			mod[3] = XK_Alt_L;
			break;
		case 'A':
			mod[3] = XK_Alt_R;
			break;
		case 'u':
			mod[4] = XK_Super_L;
			break;
		case 'U':
			mod[4] = XK_Super_R;
			break;
		case 'h':
			mod[5] = XK_Hyper_L;
			break;
		case 'H':
			mod[5] = XK_Hyper_R;
			break;
		case 'v':
			die("xkev-"VERSION", © 2014 xkev engineers"
					", see LICENSE for details.\n");
		default:
			usage();
	} ARGEND;

	if(key == NULL)
		usage();

	if(!(dpy = XOpenDisplay(0)))
		die("xkev: cannot open display.\n");

	if((keysym = XStringToKeysym(key)) == NoSymbol)
		die("xkev: keysym doesn't exist: %s\n", key);
	if((keycode = XKeysymToKeycode(dpy, keysym)) == 0)
		die("xkev: unable to find keycode for keysym: %s\n", key);

	holdmodifiers(dpy, True);

	XTestFakeKeyEvent(dpy, keycode, True, 0);
	XTestFakeKeyEvent(dpy, keycode, False, 0);

	holdmodifiers(dpy, False);

	XCloseDisplay(dpy);

	return 0;
}

