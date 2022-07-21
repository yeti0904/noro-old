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

		// functions
		Theme();
		~Theme();

		// util functions
		void ConstructTheme(INI::Structure <char>& properties);
		int  StringToColour(std::string colour);
};
