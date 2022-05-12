#include "iohandle.hh"
#include "colourpairs.hh"

void IOHandle::Init() {
	// init ncurses screen
	initscr();
	raw();                 // stop signals being generated on some key presses
	noecho();              // key inputs wont appear on the screen
	keypad(stdscr, true);  // allow input from special keys
	nodelay(stdscr, true); // make getch non-blocking
	curs_set(0);           // disable terminal cursor
	start_color();
	use_default_colors();  // add colours defined by the terminal

	// init colour pairs
	init_pair(COLOUR_PAIR_EDITOR, COLOR_WHITE, COLOR_BLUE);
	init_pair(COLOUR_PAIR_TITLEBAR, COLOR_BLACK, COLOR_WHITE);
}

void IOHandle::Quit() {
	endwin();
}
