#include "editor.hh"
#include "fs.hh"
#include "inputEvents.hh"
#include "util.hh"
#include "iohandle.hh"

// Editor class functions

Editor::Editor(std::string fname) {
	// set default values
	fileBuffer     = {""};
	fileName       = fname == ""? "Unnamed" : fname;
	cursorPosition = {0, 0};
	scroll         = {0, 0};
	title          = "Editor (" + fileName + ")";
	saved          = true;

	selection.textSelected = false;
	selection.start        = {0, 0};
	selection.end          = {0, 0};
}

void Editor::HandleInput(input_t input) {
	bool moved = false;

	if (
		(input != KEY_SLEFT) &&
		(input != KEY_SRIGHT) &&
		(input != KEY_SUP) &&
		(input != KEY_SDOWN)
	) {
		selection.textSelected = false;
	}
	
	switch (input) {
		case 0: {
			return;
		}
		case '\r':
		case '\n': {
			// insert if the cursor isnt at the end of the line
			if (cursorPosition.x < fileBuffer[cursorPosition.y].length() + 1) {
				fileBuffer.insert(
					fileBuffer.begin() + cursorPosition.y + 1,
					fileBuffer[cursorPosition.y].substr(cursorPosition.x)
				);
				fileBuffer[cursorPosition.y].erase(cursorPosition.x);
			}
			else {
				fileBuffer.insert(fileBuffer.begin() + cursorPosition.y + 1, "");
			}
			++ cursorPosition.y;
			cursorPosition.x = 0;
			moved = true;

			// do indentation
			if (parent->config->autoIndent && (cursorPosition.y > 0)) {
				size_t indents = CountIndents(cursorPosition.y - 1);
				char   indentChar = parent->config->spacesIndent? ' ' : '\t';
				fileBuffer[cursorPosition.y].insert(
					0, indents, indentChar
				);

				cursorPosition.x += indents;
			}
			break;
		}
		case 127: // also used for backspace key
		case KEY_BACKSPACE: {
			if (cursorPosition.x > 0) {
				fileBuffer[cursorPosition.y].erase(cursorPosition.x - 1, 1);
				-- cursorPosition.x;
			}
			else if (cursorPosition.y > 0) {
				fileBuffer[cursorPosition.y - 1] += fileBuffer[cursorPosition.y];
				size_t lineSize = fileBuffer[cursorPosition.y].length();
				fileBuffer.erase(fileBuffer.begin() + cursorPosition.y);
				-- cursorPosition.y;
				cursorPosition.x = fileBuffer[cursorPosition.y].size() - lineSize;
				moved = true;
			}
			break;
		}
		case KEY_LEFT: {
			CursorLeft();
			moved = true;
			break;
		}
		case KEY_RIGHT: {
			CursorRight();
			moved = true;
			break;
		}
		case KEY_UP: {
			CursorUp();
			moved = true;
			break;
		}
		case KEY_DOWN: {
			CursorDown();
			moved = true;
			break;
		}
		case KEY_SLEFT: {
			StartSelection();
			CursorLeft();
			selection.end = cursorPosition;
			CorrectSelection();
			break;
		}
		case KEY_SRIGHT: {
			StartSelection();
			CursorRight();
			selection.end = cursorPosition;
			CorrectSelection();
			break;
		}
		case KEY_SUP: {
			StartSelection();
			CursorUp();
			selection.end = cursorPosition;
			CorrectSelection();
			break;
		}
		case KEY_SDOWN: {
			StartSelection();
			CursorDown();
			selection.end = cursorPosition;
			CorrectSelection();
			break;
		}
		case '\t': {
			if (parent->config->spacesIndent) {
				fileBuffer[cursorPosition.y].insert(
					cursorPosition.x, std::string(parent->config->tabSize, ' ')
				);
				cursorPosition.x += parent->config->tabSize;
				break;
			}
		}
		// fall through
		default: {
			InsertText(std::string(1, input));
			moved = true;
			break;
		}
	}

	if (moved) {
		UpdateScroll();
	}
}

void Editor::UpdateScroll() {
	std::string line;
	size_t      lineExtendLength = 0;
	size_t      cursorX = cursorPosition.x;
	for (size_t j = 0; j < fileBuffer[cursorPosition.y].length(); ++j) {
		switch (fileBuffer[cursorPosition.y][j]) {
			case '\t': {
				if (j < cursorPosition.x) {
					cursorX += parent->config->tabSize - 1;
				}
				lineExtendLength += parent->config->tabSize - 1;
				line             += std::string(parent->config->tabSize, ' ');
				break;
			}
			default: {
				line += fileBuffer[cursorPosition.y][j];
			}
		}
	}
	if ((ssize_t) cursorX - (ssize_t) scroll.x < 0) {
		-- scroll.x;
		if (cursorX < scroll.x) {
			scroll.x = cursorX;
		}
	}
	else if (cursorX - scroll.x > parent->size.x - 1) {
		++ scroll.x;
		if (cursorX > scroll.x) {
			scroll.x = (cursorX - parent->size.x) + 1;
		}
	}
	if ((ssize_t) cursorPosition.y - (ssize_t) scroll.y < 0) {
		-- scroll.y;
	}
	else if (cursorPosition.y - scroll.y > parent->size.y - 3) {
		++ scroll.y;
	}
}

