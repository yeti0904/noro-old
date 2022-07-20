#pragma once
#include "_components.hh"
#include "structures.hh"
#include "config.hh"

class EditorWindow;

class Editor {
	public:
		// variables
		std::vector <std::string> fileBuffer;
		std::string               fileName;
		Vec2                      cursorPosition;
		Vec2                      selectionPosition;
		bool                      selected;
		Vec2                      scroll;
		bool                      saved;
		std::string               title;
		EditorWindow*             parent;
		std::string               clipboard;

		// functions
		Editor(std::string fname = "");
		void                      HandleInput(input_t input);
		void                      CursorUp();
		void                      CursorDown();
		void                      CursorLeft();
		void                      CursorRight();
		void                      UpdateScroll();
		void                      InsertText(std::string text);
		size_t                    CountIndents(size_t y);
		Vec2&                     SelectionStart();
		Vec2&                     SelectionEnd();
		void                      DeleteSelection();
		std::vector <std::string> SelectionContent();
		void                      Copy();
		void                      Paste();
		~Editor();

		// util functions
		void OpenFile(std::string fname);
		void SaveFile();
};

class EditorWindow {
	public:
		// variables
		std::vector <Editor> editors;
		size_t               tabIndex;
		Vec2                 position;
		Vec2                 size;
		bool                 maximised;
		AppConfig*           config;

		// functions
		EditorWindow();
		~EditorWindow();

		// util functions
		Editor& GetCurrentEditor();
};
