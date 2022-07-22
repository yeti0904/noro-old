#include "app.hh"
#include "iohandle.hh"
#include "constants.hh"
#include "render.hh"
#include "fs.hh"
#include "terminal.hh"
#include "inputEvents.hh"
#include "util.hh"

App::App(int argc, char** argv) {
	// set variables
	run                      = true;
	wasInit                  = false;
	textboxFocused           = false;
	textbox.size.x           = 30;
	textbox.size.y           = 5;
	FPSLimit                 = 60;
	config.tabSize           = 4;
	isRecording              = false;
	isPlayingBack            = false;
	playBackIterator         = 0;
	config.highlightedColumn = 0;
	
	for (int i = 0; i < argc; ++i) {
		args.push_back(argv[i]);
	}

	// loop through arguments
	if (args.size() > 1) {
		for (size_t i = 1; i < args.size(); ++i) {
			if (args[i][0] == '-') {
				if ((args[i] == "--version") || (args[i] == "-v")) {
					puts(APP_NAME " " APP_VERSION);
					run = false;
					return;
				}
			}
			else {
				if (!FS::File::Exists(args[i])) {
					fprintf(stderr, "[ERROR] %s file doesnt exist\n",
						args[i].c_str()
					);
					exit(1);
				}
				editorWindow.editors[editorWindow.tabIndex].OpenFile(args[i]);
			}
		}
	}

	UpdateConfig();

	editorWindow.config = &config;
	
	InputEvents::Init(*this);
	wasInit = true;
}

void App::Update() {
	// update editor windows' dimensions
	if (editorWindow.maximised) {
		editorWindow.position = {0, 1};
		editorWindow.size = {(size_t)COLS, (size_t)LINES - 1};
		// editorWindow.position = {3, 3};
		// editorWindow.size = {20, 10};
	}

	// input
	input_t               input;
	std::vector <input_t> inputs;
	if (isPlayingBack) {
		input = recordingData[playBackIterator];
		++ playBackIterator;
		if (playBackIterator >= recordingData.size()) {
			isPlayingBack = false;
			alert.NewAlert("Finished playing back", ALERT_TIMER);
		}
		inputs = {input};
	}
	else {
		input = 0;
		while (input != -1) {
			input = Util::FixInput(getch());
			inputs.push_back(input);
			if (isRecording) {
				recordingData.push_back(input);
			}
		}
	}

	for (auto& oneInput : inputs) {
		HandleInput(oneInput);
		if (!run) {
			return;
		}
	}

	if (alert.active) {
		alert.UpdateTimer(1000/FPSLimit);
	}

	Render();
	usleep(1000000/FPSLimit);
}

void App::Render() {
	Renderers::Noro::Global(*this, config);
	Renderers::Noro::RenderEditorWindow(editorWindow, config);
	if (alert.active) {
		alert.Render();
	}
	if (textboxFocused) {
		textbox.Render();
	}
	refresh();
}

