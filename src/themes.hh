#pragma once
#include "_components.hh"

struct ColourPair {
	int fg, bg;
};
typedef int Colour;

class Theme {
	public:
		// variables
		ColourPair editor;
		ColourPair titlebar;
		ColourPair alert;
		ColourPair tabs;
		ColourPair activeTab;
		ColourPair columnHighlight;
		Colour     lineHighlight;
		ColourPair rulerNumber;
		ColourPair rulerCurrent;
		ColourPair rulerLine;
		ColourPair clock;

		// functions
		Theme();
		~Theme();

		// util functions
		bool ConstructTheme(INI::Structure <char>& properties);
		int  StringToColour(std::string colour);
};
