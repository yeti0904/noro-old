#include "textbox.hh"
#include "colourpairs.hh"
#include "constants.hh"

Textbox::Textbox() {
	// init variables
	complete           = true;
	scrollX            = 0;
	cursorX            = 0;
	position           = {0, 0};
	size               = {15, 5};
	completionCallback = nullptr;
}

Textbox::~Textbox() {
	
}

void Textbox::Init(std::string p_title, std::string p_content) {
	Textbox();
	title            = p_title;
	content          = p_content;
	userInput        = "";
	complete         = false;
	historySelection = -1;
}

void Textbox::HandleInput(input_t input) {
	switch (input) {
		case CTRL('q'):
		case CTRL('c'): { // cancel
			complete           = true;
			code               = TextboxExitCode::Cancelled;
			content            = "";
			cursorX            = 0;
			scrollX            = 0;
			completionCallback = nullptr;
			userInput          = "";
			historySelection   = -1;
			break;
		}
		case KEY_LEFT: {
			if (cursorX > 0) {
				-- cursorX;
				if (scrollX > cursorX) {
					-- scrollX;
				}
			}
			break;
		}
		case KEY_RIGHT: {
			if (cursorX < userInput.length()) {
				++ cursorX;
				if (cursorX - scrollX > size.x - 2) {
					++ scrollX;
				}
			}
			break;
		}
		case KEY_UP: { // go up in history
			if (historySelection > 0) {
				-- historySelection;
			}
			else if (historySelection == -1) {
				historySelection = history.size() - 1;
			}
			userInput = history[historySelection];
			cursorX   = userInput.length();
			break;
		}
		case KEY_DOWN: { // go down in history
			if (historySelection == -1) {
				break;
			}
			else if ((size_t)historySelection < history.size() - 1) {
				++ historySelection;
				userInput = history[historySelection];
				cursorX   = userInput.length();
			}
			else {
				userInput = "";
				historySelection = -1;
			}
			break;
		}
		case 127: // also used for backspace key
		case KEY_BACKSPACE: {
			if (cursorX > 0) {
				userInput.erase(cursorX - 1, 1);
				-- cursorX;
				if (scrollX > cursorX) {
					-- scrollX;
				}
			}
			break;
		}
		case '\r':
		case '\n': { // confirm input
			complete           = true;
			code               = TextboxExitCode::Complete;

			// add input to input history
			history.push_back(userInput);
			if (history.size() > MAX_HISTORY) {
				history.erase(history.begin());
			}

			// call callback			
			if (completionCallback != nullptr) {
				completionCallback(*this);
			}

			// reset textbox
			content            = "";
			cursorX            = 0;
			scrollX            = 0;
			completionCallback = nullptr;
			userInput          = "";
			historySelection   = -1;
			break;
		}
		default: {
			if (((input >= ' ') && (input <= '~')) || (input == '\t')) {
				userInput.insert(cursorX, std::string(1, input));
				++ cursorX;
				if (cursorX - scrollX > size.x - 2) {
					++ scrollX;
				}
			}
			break;
		}
	}
}

void Textbox::Render() {
	attron(COLOR_PAIR(COLOUR_PAIR_TITLEBAR));

	for (size_t i = 0; i < size.y; ++i) {
		mvhline(position.y + i, position.x, ' ', size.x);
	}

	// render border
	mvaddch(position.y, position.x, ACS_ULCORNER); // corners
	mvaddch(position.y, position.x + size.x, ACS_URCORNER);
	mvaddch(position.y + size.y, position.x, ACS_LLCORNER);
	mvaddch(position.y + size.y, position.x + size.x, ACS_LRCORNER);
	mvvline(position.y + 1, position.x, ACS_VLINE, size.y - 1); // lines
	mvvline(position.y + 1, position.x + size.x, ACS_VLINE, size.y - 1);
	mvhline(position.y, position.x + 1, ACS_HLINE, size.x - 1);
	mvhline(position.y + size.y, position.x + 1, ACS_HLINE, size.x - 1);

	// render title
	mvaddstr(position.y, position.x + ((size.x / 2) - (title.length() - 2)), title.c_str());

	// render content
	move(position.y + 1, position.x + 1);
	{
		size_t lines = 0;
		size_t cols  = 0;
		for (size_t i = 0; i < content.length(); ++i) {
			switch (content[i]) {
				case '\r': break;
				case '\n': {
					++ lines;
					cols = 0;
					break;
				}
				default: {
					++ cols;
					if (cols < size.x - 2) {
						addch(content[i]);
					}
					break; 
				}
			}
			move(position.y + 1 + lines, position.x + 1 + cols);
		}
	}

	// render input
	attroff(COLOR_PAIR(COLOUR_PAIR_TITLEBAR));
	attron(COLOR_PAIR(COLOUR_PAIR_EDITOR));

	mvhline(position.y + size.y - 1, position.x + 1, ' ', size.x - 1);
	move(position.y + size.y - 1, position.x + 1);
	for (size_t i = scrollX; (i <= userInput.length()) && (i - scrollX < size.x - 2); ++i) {
		if (i - scrollX == cursorX) {
			attron(A_REVERSE);
		}

		switch (userInput[i - scrollX]) {
			case '\0': {
				addch(' ');
				break;
			}
			default: {
				addch(userInput[i - scrollX]);
				break;
			}
		}
		
		if (i - scrollX == cursorX) {
			attroff(A_REVERSE);
		}
	}

	attroff(COLOR_PAIR(COLOUR_PAIR_EDITOR));
}

void Textbox::CenterOnScreen() {
	position.x = (COLS / 2)  - (size.x / 2);
	position.y = (LINES / 2) - (size.y / 2);
}