void App::HandleInput(input_t input) {
	if (textboxFocused) {
		textbox.HandleInput(input);
		if (textbox.complete) {
			textboxFocused = false;
		}
	}
	else {
		switch (input) {
			case CTRL('q'): { // quit
				run = false;
				for (auto& editor : editorWindow.editors) {
					if (!editor.saved) {
						textboxFocused = true;
						textbox.CenterOnScreen();
						textbox.Init("Unsaved work", "Do you want to save it? (Y/N)");
						textbox.completionCallback = InputEvents::SaveAfterQuit;
						textbox.inputType          = InputType::KeyPress;
						run = true;
					}
				}
				break;
			}
			/*
			dead feature lies here
			case CTRL('e'): { // maximise/minimise
				editorWindow.maximised = !editorWindow.maximised;

				if (!editorWindow.maximised) {
					editorWindow.position = {1, 2};
					editorWindow.size     = {(size_t)COLS - 2, (size_t)LINES - 2};
				}
				
				break;
			}
			*/
			case CTRL('t'): { // suspend to terminal
				if (!isPlayingBack) {
					Terminal::Run();
				}
				break;
			}
			case CTRL('d'): { // save as
				textbox.CenterOnScreen();
				textboxFocused = true;
				textbox.Init("Save As", "Type a filename to save the file as");
				textbox.completionCallback = InputEvents::SaveAs;
				textbox.inputType          = InputType::Text;
				break;
			}
			case CTRL('s'): { // save
				editorWindow.editors[editorWindow.tabIndex].SaveFile();
				alert.NewAlert("Saved " +
					editorWindow.editors[editorWindow.tabIndex].fileName, 
					ALERT_TIMER
				);
				break;
			}
			case CTRL('o'): { // open
				textbox.CenterOnScreen();
				textboxFocused = true;
				textbox.Init("Open", "Type a filename to open");
				textbox.completionCallback = InputEvents::Open;
				textbox.inputType          = InputType::Text;
				break;
			}
			case CTRL('f'): { // find
				textbox.CenterOnScreen();
				textboxFocused = true;
				textbox.Init("Find", "Type the text you want to\nfind");
				textbox.completionCallback = InputEvents::Find;
				textbox.inputType          = InputType::Text;
				break;
			}
			case KEY_NPAGE: { // next tab
				if (editorWindow.tabIndex < editorWindow.editors.size() - 1) {
					++ editorWindow.tabIndex;
				}
				break;
			}
			case KEY_PPAGE: { // previous tab
				if (editorWindow.tabIndex > 0) {
					-- editorWindow.tabIndex;
				}
				break;
			}
			case CTRL('n'): { // new tab
				editorWindow.editors.push_back(Editor());
				editorWindow.editors.back().parent = &editorWindow;
				editorWindow.tabIndex = editorWindow.editors.size() - 1;
				break;
			}
			case CTRL('w'): { // close tab
				editorWindow.editors.erase(editorWindow.editors.begin() +
					editorWindow.tabIndex
				);
				if (editorWindow.editors.size() == 0) {
					run = false;
					return;
				}
				if (editorWindow.tabIndex >= editorWindow.editors.size()) {
					editorWindow.tabIndex = editorWindow.editors.size() - 1;
				}
				break;
			}
			case CTRL('y'): { // settings
				textboxFocused = true;
				textbox.CenterOnScreen();
				textbox.Init("Settings", "");
				textbox.completionCallback = InputEvents::Settings;
				textbox.inputType          = InputType::Selection;
				textbox.buttons            = {
					"Change theme",
					"Change tab size",
					"Toggle column highlight",
					"Change highlighted column",
					"Toggle auto indent",
					"Set indent type",
					"Toggle line highlighting"
				};
				break;
			}
			case CTRL('r'): { // reload
				IOHandle::Quit();
				UpdateConfig();
				alert.NewAlert("Reloaded", ALERT_TIMER);
				break;
			}
			case CTRL('p'): { // recording menu
				break; // disabled for now
				textboxFocused = true;
				textbox.CenterOnScreen();
				textbox.Init("Recording", "");
				textbox.completionCallback = InputEvents::RecordingMenu;
				textbox.inputType          = InputType::Selection;
				textbox.buttons            = {
					"Start recording",
					"Stop recording/playback",
					"Play recording"
				};
				break;
			}
			case CTRL('c'): { // copy
				editorWindow.GetCurrentEditor().Copy();
				alert.NewAlert("Copied selection", ALERT_TIMER);
				break;
			}
			case CTRL('x'): { // cut
				editorWindow.GetCurrentEditor().Cut();
				alert.NewAlert("Cut selection", ALERT_TIMER);
				break;
			}
			case CTRL('v'): { // paste
				editorWindow.GetCurrentEditor().Paste();
				alert.NewAlert("Pasted contents of clipboard", ALERT_TIMER);
				break;
			}
			default: {
				editorWindow.editors[editorWindow.tabIndex].HandleInput(input);
				break;
			}
		}
	}
}

App::~App() {
	if (wasInit) {
		IOHandle::Quit();
	}
}

