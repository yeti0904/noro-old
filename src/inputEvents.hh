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
	void ChangeColumnHighlighted(InputWindow& textbox);
	void RecordingMenu(InputWindow& textbox);
	void SaveRecording(InputWindow& textbox);
	void LoadRecording(InputWindow& textbox);
	void SaveAfterQuit(InputWindow& textbox);
}
