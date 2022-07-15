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

	/*
	old code
	// init colour pairs
	init_pair(COLOUR_PAIR_EDITOR, COLOR_WHITE, COLOR_BLUE);
	init_pair(COLOUR_PAIR_TITLEBAR, COLOR_BLACK, COLOR_WHITE);
	init_pair(COLOUR_PAIR_ALERT, COLOR_BLACK, COLOR_GREEN);
	*/
}

void IOHandle::InitColours(Theme& theme) {
	init_pair(COLOUR_PAIR_EDITOR,    theme.editor.fg,    theme.editor.bg);
	init_pair(COLOUR_PAIR_TITLEBAR,  theme.titlebar.fg,  theme.titlebar.bg);
	init_pair(COLOUR_PAIR_ALERT,     theme.alert.fg,     theme.alert.bg);
	init_pair(COLOUR_PAIR_TABS,      theme.tabs.fg,      theme.tabs.bg);
	init_pair(COLOUR_PAIR_ACTIVETAB, theme.activeTab.fg, theme.activeTab.bg);
	init_pair(
		COLOUR_PAIR_COLUMNHIGHLIGHT, theme.columnHighlight.fg, theme.columnHighlight.bg
	);
}

void IOHandle::Quit() {
	endwin();
}
