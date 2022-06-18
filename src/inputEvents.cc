#include "inputEvents.hh"
#include "app.hh"
#include "inputWindow.hh"
#include "constants.hh"
#include "util.hh"
#include "iohandle.hh"

App* app;

void InputEvents::Init(App& p_app) {
	app = &p_app;
}

void InputEvents::SaveAs(InputWindow& textbox) {
	// this sucks
	app->editorWindow.GetCurrentEditor().fileName = textbox.userInput;
	app->editorWindow.GetCurrentEditor().SaveFile();
	app->alert.NewAlert("Saved " + textbox.userInput, ALERT_TIMER);
}

void InputEvents::Open(InputWindow& textbox) {
	// this will probably suck too (i havent written it yet)
	app->editorWindow.GetCurrentEditor().OpenFile(textbox.userInput);
	app->alert.NewAlert("Opened " + textbox.userInput, ALERT_TIMER);
	// ok it does
}

void InputEvents::Find(InputWindow& textbox) {
	for (size_t i = 
		app->editorWindow.GetCurrentEditor().cursorPosition.y;
		i < app->editorWindow.GetCurrentEditor().fileBuffer.size();
	++i) {
		std::string line = (*app).editorWindow.GetCurrentEditor().fileBuffer[i];
		if (line.find(textbox.userInput) != std::string::npos) {
			app->alert.NewAlert("Found text", ALERT_TIMER);
			app->editorWindow.GetCurrentEditor().cursorPosition.x =
				line.find(textbox.userInput);
			app->editorWindow.GetCurrentEditor().cursorPosition.y = i;
			return;
		}
	}
	(*app).alert.NewAlert("Not found", ALERT_TIMER);
}

void InputEvents::Settings(InputWindow& textbox) {
	if (textbox.userInput == "Change theme") {
		textbox.resetVars = false;

		textbox.title              = "Change theme";
		textbox.buttons            = Util::GetThemes();
		textbox.completionCallback = InputEvents::ChangeTheme;
		textbox.complete           = false;
	}
	else if (textbox.userInput == "Change tab size") {
		textbox.resetVars = false;

		textbox.ResetVariables();
		textbox.title              = "Change tab size";
		textbox.content            = "Set tab size to:";
		textbox.inputType          = InputType::Text;
		textbox.completionCallback = InputEvents::ChangeTabSize;
		textbox.complete           = false;
	}
}

void InputEvents::ChangeTheme(InputWindow& textbox) {
	app->settings[INI::DefaultSection]["theme"] = textbox.userInput;
	app->SaveConfig();
	IOHandle::Quit();
	app->UpdateConfig();
	app->alert.NewAlert("Changed theme to " + textbox.userInput, ALERT_TIMER);
}

void InputEvents::ChangeTabSize(InputWindow& textbox) {
	if (!Util::IsNumber(textbox.userInput)) {
		app->alert.NewAlert("Error: input not an integer", ALERT_TIMER);
		return;
	}
	app->settings[INI::DefaultSection]["tabSize"] = textbox.userInput;
	app->SaveConfig();
	IOHandle::Quit();
	app->UpdateConfig();
	app->alert.NewAlert("Changed tab size to " + textbox.userInput, ALERT_TIMER);
}
