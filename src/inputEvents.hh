#pragma once
#include "inputWindow.hh"

class App;

namespace InputEvents {
	void Init(App& p_app);

	// editor events
	void SaveAs(InputWindow& textbox);
	void Open(InputWindow& textbox);
	void Find(InputWindow& textbox);

	// settings menu
	void Settings(InputWindow& textbox);
	void ChangeTheme(InputWindow& textbox);
	void ChangeTabSize(InputWindow& textbox);

	// recording menu
	void Recording(InputWindow& textbox);
	void SaveRecording(InputWindow& textbox);
	void PlayRecording(InputWindow& textbox);
}
