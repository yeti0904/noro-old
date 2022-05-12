#include "render.hh"
#include "editor.hh"
#include "colourpairs.hh"
#include "constants.hh"

void Renderers::Noro::Global() {
	// clear screen
	for (int i = 0; i < LINES; ++i) {
		mvhline(i, 0, ' ', COLS);
	}

	// render title/bottom bar
	attron(COLOR_PAIR(COLOUR_PAIR_TITLEBAR));

	mvhline(0, 0, ' ', COLS);
	move(0, 0);
	addstr(APP_NAME " " APP_VERSION);

	mvhline(LINES - 1, 0, ' ', COLS);

	attroff(COLOR_PAIR(COLOUR_PAIR_TITLEBAR));
}

void Renderers::Noro::RenderEditorWindow(EditorWindow& editorWindow) {
	uint8_t maximised = editorWindow.maximised? 0 : 1;

	attron(COLOR_PAIR(COLOUR_PAIR_EDITOR));

	// render titlebar
	attron(A_REVERSE);

	mvhline(editorWindow.position.y, editorWindow.position.x, ' ', editorWindow.size.x);
	move(editorWindow.position.y, editorWindow.position.x);
	printw("Editor (%s)", editorWindow.editors[editorWindow.tabIndex].fileName.c_str());

	attroff(A_REVERSE);

	// render editor
	Editor editor = editorWindow.editors[editorWindow.tabIndex];
	
	for (size_t i = editorWindow.position.y + maximised; i < editorWindow.size.y; ++i) {
		mvhline(i, editorWindow.position.x, ' ', editorWindow.size.x);
	}

	for (size_t i = 0; (i < editor.fileBuffer.size()) && (i < editorWindow.size.y); ++i) {
		move(editorWindow.position.y + i + maximised, editorWindow.position.x);
		for (size_t j = 0; (j <= editor.fileBuffer[i].length()) && (j < editorWindow.size.x); ++j) {
			if ((i == editor.cursorPosition.y) && (j == editor.cursorPosition.x)) {
				attron(A_REVERSE);
			}
			
			switch (editor.fileBuffer[i][j]) {
				case '\n':
				case '\r': {
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
