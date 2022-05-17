#pragma once
#include "_components.hh"
#include "structures.hh"

class Editor {
	public:
		// variables
		std::vector <std::string> fileBuffer;
		std::string               fileName;
		Vec2                      cursorPosition;
		bool                      saved;

		// functions
		Editor(std::string fname = "");
		void HandleInput(input_t input);
		~Editor();
};

class EditorWindow {
	public:
		// variables
		std::vector <Editor> editors;
		size_t               tabIndex;
		Vec2                 position;
		Vec2                 size;
		bool                 maximised;

		// functions
		EditorWindow();
		~EditorWindow();
};
