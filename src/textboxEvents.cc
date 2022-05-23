#include "textboxEvents.hh"
#include "app.hh"
#include "textbox.hh"
#include "constants.hh"
App* app;

void TextboxEvents::Init(App& p_app) {
	app = &p_app;
}

void TextboxEvents::SaveAs(Textbox& textbox) {
	// this sucks
	(*app).editorWindow.editors[(*app).editorWindow.tabIndex].fileName = textbox.userInput;
	(*app).editorWindow.editors[(*app).editorWindow.tabIndex].SaveFile();
	(*app).alert.NewAlert("Saved " + textbox.userInput, ALERT_TIMER);
}

void TextboxEvents::Open(Textbox& textbox) {
	// this will probably suck too (i havent written it yet)
	(*app).editorWindow.editors[(*app).editorWindow.tabIndex].OpenFile(textbox.userInput);
	(*app).alert.NewAlert("Opened " + textbox.userInput, ALERT_TIMER);
	// ok it does
}