void App::UpdateConfig() {
	// create files/folders in .config
	const char* homeraw = getenv("HOME");
	if (homeraw == nullptr) {
		fprintf(stderr, "[ERROR] could not get enviroment variable HOME\n");
		exit(1);
	}
	std::string home = homeraw;
	if (!FS::Directory::Exists(home + "/.config")) {
		FS::Directory::Create(home + "/.config");
	}
	if (!FS::Directory::Exists(home + "/.config/noro")) {
		FS::Directory::Create(home + "/.config/noro");
	}
	if (!FS::Directory::Exists(home + "/.config/noro/themes")) {
		FS::Directory::Create(home + "/.config/noro/themes");
	}
	if (!FS::Directory::Exists(home + "/.config/noro/recordings")) {
		FS::Directory::Create(home + "/.config/noro/recordings");
	}
	if (!FS::File::Exists(home + "/.config/noro/settings.ini")) {
		FS::File::Write(home + "/.config/noro/settings.ini",
			"# noro properties\n"
			"theme = noro\n"
			"tabSize = 4\n"
			"highlightColumn = false\n"
			"autoIndent = true\n"
			"spacesIndent = false\n"
			"highlightLine = true"
		);
	}
	if (!FS::File::Exists(home + "/.config/noro/themes/noro.ini")) {
		FS::File::Write(home + "/.config/noro/themes/noro.ini",
			"# noro theme\n"
			"editorFG = white\n"
			"editorBG = blue\n"
			"titlebarFG = black\n"
			"titlebarBG = white\n"
			"alertFG = black\n"
			"alertBG = green\n"
			"tabFG = black\n"
			"tabBG = cyan\n"
			"activeTabFG = cyan\n"
			"activeTabBG = black\n"
			"columnHighlightFG = black\n"
			"columnHighlightBG = cyan\n"
			"lineHighlight = blue"
		);
		
	}
	if (!FS::File::Exists(home + "/.config/noro/themes/dark.ini")) {
		FS::File::Write(home + "/.config/noro/themes/dark.ini",
			"# dark theme\n"
			"editorFG = white\n"
			"editorBG = black\n"
			"titlebarFG = black\n"
			"titlebarBG = cyan\n"
			"alertFG = black\n"
			"alertBG = green\n"
			"tabFG = black\n"
			"tabBG = green\n"
			"activeTabFG = green\n"
			"activeTabBG = black\n"
			"columnHighlightFG = black\n"
			"columnHighlightBG = green\n"
			"lineHighlight = green"
		);
	}
	if (!FS::File::Exists(home + "/.config/noro/themes/dark16.ini")) {
		FS::File::Write(home + "/.config/noro/themes/dark16.ini",
			"# dark16 theme\n"
			"editorFG = white\n"
			"editorBG = black\n"
			"titlebarFG = black\n"
			"titlebarBG = cyan\n"
			"alertFG = black\n"
			"alertBG = green\n"
			"tabFG = black\n"
			"tabBG = green\n"
			"activeTabFG = green\n"
			"activeTabBG = black\n"
			"columnHighlightFG = white\n"
			"columnHighlightBG = grey\n"
			"lineHighlight = grey"
		);
	}
	if (!FS::File::Exists(home + "/.config/noro/themes/mono.ini")) {
		FS::File::Write(home + "/.config/noro/themes/mono.ini",
			"# dark theme\n"
			"editorFG = white\n"
			"editorBG = black\n"
			"titlebarFG = black\n"
			"titlebarBG = white\n"
			"alertFG = black\n"
			"alertBG = white\n"
			"tabFG = black\n"
			"tabBG = white\n"
			"activeTabFG = white\n"
			"activeTabBG = black\n"
			"columnHighlightFG = black\n"
			"columnHighlightBG = white\n"
			"lineHighlight = black"
		);
	}
	if (!FS::File::Exists(home + "/.config/noro/themes/gruvy.ini")) {
		FS::File::Write(home + "/.config/noro/themes/gruvy.ini",
			"# gruvy theme <3\n"
			"# made by LordOfTrident\n"
			"editorFG = default\n"
			"editorBG = default\n"
			"titlebarFG = black\n"
			"titlebarBG = green\n"
			"alertFG = black\n"
			"alertBG = green\n"
			"tabFG = green\n"
			"tabBG = default\n"
			"activeTabFG = brightgreen\n"
			"activeTabBG = default\n"
			"columnHighlightFG = white\n"
			"columnHighlightBG = grey\n"
			"lineHighlight = grey"
		);
	}

	// set up config
	try {
		settings.Parse(FS::File::Read(std::string(home) + "/.config/noro/settings.ini"));
	}
	catch (const INI::ParserException& error) {
		fprintf(stderr,
			"[ERROR] settings.ini:%i : %s\n",
			(int)error.Line(),
			error.What().c_str()
		);
	}
	std::vector <std::string> requiredProperties = {
		"theme",
		"tabSize",
		"highlightColumn",
		"autoIndent",
		"spacesIndent",
		"highlightLine"
	};

	/*std::vector <std::string> props;
	settings.listProps(props);
	for (size_t i = 0; i < props.size(); ++i) {
		puts(props[i].c_str());
	}*/
	
	for (size_t i = 0; i < requiredProperties.size(); ++i) {
		if (!settings.Contains(INI::DefaultSection, requiredProperties[i])) {
			fprintf(stderr, "[ERROR] missing property in config file: %s\n", requiredProperties[i].c_str());
			exit(1);
		}
	}
	if (!Util::IsBool(settings[INI::DefaultSection]["highlightColumn"])) {
		fprintf(stderr, "[ERROR] property highlightColumn is not a valid boolean\n");
		exit(1);
	}
	config.highlightColumn = settings.AsBoolean(
		INI::DefaultSection, "highlightColumn"
	);
	if (config.highlightColumn) {
		if (!settings.Contains(INI::DefaultSection, "highlightedColumn")) {
			fprintf(stderr, "[ERROR] highlighted column enabled but value not given\n");
			exit(1);
		}
		config.highlightColumn   = true;
		config.highlightedColumn = (size_t) settings.AsInteger(
			INI::DefaultSection, "highlightedColumn"
		);
	}

	// handle config
	if (!Util::IsNumber(settings[INI::DefaultSection]["tabSize"])) {
		fputs("[ERROR] property tabSize is not an integer\n", stderr);
		exit(1);
	}
	config.tabSize = std::stoi(settings[INI::DefaultSection]["tabSize"]);
	if (!Util::IsBool(settings[INI::DefaultSection]["autoIndent"])) {
		fputs("[ERROR] property autoIndent is not a valid boolean\n", stderr);
		exit(1);
	}
	config.autoIndent = settings.AsBoolean(INI::DefaultSection, "autoIndent");
	if (!Util::IsBool(settings[INI::DefaultSection]["spacesIndent"])) {
		fputs("[ERROR] property spacesIndent is not a valid boolean", stderr);
		exit(1);
	}
	config.spacesIndent = settings.AsBoolean(INI::DefaultSection, "spacesIndent");
	if (!Util::IsBool(settings[INI::DefaultSection]["highlightLine"])) {
		fputs("[ERROR] property highlightLine is not a valid boolean", stderr);
		exit(1);
	}
	config.highlightLine = settings.AsBoolean(INI::DefaultSection, "highlightLine");

	INI::Structure <char> themeConfig;
	try {
		themeConfig.Parse(FS::File::Read(
			std::string(home) + "/.config/noro/themes/"
			+ settings[INI::DefaultSection]["theme"] + ".ini"
		));
	}
	catch (const INI::ParserException& error) {
		fprintf(stderr,
			"[ERROR] settings.ini:%i : %s\n",
			(int)error.Line(),
			error.What().c_str()
		);
	}
	theme.ConstructTheme(themeConfig);

	// call init functions
	IOHandle::Init();
	IOHandle::InitColours(theme);
}

void App::SaveConfig() {
	const char* homeraw = getenv("HOME");
	if (homeraw == nullptr) {
		alert.NewAlert("Failed to get home folder", ALERT_TIMER);
		return;
	}
	std::string home = homeraw;

	std::string properties =
		"# noro properties\n"
		"theme = " + settings[INI::DefaultSection]["theme"] +
		"\ntabSize = " + settings[INI::DefaultSection]["tabSize"] +
		"\nhighlightColumn = " + settings[INI::DefaultSection]["highlightColumn"] +
		"\nautoIndent = " + settings[INI::DefaultSection]["autoIndent"] +
		"\nspacesIndent = " + settings[INI::DefaultSection]["spacesIndent"] +
		"\nhighlightLine = " + settings[INI::DefaultSection]["highlightLine"];
		
	if (settings[INI::DefaultSection]["highlightColumn"] == "true") {
		properties +=
			"\nhighlightedColumn = " + settings[INI::DefaultSection]["highlightedColumn"];
	}
	
	FS::File::Write(home + "/.config/noro/settings.ini", properties);
}
