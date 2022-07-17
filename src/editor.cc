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
}

void Editor::HandleInput(input_t input) {
	bool moved = false;
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
			if (cursorPosition.x != 0) {
				-- cursorPosition.x;
				moved = true;
			}
			else if (cursorPosition.y > 0) {
				-- cursorPosition.y;
				cursorPosition.x = fileBuffer[cursorPosition.y].size();
				moved = true;
			}
			break;
		}
		case KEY_RIGHT: {
			if (cursorPosition.x < fileBuffer[cursorPosition.y].length()) {
				++ cursorPosition.x;
				moved = true;
			}
			else if (
				(cursorPosition.x >= fileBuffer[cursorPosition.y].length()) &&
				(cursorPosition.y != fileBuffer.size() - 1)
			) {
				++ cursorPosition.y;
				cursorPosition.x = 0;
				moved = true;
			}
			break;
		}
		case KEY_UP: {
			if (cursorPosition.y > 0) {
				-- cursorPosition.y;
				if (cursorPosition.x > fileBuffer[cursorPosition.y].size()) {
					cursorPosition.x = fileBuffer[cursorPosition.y].size();
				}
				moved = true;
			}
			else {
				cursorPosition.x = 0;
				moved = true;
			}
			break;
		}
		case KEY_DOWN: {
			if (cursorPosition.y < fileBuffer.size() - 1) {
				++ cursorPosition.y;
				if (cursorPosition.x > fileBuffer[cursorPosition.y].size()) {
					cursorPosition.x = fileBuffer[cursorPosition.y].size();
				}
				moved = true;
			}
			else {
				cursorPosition.x = fileBuffer[cursorPosition.y].size();
				moved = true;
			}
			break;
		}
		default: {
			if (((input >= ' ') && (input <= '~')) || (input == '\t')) {
				if (saved) {
					title = "Editor (" + fileName + ") +";
				}
				fileBuffer[cursorPosition.y].insert(
					cursorPosition.x, std::string(1, input)
				);
				++ cursorPosition.x;
				saved = false;
				moved = true;
			}
			break;
		}
	}

	if (moved) {
		if ((ssize_t) cursorPosition.x - (ssize_t) scroll.x < 0) {
			-- scroll.x;
			if (cursorPosition.x < scroll.x) {
				scroll.x = cursorPosition.x;
			}
		}
		else if (cursorPosition.x - scroll.x > (*parent).size.x - 1) {
			++ scroll.x;
		}
		if ((ssize_t) cursorPosition.y - (ssize_t) scroll.y < 0) {
			-- scroll.y;
		}
		else if (cursorPosition.y - scroll.y > (*parent).size.y - 3) {
			++ scroll.y;
		}
	}
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
