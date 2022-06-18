#pragma once
#include "inputWindow.hh"

class App;

namespace InputEvents {
	void Init(App& p_app);
	void SaveAs(InputWindow& textbox);
	void Open(InputWindow& textbox);
	void Find(InputWindow& textbox);
	void Settings(InputWindow& textbox);
	void ChangeTheme(InputWindow& textbox);
	void ChangeTabSize(InputWindow& textbox);
}
