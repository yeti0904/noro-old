#include "themes.hh"
#include "iohandle.hh"

Theme::Theme() {
	editor   = {COLOR_BLACK, COLOR_BLACK};
	titlebar = {COLOR_BLACK, COLOR_BLACK};
	alert    = {COLOR_BLACK, COLOR_BLACK};
}

Theme::~Theme() {
	
}

void Theme::ConstructTheme(INI::Structure <char>& properties) {
	Theme();

	std::vector <std::string> requiredProperties = {
		"editorFG", "editorBG",
		"titlebarFG", "titlebarBG",
		"alertFG", "alertBG"
	};

	for (size_t i = 0; i < requiredProperties.size(); ++i) {
		if (!properties.Contains(INI::DefaultSection, requiredProperties[i])) {
			IOHandle::Quit();
			printf("[ERROR] '%s' property missing from theme configuration\n", requiredProperties[i].c_str());
			exit(1);
		}
	}

	editor.fg   = StringToColour(properties[INI::DefaultSection]["editorFG"]);
	editor.bg   = StringToColour(properties[INI::DefaultSection]["editorBG"]);
	
	titlebar.fg = StringToColour(properties[INI::DefaultSection]["titlebarFG"]);
	titlebar.bg = StringToColour(properties[INI::DefaultSection]["titlebarBG"]);
	
	alert.fg    = StringToColour(properties[INI::DefaultSection]["alertFG"]);
	alert.bg    = StringToColour(properties[INI::DefaultSection]["alertBG"]);
}

int Theme::StringToColour(std::string colour) {
	if (colour == "black") {
		return COLOR_BLACK;
	}
	if (colour == "red") {
		return COLOR_RED;
	}
	if (colour == "green") {
		return COLOR_GREEN;
	}
	if (colour == "yellow") {
		return COLOR_YELLOW;
	}
	if (colour == "blue") {
		return COLOR_BLUE;
	}
	if (colour == "magenta") {
		return COLOR_MAGENTA;
	}
	if (colour == "cyan") {
		return COLOR_CYAN;
	}
	if (colour == "white") {
		return COLOR_WHITE;
	}
	else return -1;
}