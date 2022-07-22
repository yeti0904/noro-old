#include "editor.hh"
#include "fs.hh"
#include "inputEvents.hh"
#include "util.hh"
#include "iohandle.hh"

// Editor class functions

Editor::Editor(std::string fname) {
	// set default values
	fileBuffer        = {""};
	fileName          = fname == ""? "Unnamed" : fname;
	cursorPosition    = {0, 0};
	selected          = false;
	selectionPosition = {0, 0};
	scroll            = {0, 0};
	title             = "Editor (" + fileName + ")";
	saved             = true;
}

void Editor::HandleInput(input_t input) {
	bool moved = false;
	switch (input) {
		case 0: {
			return;
		}
		case '\r':
		case '\n': {
			DeleteSelection();

			// insert if the cursor isnt at the end of the line
			InsertText("\n");

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
			if (selected) {
				DeleteSelection();
				break;
			}

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
			selected = false;

			CursorLeft();
			moved = true;
			break;
		}
		case KEY_RIGHT: {
			selected = false;

			CursorRight();
			moved = true;
			break;
		}
		case KEY_UP: {
			selected = false;

			CursorUp();
			moved = true;
			break;
		}
		case KEY_DOWN: {
			selected = false;

			CursorDown();
			moved = true;
			break;
		}
		case KEY_SLEFT: {
			if (!selected) {
				selected = true;
				selectionPosition = cursorPosition;
			}

			CursorLeft();
			moved = true;
			break;
		}
		case KEY_SRIGHT: {
			if (!selected) {
				selected = true;
				selectionPosition = cursorPosition;
			}

			CursorRight();
			moved = true;
			break;
		}
		case KEY_SUP: {
			if (!selected) {
				selected = true;
				selectionPosition = cursorPosition;
			}

			CursorUp();
			moved = true;
			break;
		}
		case KEY_SDOWN: {
			if (!selected) {
				selected = true;
				selectionPosition = cursorPosition;
			}

			CursorDown();
			moved = true;
			break;
		}
		case CTRL_LEFT: {
			CursorWordLeft();
			moved = true;
			break;
		}
		case CTRL_RIGHT: {
			CursorWordRight();
			moved = true;
			break;
		}
		case CTRL_SLEFT: {
			if (!selected) {
				selected = true;
				selectionPosition = cursorPosition;
			}

			CursorWordLeft();
			moved = true;
			break;
		}
		case CTRL_SRIGHT: {
			if (!selected) {
				selected = true;
				selectionPosition = cursorPosition;
			}

			CursorWordRight();
			moved = true;
			break;
		}
		case KEY_SHIFTTAB: {
			if (selected) {
				Vec2 start = SelectionStart();
				Vec2 end   = SelectionEnd();

				for (size_t i = start.y; i < end.y; ++i) {
					if (parent->config->spacesIndent) {
						if (
							fileBuffer[i].substr(0, parent->config->tabSize) ==
							std::string(parent->config->tabSize, ' ')
						) {
							fileBuffer[i].erase(0, parent->config->tabSize);
						}
					}
					else {
						if (fileBuffer[i][0] == '\t') {
							fileBuffer[i].erase(0, 1);
						}
					}
				}
				break;
			}
		}
		// fall through
		case '\t': {
			/*
			DeleteSelection();

			if (parent->config->spacesIndent) {
				fileBuffer[cursorPosition.y].insert(
					cursorPosition.x, std::string(parent->config->tabSize, ' ')
				);
				cursorPosition.x += parent->config->tabSize;
				break;
			}*/
			if (selected) {
				Vec2 start = SelectionStart();
				Vec2 end   = SelectionEnd();

				for (size_t i = start.y; i < end.y; ++i) {
					if (parent->config->spacesIndent) {
						fileBuffer[i].insert(0, parent->config->tabSize, ' ');
					}
					else {
						fileBuffer[i].insert(0, 1, '\t');
					}
				}
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
	while ((ssize_t) cursorX - (ssize_t) scroll.x < 0) {
		-- scroll.x;
		if (cursorX < scroll.x) {
			scroll.x = cursorX;
		}
	}
	while (cursorX - scroll.x > parent->size.x - 1) {
		++ scroll.x;
		if (cursorX > scroll.x) {
			scroll.x = (cursorX - parent->size.x) + 1;
		}
	}
	while ((ssize_t) cursorPosition.y - (ssize_t) scroll.y < 0) {
		-- scroll.y;
	}
	while (cursorPosition.y - scroll.y > parent->size.y - 3) {
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

void Editor::CursorWordLeft() {
	// based on code from LordOfTrident's pona
	CursorLeft();

	while (isspace(fileBuffer[cursorPosition.y][cursorPosition.x])) {
		if (cursorPosition.x == 0) {
			return;
		}
		CursorLeft();
	}

	CursorLeft();
	while (Util::IsWordChar(fileBuffer[cursorPosition.y][cursorPosition.x])) {
		if (cursorPosition.x == 0) {
			return;
		}
		if (Util::IsWordChar(fileBuffer[cursorPosition.y][cursorPosition.x])) {
			CursorLeft();
		}
	}

	CursorRight();
}

void Editor::CursorWordRight() {
	while (isspace(fileBuffer[cursorPosition.y][cursorPosition.x])) {
		CursorRight();
		if (cursorPosition.x >= fileBuffer[cursorPosition.y].length()) {
			return;
		}
	}

	do {
		if (cursorPosition.x >= fileBuffer[cursorPosition.y].length()) {
			CursorRight();

			return;
		}

		CursorRight();
	} while (Util::IsWordChar(fileBuffer[cursorPosition.y][cursorPosition.x]));
}

void Editor::InsertText(std::string text) {
	for (auto& input : text) {
		switch (input) {
			case '\n': {
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
				break;
			}
			default: {
				if (((input >= ' ') && (input <= '~')) || (input == '\t')) {
					DeleteSelection();
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

Vec2& Editor::SelectionStart() {
	if (cursorPosition.y > selectionPosition.y) {
		return selectionPosition;
	}
	else if (cursorPosition.y == selectionPosition.y) {
		if (cursorPosition.x > selectionPosition.x) {
			return selectionPosition;
		}
		else {
			return cursorPosition;
		}
	}
	else {
		return cursorPosition;
	}
}

Vec2& Editor::SelectionEnd() {
	return &SelectionStart() == &cursorPosition? selectionPosition : cursorPosition;
}

void Editor::DeleteSelection() {
	if (selected) {
		Vec2 selectionStart = SelectionStart();
		Vec2 selectionEnd   = SelectionEnd();

		if (selectionStart.y == selectionEnd.y) {
			std::string part1 = fileBuffer[cursorPosition.y].substr(0, selectionStart.x);
			std::string part2 = fileBuffer[cursorPosition.y].substr(selectionEnd.x);

			fileBuffer[cursorPosition.y] = part1 + part2;
			cursorPosition.x             = part1.length();
		}
		else {
			fileBuffer.erase(
				fileBuffer.begin() + selectionStart.y + 1,
				fileBuffer.begin() + selectionEnd.y
			);

			selectionEnd.y = selectionStart.y + 1;

			std::string part1 = fileBuffer[selectionStart.y].substr(0, selectionStart.x);
			std::string part2 = fileBuffer[selectionEnd.y].substr(selectionEnd.x);

			fileBuffer.erase(fileBuffer.begin() + selectionEnd.y);

			cursorPosition.y             = selectionStart.y;
			cursorPosition.x             = part1.length();
			fileBuffer[cursorPosition.y] = part1 + part2;
		}

		selected = false;
	}
}

std::vector <std::string> Editor::SelectionContent() {
	std::vector <std::string> content;
	Vec2&                     selectionStart = SelectionStart();
	Vec2&                     selectionEnd   = SelectionEnd();

	if (selectionStart.y == selectionEnd.y) {
		content.push_back(
			fileBuffer[selectionStart.y].substr(
				selectionStart.x, selectionEnd.x - selectionStart.x
			)
		);
	}
	else {
		content.push_back(fileBuffer[selectionStart.y].substr(selectionStart.x));

		for (size_t i = selectionStart.y; i < selectionEnd.y; ++ i) {
			content.push_back(fileBuffer[i]);
		}

		content.push_back(fileBuffer[selectionEnd.y].substr(0, selectionEnd.x));
	}

	return content;
}

std::string Editor::SelectionContentString() {
	std::string               ret;
	std::vector <std::string> selection = SelectionContent();
	
	for (auto& line : selection) {
		ret += line + '\n';
	}

	if (selection.size() > 0) {
		ret.erase(ret.length() - 1, 1);
	}

	return ret;
}

void Editor::Copy() {
	clipboard = SelectionContentString();
}

void Editor::Cut() {
	clipboard = SelectionContentString();
	DeleteSelection();
}

void Editor::Paste() {
	InsertText(clipboard);
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
