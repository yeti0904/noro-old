#include "app.hh"
#include "iohandle.hh"
#include "constants.hh"
#include "render.hh"
#include "fs.hh"
#include "terminal.hh"
#include "textboxEvents.hh"

App::App(int argc, char** argv) {
	// set variables
	run            = true;
	wasInit        = false;
	textboxFocused = false;
	textbox.size.x = 40;
	textbox.size.y = 5;
	FPSLimit       = 60;
	config.tabSize = 4;
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
					fprintf(stderr, "[ERROR] %s file doesnt exist\n", args[i].c_str());
					exit(1);
				}
				editorWindow.editors[editorWindow.tabIndex].OpenFile(args[i]);
			}
		}
	}

	// call init functions
	IOHandle::Init();
	TextboxEvents::Init(*this);
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
	input_t input = getch();

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
				break;
			}
			case CTRL('e'): { // maximise/minimise
				editorWindow.maximised = !editorWindow.maximised;

				if (!editorWindow.maximised) {
					editorWindow.position = {1, 2};
					editorWindow.size     = {(size_t)COLS - 2, (size_t)LINES - 2};
				}
				
				break;
			}
			case CTRL('t'): { // suspend to terminal
				Terminal::Run();
				break;
			}
			case CTRL('d'): { // save as
				textbox.CenterOnScreen();
				textboxFocused = true;
				textbox.Init("Save As", "Type a filename to save the file as");
				textbox.completionCallback = TextboxEvents::SaveAs;
				break;
			}
			case CTRL('s'): { // save
				editorWindow.editors[editorWindow.tabIndex].SaveFile();
				alert.NewAlert("Saved " + editorWindow.editors[editorWindow.tabIndex].fileName, ALERT_TIMER);
				break;
			}
			case CTRL('o'): { // open
				textbox.CenterOnScreen();
				textboxFocused = true;
				textbox.Init("Open", "Type a filename to open");
				textbox.completionCallback = TextboxEvents::Open;
				break;
			}
			case CTRL('f'): {
				textbox.CenterOnScreen();
				textboxFocused = true;
				textbox.Init("Find", "Type the text you want to find");
				textbox.completionCallback = TextboxEvents::Find;
				break;
			}
			default: {
				editorWindow.editors[editorWindow.tabIndex].HandleInput(input);
				break;
			}
		}
	}

	if (alert.active) {
		alert.UpdateTimer(1000/FPSLimit);
	}

	Render();
	usleep(1000000/FPSLimit);
}

void App::Render() {
	Renderers::Noro::Global(*this);
	Renderers::Noro::RenderEditorWindow(editorWindow);
	if (textboxFocused) {
		textbox.Render();
	}
	if (alert.active) {
		alert.Render();
	}
	refresh();
}

App::~App() {
	if (wasInit) {
		IOHandle::Quit();
	}
}
