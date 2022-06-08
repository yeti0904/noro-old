#include "render.hh"
#include "editor.hh"
#include "colourpairs.hh"
#include "constants.hh"
#include "app.hh"

uint8_t tabSize;

void Renderers::Noro::Global(App& app) {
	// set global variables for use in window renderers
	tabSize = app.config.tabSize;
	
	// clear screen
	for (int i = 0; i < LINES; ++i) {
		mvhline(i, 0, 'c', COLS);
	}

	// render title/bottom bar
	attron(COLOR_PAIR(COLOUR_PAIR_TITLEBAR));

	mvhline(0, 0, ' ', COLS);
	move(0, 0);
	addstr(APP_NAME " " APP_VERSION);

	mvhline(LINES - 1, 0, ' ', COLS);
	move(LINES - 1, 0);
	if (app.editorWindow.maximised) {
		addstr(app.editorWindow.editors[app.editorWindow.tabIndex].title.c_str());
	}

	{
		std::string bottomBarInfo;
		bottomBarInfo = 
			std::to_string(app.editorWindow.GetCurrentEditor().cursorPosition.y) + ":"
			+ std::to_string(app.editorWindow.GetCurrentEditor().cursorPosition.x);

		// decide if bottom bar info should be rendered next to editor window title
		if (!app.editorWindow.maximised || (LINES - app.editorWindow.GetCurrentEditor().title.length() - 1 > bottomBarInfo.length())) {
			mvaddstr(LINES - 1, COLS - bottomBarInfo.length() - 1, bottomBarInfo.c_str());
		}
	}
	
	attroff(COLOR_PAIR(COLOUR_PAIR_TITLEBAR));
}

void Renderers::Noro::RenderEditorWindow(EditorWindow& editorWindow) {
	uint8_t maximised = editorWindow.maximised? 0 : 1;

	attron(COLOR_PAIR(COLOUR_PAIR_EDITOR));

	// render titlebar
	attron(A_REVERSE);

	mvhline(editorWindow.position.y, editorWindow.position.x, ' ', editorWindow.size.x);
	move(editorWindow.position.y, editorWindow.position.x);
	addstr(editorWindow.editors[editorWindow.tabIndex].title.c_str());

	attroff(A_REVERSE);

	// render editor
	Editor editor = editorWindow.editors[editorWindow.tabIndex];
	
	for (size_t i = editorWindow.position.y + maximised; i < editorWindow.size.y; ++i) {
		mvhline(i, editorWindow.position.x, ' ', editorWindow.size.x);
	}

	for (size_t i = editor.scroll.y; 
		(i - editor.scroll.y < (editorWindow.size.y - maximised - 1)) &&
		(i < editor.fileBuffer.size());
	++i) {
		move((editorWindow.position.y + i + maximised) - editor.scroll.y, editorWindow.position.x);
		for (size_t j = editor.scroll.x;
			(j - editor.scroll.x <= editor.fileBuffer[i].length()) &&
			(j - editor.scroll.x < editorWindow.size.x);
		++j) {
			if ((i == editor.cursorPosition.y) && (j == editor.cursorPosition.x)) {
				attron(A_REVERSE);
			}
			
			switch (editor.fileBuffer[i][j]) {
				case '\n':
				case '\r': {
					break;
				}
				case '\t': {
					for (uint8_t i = 0; i < tabSize; ++i) {
						addch(' ');
					}
					break;
				}
				case '\0': {
					addch(' ');
					break;
				}
				default: {
					addch(editor.fileBuffer[i][j]);
					break;
				}
			}
			
			if ((i == editor.cursorPosition.y) && (j == editor.cursorPosition.x)) {
				attroff(A_REVERSE);
			}
		}
	}

	attroff(COLOR_PAIR(COLOUR_PAIR_EDITOR));
	
}
