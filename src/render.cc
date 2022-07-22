#include "render.hh"
#include "editor.hh"
#include "colourpairs.hh"
#include "constants.hh"
#include "app.hh"

uint8_t tabSize;

void Renderers::Noro::Global(App& app, AppConfig&) {
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
			std::to_string(app.editorWindow.GetCurrentEditor().cursorPosition.y)
				+ ":"
			+ std::to_string(app.editorWindow.GetCurrentEditor().cursorPosition.x);

		// decide if bottom bar info should be rendered next to editor window title
		if (
			!app.editorWindow.maximised ||
			(LINES - app.editorWindow.GetCurrentEditor().title.length() - 1
				> bottomBarInfo.length()
			)
		) {
			mvaddstr(
				LINES - 1,
				COLS - bottomBarInfo.length() - 1,
				bottomBarInfo.c_str()
			);
		}
	}

	attroff(COLOR_PAIR(COLOUR_PAIR_TITLEBAR));
}

void Renderers::Noro::RenderEditorWindow(EditorWindow& editorWindow, AppConfig& config) {
	attron(COLOR_PAIR(COLOUR_PAIR_EDITOR));

	// render titlebar
	attron(A_REVERSE);

	mvhline(
		editorWindow.position.y, editorWindow.position.x, ' ', editorWindow.size.x
	);
	move(editorWindow.position.y, editorWindow.position.x);
	addstr(editorWindow.editors[editorWindow.tabIndex].title.c_str());

	attroff(A_REVERSE);

	// render editor
	Editor editor = editorWindow.editors[editorWindow.tabIndex];

	for (size_t i = editorWindow.position.y; i < editorWindow.size.y; ++i) {
		mvhline(i, editorWindow.position.x, ' ', editorWindow.size.x);
	}
	if (config.highlightColumn) {
		attron(COLOR_PAIR(COLOUR_PAIR_COLUMNHIGHLIGHT));
		mvvline(1, config.highlightedColumn - editor.scroll.x, ' ', LINES - 2);
		attroff(COLOR_PAIR(COLOUR_PAIR_COLUMNHIGHLIGHT));
	}

	bool inSelection = false;
	for (size_t i = editor.scroll.y;
		(i - editor.scroll.y < (editorWindow.size.y - 2)) &&
		(i < editor.fileBuffer.size());
	++i) {
		std::string line;
		size_t      lineExtendLength = 0;
		size_t      cursorX = editor.cursorPosition.x;
		for (size_t j = 0; j < editor.fileBuffer[i].length(); ++j) {
			switch (editor.fileBuffer[i][j]) {
				case '\t': {
					if (i == editor.cursorPosition.y && j < editor.cursorPosition.x) {
						cursorX += tabSize - 1;
					}
					lineExtendLength += tabSize - 1;
					line             += std::string(tabSize, ' ');
					break;
				}
				default: {
					line += editor.fileBuffer[i][j];
				}
			}
		}

		bool highlightLine = false;
		if (config.highlightLine && (i == editor.cursorPosition.y)) {
			attroff(A_REVERSE);
			attron(COLOR_PAIR(COLOUR_PAIR_LINEHIGHLIGHT));
			mvhline(
				(editorWindow.position.y + i) - editor.scroll.y,
				editorWindow.position.x,
				' ', editorWindow.size.x
			);
			highlightLine = true;
		}

		move((editorWindow.position.y + i) - editor.scroll.y, editorWindow.position.x);
		for (size_t j = editor.scroll.x;
			(j <= editor.fileBuffer[i].length() + lineExtendLength) &&
			(j - editor.scroll.x < editorWindow.size.x);
		++j) {
			Vec2 selectionStart = editor.SelectionStart();
			Vec2 selectionEnd   = editor.SelectionEnd();

			selectionStart.x +=
				std::count(
					editor.fileBuffer[i].begin(),
					editor.fileBuffer[i].begin() + selectionStart.x,
					'\t'
				) * (tabSize - 1);
			selectionEnd.x +=
				std::count(
					editor.fileBuffer[i].begin(),
					editor.fileBuffer[i].begin() + selectionEnd.x,
					'\t'
				) * (tabSize - 1);

			if (highlightLine) {
				if (inSelection) {
					attron(A_REVERSE);
				}
				attron(COLOR_PAIR(COLOUR_PAIR_LINEHIGHLIGHT));
			}
			else {
				if (inSelection) {
					attron(A_REVERSE);
				}
				attron(COLOR_PAIR(COLOUR_PAIR_EDITOR));
			}
			if (
				editor.selected &&
				!inSelection &&
				(i == selectionStart.y) &&
				(j == selectionStart.x)
			) {
				inSelection = true;
				if (highlightLine) {
					attroff(COLOR_PAIR(COLOUR_PAIR_LINEHIGHLIGHT));
					attron(COLOR_PAIR(COLOUR_PAIR_EDITOR));
				}
				attron(A_REVERSE);
			}
			if (
				inSelection &&
				(i == selectionEnd.y) &&
				(j == selectionEnd.x)
			) {
				inSelection = false;
				attroff(A_REVERSE);
				if (highlightLine) {
					attroff(COLOR_PAIR(COLOUR_PAIR_EDITOR));
					attron(COLOR_PAIR(COLOUR_PAIR_LINEHIGHLIGHT));
				}
			}

			if ((i == editor.cursorPosition.y) && (j == cursorX)) {
				attron(A_REVERSE);
			}
			else if (
				config.highlightColumn &&
				(j - editor.scroll.x == config.highlightedColumn)
			) {
				if (highlightLine) {
					attroff(COLOR_PAIR(COLOUR_PAIR_LINEHIGHLIGHT));
				}
				else {
					attroff(COLOR_PAIR(COLOUR_PAIR_EDITOR));
				}
				attron(COLOR_PAIR(COLOUR_PAIR_COLUMNHIGHLIGHT));
			}

			switch (line[j]) {
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
					addch(line[j]);
					break;
				}
			}

				if ((i == editor.cursorPosition.y) && (j == cursorX) && !inSelection) {
				attroff(A_REVERSE);
			}
			else if (
				config.highlightColumn &&
				(j - editor.scroll.x == config.highlightedColumn)
			) {
				attroff(COLOR_PAIR(COLOUR_PAIR_COLUMNHIGHLIGHT));
				if (highlightLine) {
					attron(COLOR_PAIR(COLOUR_PAIR_LINEHIGHLIGHT));
				}
				else {
					attron(COLOR_PAIR(COLOUR_PAIR_EDITOR));
				}
			}
		}
	}

	if (inSelection) {
		attroff(A_REVERSE);
	}

	attroff(COLOR_PAIR(COLOUR_PAIR_EDITOR));

	// render tab bar
	attron(COLOR_PAIR(COLOUR_PAIR_TABS));
	mvhline(
		editorWindow.position.y + editorWindow.size.y - 2,
		editorWindow.position.x,
		' ', editorWindow.size.x
	);
	move(
		editorWindow.position.y + editorWindow.size.y - 2,
		editorWindow.position.x
	);
	size_t x = 0;
	for (size_t i = 0; i < editorWindow.editors.size(); ++i) {
		// check if this tab is the tab that is in use
		if (i == editorWindow.tabIndex) {
			attroff(COLOR_PAIR(COLOUR_PAIR_TABS));
			attron(COLOR_PAIR(COLOUR_PAIR_ACTIVETAB));
		}
		for (size_t j = 0;
			j < editorWindow.editors[i].fileName.length();
		++j) {
			addch(editorWindow.editors[i].fileName[j]);
			++ x;
			if (x > editorWindow.size.x) {
				goto endTabBarRendering;
			}
		}
		if (i == editorWindow.tabIndex) {
			attroff(COLOR_PAIR(COLOUR_PAIR_ACTIVETAB));
			attron(COLOR_PAIR(COLOUR_PAIR_TABS));
		}
		++ x;
		addch(' ');
	}
	endTabBarRendering:
		attroff(COLOR_PAIR(COLOUR_PAIR_TABS));
}
