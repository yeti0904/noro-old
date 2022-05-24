#pragma once
#include "textbox.hh"

class App;

namespace TextboxEvents {
	void Init(App& p_app);
	void SaveAs(Textbox& textbox);
	void Open(Textbox& textbox);
	void Find(Textbox& textbox);
}