void Editor::CursorUp() {
	if (cursorPosition.y > 0) {
		-- cursorPosition.y;
		if (cursorPosition.x > fileBuffer[cursorPosition.y].size()) {
			cursorPosition.x = fileBuffer[cursorPosition.y].size();
		}
	}
	else {
		cursorPosition.x = 0;
	}
}

void Editor::CursorDown() {
	if (cursorPosition.y < fileBuffer.size() - 1) {
		++ cursorPosition.y;
		if (cursorPosition.x > fileBuffer[cursorPosition.y].size()) {
			cursorPosition.x = fileBuffer[cursorPosition.y].size();
		}
	}
	else {
		cursorPosition.x = fileBuffer[cursorPosition.y].size();
	}
}

void Editor::CursorLeft() {
	if (cursorPosition.x != 0) {
		-- cursorPosition.x;
	}
	else if (cursorPosition.y > 0) {
		-- cursorPosition.y;
		cursorPosition.x = fileBuffer[cursorPosition.y].size();
	}
}

void Editor::CursorRight() {
	if (cursorPosition.x < fileBuffer[cursorPosition.y].length()) {
		++ cursorPosition.x;
	}
	else if (
		(cursorPosition.x >= fileBuffer[cursorPosition.y].length()) &&
		(cursorPosition.y != fileBuffer.size() - 1)
	) {
		++ cursorPosition.y;
		cursorPosition.x = 0;
	}
}

void Editor::InsertText(std::string text) {
	for (auto& input : text) {
		if (((input >= ' ') && (input <= '~')) || (input == '\t')) {
			if (saved) {
				title = "Editor (" + fileName + ") +";
			}
			fileBuffer[cursorPosition.y].insert(
				cursorPosition.x, std::string(1, input)
			);
			++ cursorPosition.x;
			saved = false;
		}
	}
}

size_t Editor::CountIndents(size_t y) {
	/*size_t ret = 0;
	for (size_t i = 0; i < fileBuffer[y].length(); ++i) {
		if (fileBuffer[y][i] == '\t') {
			++ ret;
			continue;
		}
		break;
	}

	return ret;*/

	char indentCharacter = parent->config->spacesIndent? ' ' : '\t';

	size_t pos = fileBuffer[y].find_first_not_of(indentCharacter);
	return pos == std::string::npos? fileBuffer[y].length() : pos;
}

void Editor::StartSelection() {
	if (!selection.textSelected) {
		selection.textSelected = true;
		selection.start        = cursorPosition;
		selection.end          = cursorPosition;
	}
}

void Editor::CorrectSelection() {
	Selection corrected;
	corrected.start = selection.start;
	corrected.end   = selection.end;
	if (selection.start.x > selection.end.x) {
		corrected.end.x   = selection.start.x;
		corrected.start.x = selection.end.x;
	}
	if (selection.start.y > selection.end.y) {
		corrected.end.y   = selection.start.y;
		corrected.start.y = selection.end.y;
	}
	selection.start = corrected.start;
	selection.end   = corrected.end;
}

Editor::~Editor() {

}

void Editor::OpenFile(std::string fname) {
	char* home = getenv("HOME");
	if (home == NULL) {
		IOHandle::Quit();
		fprintf(stderr, "getenv(\"HOME\") failed");
		exit(1);
	}
	// fileName   = Util::StringReplaceAll(fname, "~", home);
	fileName       = fname;
	fileBuffer     = FS::File::ReadIntoVector(fileName);
	title          = "Editor (" + fileName + ")";
	cursorPosition = {0, 0};
	scroll         = {0, 0};
	if (fileBuffer.size() == 0) {
		fileBuffer = {""};
	}
}

void Editor::SaveFile() {
	std::string fileBufferString;
	for (size_t i = 0; i < fileBuffer.size(); ++i) {
		fileBufferString += fileBuffer[i] + '\n';
	}
	fileBufferString.erase(fileBufferString.length() - 1);
	// the last newline doesnt need to be there

	FS::File::Write(fileName, fileBufferString);

	saved = true;

	title = "Editor (" + fileName + ")";
}

// EditorWindow class functions

EditorWindow::EditorWindow() {
	// create new editor tab
	editors.push_back(Editor());
	editors.back().parent = this;
	tabIndex = 0;

	// set default values
	maximised = true;
	position  = {0, 1};
	size      = {20, 10};
}

Editor& EditorWindow::GetCurrentEditor() {
	return editors[tabIndex];
}

EditorWindow::~EditorWindow() {

}
