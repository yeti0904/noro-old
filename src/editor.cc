#include "editor.hh"
#include "fs.hh"

// Editor class functions

Editor::Editor(std::string fname) {
	// set default values
	fileBuffer     = {""};
	fileName       = fname == ""? "Unnamed" : fname;
	cursorPosition = {0, 0};
}

void Editor::HandleInput(input_t input) {
	switch (input) {
		case CTRL('s'): {
			std::string fileBufferString;
			for (size_t i = 0; i < fileBuffer.size(); ++i) {
				fileBufferString += fileBuffer[i] + '\n';
			}
			fileBufferString.erase(fileBufferString.length() - 1); // the last newline doesnt need to be there

			FS::File::Write(fileName, fileBufferString);

			saved = true;
			
			break;
		}
		case '\r':
		case '\n': {
			// insert if the cursor isnt at the end of the line
			if (cursorPosition.x < fileBuffer[cursorPosition.y].length() + 1) {
				fileBuffer.insert(fileBuffer.begin() + cursorPosition.y + 1, fileBuffer[cursorPosition.y].substr(cursorPosition.x));
				fileBuffer[cursorPosition.y].erase(cursorPosition.x);
			}
			else {
				fileBuffer.insert(fileBuffer.begin() + cursorPosition.y + 1, "");
			}
			++ cursorPosition.y;
			cursorPosition.x = 0;
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
				fileBuffer.erase(fileBuffer.begin() + cursorPosition.y);
				-- cursorPosition.y;
				cursorPosition.x = fileBuffer[cursorPosition.y].size(); 
			}
			break;
		}
		case KEY_LEFT: {
			if (cursorPosition.x != 0) {
				-- cursorPosition.x;
			}
			else if (cursorPosition.y > 0) {
				-- cursorPosition.y;
				cursorPosition.x = 0;
			}
			break;
		}
		case KEY_RIGHT: {
			if (cursorPosition.x < fileBuffer[cursorPosition.y].length() + 1) {
				++ cursorPosition.x;
			}
			else if (cursorPosition.y > fileBuffer.size() - 1) {
				++ cursorPosition.y;
				cursorPosition.x = 0;
			}
			break;
		}
		case KEY_UP: {
			if (cursorPosition.y > 0) {
				-- cursorPosition.y;
				if (cursorPosition.x > fileBuffer[cursorPosition.y].size()) {
					cursorPosition.x = fileBuffer[cursorPosition.y].size();
				}
			}
			else {
				cursorPosition.x = 0;
			}
			break;
		}
		case KEY_DOWN: {
			if (cursorPosition.y < fileBuffer.size() - 1) {
				++ cursorPosition.y;
				if (cursorPosition.x > fileBuffer[cursorPosition.y].size()) {
					cursorPosition.x = fileBuffer[cursorPosition.y].size();
				}
			}
			else {
				cursorPosition.x = fileBuffer[cursorPosition.y].size();
			}
			break;
		}
		default: {
			if (((input >= ' ') && (input <= '~')) || (input == '\t')) {
				fileBuffer[cursorPosition.y].insert(cursorPosition.x, std::string(1, input));
				++ cursorPosition.x;
				saved = false;
			}
			break;
		}
	}
}

Editor::~Editor() {
	
}

void Editor::OpenFile(std::string fname) {
	fileName = fname;
	fileBuffer = FS::File::ReadIntoVector(fname);
}

// EditorWindow class functions

EditorWindow::EditorWindow() {
	// create new editor tab
	editors.push_back(Editor());
	tabIndex = 0;

	// set default values
	maximised = true;
	position  = {0, 1};
	size      = {20, 10};
}

EditorWindow::~EditorWindow() {
	
}
