#include "themes.hh"
#include "iohandle.hh"

Theme::Theme() {
	editor          = {COLOR_BLACK, COLOR_BLACK};
	titlebar        = {COLOR_BLACK, COLOR_BLACK};
	alert           = {COLOR_BLACK, COLOR_BLACK};
	tabs            = {COLOR_BLACK, COLOR_BLACK};
	activeTab       = {COLOR_BLACK, COLOR_BLACK};
	columnHighlight = {COLOR_BLACK, COLOR_BLACK};
	rulerNumber     = {COLOR_BLACK, COLOR_BLACK};
	rulerCurrent    = {COLOR_BLACK, COLOR_BLACK};
	rulerLine       = {COLOR_BLACK, COLOR_BLACK};
	clock           = {COLOR_BLACK, COLOR_BLACK};
}

Theme::~Theme() {
	
}

bool Theme::ConstructTheme(INI::Structure <char>& properties) {
	Theme();

	std::vector <std::string> requiredProperties = {
		"editorFG",          "editorBG",
		"titlebarFG",        "titlebarBG",
		"alertFG",           "alertBG",
		"tabFG",             "tabBG",
		"activeTabFG",       "activeTabBG",
		"columnHighlightFG", "columnHighlightBG",
		"lineHighlight",
		"rulerNumberFG",     "rulerNumberBG",
		"rulerCurrentFG",    "rulerCurrentBG",
		"rulerLineFG",       "rulerLineBG",
		"clockFG",           "clockBG"
	};

	for (size_t i = 0; i < requiredProperties.size(); ++i) {
		if (!properties.Contains(INI::DefaultSection, requiredProperties[i])) {
			IOHandle::Quit();
			fprintf(
				stderr, "[ERROR] '%s' property missing from theme configuration\n",
				requiredProperties[i].c_str()
			);
			return false;
		}
	}

	editor.fg      = StringToColour(properties[INI::DefaultSection]["editorFG"]);
	editor.bg      = StringToColour(properties[INI::DefaultSection]["editorBG"]);
	
	titlebar.fg    = StringToColour(properties[INI::DefaultSection]["titlebarFG"]);
	titlebar.bg    = StringToColour(properties[INI::DefaultSection]["titlebarBG"]);
	
	alert.fg       = StringToColour(properties[INI::DefaultSection]["alertFG"]);
	alert.bg       = StringToColour(properties[INI::DefaultSection]["alertBG"]);

	tabs.fg        = StringToColour(properties[INI::DefaultSection]["tabFG"]);
	tabs.bg        = StringToColour(properties[INI::DefaultSection]["tabBG"]);

	activeTab.fg   = StringToColour(properties[INI::DefaultSection]["activeTabFG"]);
	activeTab.bg   = StringToColour(properties[INI::DefaultSection]["activeTabBG"]);

	lineHighlight  = StringToColour(properties[INI::DefaultSection]["lineHighlight"]);

	rulerNumber.fg = StringToColour(properties[INI::DefaultSection]["rulerNumberFG"]);
	rulerNumber.bg = StringToColour(properties[INI::DefaultSection]["rulerNumberBG"]);

	rulerCurrent.fg = StringToColour(properties[INI::DefaultSection]["rulerCurrentFG"]);
	rulerCurrent.bg = StringToColour(properties[INI::DefaultSection]["rulerCurrentBG"]);

	rulerLine.fg = StringToColour(properties[INI::DefaultSection]["rulerLineFG"]);
	rulerLine.bg = StringToColour(properties[INI::DefaultSection]["rulerLineBG"]);

	columnHighlight.fg = StringToColour(
		properties[INI::DefaultSection]["columnHighlightFG"]
	);
	columnHighlight.bg = StringToColour(
		properties[INI::DefaultSection]["columnHighlightBG"]
	);

	clock.fg = StringToColour(properties[INI::DefaultSection]["clockFG"]);
	clock.bg = StringToColour(properties[INI::DefaultSection]["clockBG"]);

	return true;
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
	// +16 colours
	if (colour == "grey") {
		return COLOR_GREY;
	}
	if (colour == "brightred") {
		return COLOR_BRIGHTRED;
	}
	if (colour == "brightgreen") {
		return COLOR_BRIGHTGREEN;
	}
	if (colour == "brightyellow") {
		return COLOR_BRIGHTYELLOW;
	}
	if (colour == "brightblue") {
		return COLOR_BRIGHTBLUE;
	}
	if (colour == "brightcyan") {
		return COLOR_BRIGHTCYAN;
	}
	if (colour == "brightwhite") {
		return COLOR_BRIGHTWHITE;
	}
	if (colour == "default") {
		return -1;
	}
	else return -1;
}
