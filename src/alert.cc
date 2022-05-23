#include "alert.hh"
#include "colourpairs.hh"

Alert::Alert() {
	text   = "";
	active = false;
	timer  = 0;
}

Alert::~Alert() {
	
}

void Alert::NewAlert(std::string p_text, int16_t p_timer) {
	text   = p_text;
	active = true;
	timer  = p_timer;
}

void Alert::UpdateTimer(int16_t frameTime) {
	timer -= frameTime;
	if (timer <= 0) {
		active = false;
	}
}

void Alert::Render() {
	std::string alertText = "[ " + text + " ]";

	attron(COLOR_PAIR(COLOUR_PAIR_ALERT));

	mvaddstr(LINES / 2, (COLS / 2) - (alertText.length() / 2), alertText.c_str());

	attroff(COLOR_PAIR(COLOUR_PAIR_ALERT));
}
