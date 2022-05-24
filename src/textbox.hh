#pragma once
#include "_components.hh"
#include "structures.hh"

class Textbox;
typedef void (*TextboxCompletionEvent)(Textbox&);

enum class TextboxExitCode {
	Complete,
	Cancelled
};

class Textbox {
	public:
		// variables
		std::string               title;
		std::string               content;
		std::string               userInput;
		size_t                    cursorX;
		size_t                    scrollX;
		bool                      complete;
		TextboxExitCode           code;
		Vec2                      position;
		Vec2                      size;
		TextboxCompletionEvent    completionCallback;
		std::vector <std::string> history;
		ssize_t                   historySelection; // value -1 = nothing selected
		
		// functions
		Textbox();
		~Textbox();

		// util functions
		void Init(std::string p_title, std::string p_content);
		void HandleInput(input_t input);
		void Render();
		void CenterOnScreen();
};
