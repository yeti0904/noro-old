#include "inputEvents.hh"
#include "app.hh"
#include "inputWindow.hh"
#include "constants.hh"
#include "util.hh"
#include "iohandle.hh"
#include "fs.hh"

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
	else if (textbox.userInput == "Toggle column highlight") {
		app->config.highlightColumn = !app->config.highlightColumn;
		if (app->config.highlightColumn) {
			textbox.resetVars = false;
			app->settings[INI::DefaultSection]["highlightColumn"] = "true";

			textbox.ResetVariables();
			textbox.title              = "Change column highlighted";
			textbox.content            = "Highlight column:";
			textbox.inputType          = InputType::Text;
			textbox.completionCallback = InputEvents::ChangeColumnHighlighted;
			textbox.complete           = false;
		}
		else {
			app->settings[INI::DefaultSection]["highlightColumn"] = "false";
			app->SaveConfig();
			IOHandle::Quit();
			app->UpdateConfig();
			app->alert.NewAlert("Disabled column highlighting", ALERT_TIMER);
		}
	}
	else if (textbox.userInput == "Change highlighted column") {
		textbox.resetVars = false;
		app->settings[INI::DefaultSection]["highlightColumn"] = "true";
		
		textbox.ResetVariables();
		textbox.title              = "Change column highlighted";
		textbox.content            = "Highlight column:";
		textbox.inputType          = InputType::Text;
		textbox.completionCallback = InputEvents::ChangeColumnHighlighted;
		textbox.complete           = false;
	}
	else if (textbox.userInput == "Toggle auto indent") {
		app->settings[INI::DefaultSection]["autoIndent"] =
			app->config.autoIndent? "false" : "true";
		app->SaveConfig();
		IOHandle::Quit();
		app->UpdateConfig();
		app->alert.NewAlert(
			std::string("Set auto indent to ") +
			(app->config.autoIndent? "true" : "false"), ALERT_TIMER
		);
	}
	else if (textbox.userInput == "Set indent type") {
		textbox.resetVars = false;

		textbox.ResetVariables();
		textbox.title              = "Set indent type";
		textbox.content            = "";
		textbox.inputType          = InputType::Selection;
		textbox.buttons            = {
			"Spaces",
			"Tabs"
		};
		textbox.completionCallback = InputEvents::ChangeIndentType;
		textbox.complete           = false;
	}
	else if (textbox.userInput == "Toggle line highlighting") {
		app->settings[INI::DefaultSection]["highlightLine"] =
			app->config.highlightLine? "false" : "true";
		app->SaveConfig();
		IOHandle::Quit();
		app->UpdateConfig();
		app->alert.NewAlert(
			std::string("Line highlight is now ") +
			(app->config.highlightLine? "on" : "off"), ALERT_TIMER
		);
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

void InputEvents::ChangeColumnHighlighted(InputWindow& textbox) {
	if (!Util::IsNumber(textbox.userInput)) {
		app->alert.NewAlert("Error: input not an integer", ALERT_TIMER);
		return;
	}
	app->settings[INI::DefaultSection]["highlightedColumn"] = textbox.userInput;
	app->SaveConfig();
	IOHandle::Quit();
	app->UpdateConfig();
	app->alert.NewAlert(
		"Changed highlighted column to " + textbox.userInput, ALERT_TIMER
	);
}

void InputEvents::ChangeIndentType(InputWindow& textbox) {
	app->settings[INI::DefaultSection]["spacesIndent"] =
		textbox.userInput == "Spaces"? "true" : "false";
	app->SaveConfig();
	IOHandle::Quit();
	app->UpdateConfig();
	app->alert.NewAlert("Changed indent type to " + textbox.userInput, ALERT_TIMER);
}

void InputEvents::RecordingMenu(InputWindow& textbox) {
	if (textbox.userInput == "Start recording") {
		app->isRecording = true;
		app->alert.NewAlert("Started recording", ALERT_TIMER);
	}
	else if (textbox.userInput == "Stop recording/playback") {
		if (app->isPlayingBack) {
			return;
		}
		if (app->isRecording) {
			app->alert.NewAlert("no", ALERT_TIMER);
		}
		textbox.resetVars = false;
		textbox.ResetVariables();
		textbox.title              = "Save recording";
		textbox.content            = "Save recording as: ";
		textbox.inputType          = InputType::Text;
		textbox.completionCallback = SaveRecording;
		textbox.complete           = false;
	}
	else if (textbox.userInput == "Play recording") {
		textbox.resetVars = false;
		textbox.ResetVariables();
		textbox.title              = "Play recording";
		textbox.buttons            = Util::GetRecordings();
		textbox.inputType          = InputType::Selection;
		textbox.completionCallback = LoadRecording;
		textbox.complete           = false;
	}
}

void InputEvents::SaveRecording(InputWindow& textbox) {
	// save recording data
	std::vector <uint8_t> rawData;
	for (auto& button : app->recordingData) {
		if ((int16_t) button == -1) {
			continue;
		}
		rawData.push_back(button & 0xFF00);
		rawData.push_back(button & 0xFF);
	}

	char* homeraw = getenv("HOME");
	if (homeraw == nullptr) {
		IOHandle::Quit();
		fprintf(stderr, "[ERROR] Failed to get home folder\n");
		exit(EXIT_FAILURE);
	}
	std::string home = homeraw;

	FS::File::Binary::Write(
		home + "/.config/noro/recordings/" + textbox.userInput,
		rawData
	);

	app->alert.NewAlert("Saved recording as " + textbox.userInput, ALERT_TIMER);
	app->isRecording = false;
	app->recordingData.clear();
}

void InputEvents::LoadRecording(InputWindow& textbox) {
	char* homeraw = getenv("HOME");
	if (homeraw == nullptr) {
		IOHandle::Quit();
		fprintf(stderr, "[ERROR] Failed to get home folder\n");
		exit(EXIT_FAILURE);
	}
	std::string home = homeraw;

	std::string recordingPath =
		home + "/.config/noro/recordings/" + textbox.userInput;

	if (!FS::File::Exists(recordingPath)) {
		app->alert.NewAlert("The recording has vanished, sorry", ALERT_TIMER);
		return;
	}

	std::vector <uint8_t> rawData = FS::File::Binary::Read(recordingPath);

	if (rawData.size() % 2 != 0) {
		app->alert.NewAlert("Invalid recording file", ALERT_TIMER);
		return;
	}

	std::vector <uint16_t> recording;
	for (size_t i = 0; i < rawData.size(); i += 2) {
		uint16_t key = rawData[i];
		key <<= 8;
		key |=  rawData[i + 1];
		recording.push_back(key);
	}

	app->isPlayingBack = true;
	app->recordingData = recording;
}

void InputEvents::SaveAfterQuit(InputWindow& textbox) {
	if ((textbox.userInput == "Y") || (textbox.userInput == "y")) {
		for (auto& editor : app->editorWindow.editors) {
			editor.SaveFile();
		}
	}
	app->run = false;
}
