#include "app.hh"
#include "iohandle.hh"
#include "constants.hh"
#include "render.hh"
#include "fs.hh"
#include "terminal.hh"

App::App(int argc, char** argv) {
	// set variables
	run     = true;
	wasInit = false;
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

	switch (input) {
		case CTRL('q'): {
			run = false;
			break;
		}
		case CTRL('e'): {
			editorWindow.maximised = !editorWindow.maximised;

			if (!editorWindow.maximised) {
				editorWindow.position = {1, 2};
				editorWindow.size     = {(size_t)COLS - 2, (size_t)LINES - 2};
			}
			
			break;
		}
		case CTRL('t'): {
			Terminal::Run();
			break;
		}
		default: {
			editorWindow.editors[editorWindow.tabIndex].HandleInput(input);
			break;
		}
	}

	Render();
	usleep(1000000/60);
}

void App::Render() {
	Renderers::Noro::Global();
	Renderers::Noro::RenderEditorWindow(editorWindow);
	refresh();
}

App::~App() {
	if (wasInit) {
		IOHandle::Quit();
	}
}
