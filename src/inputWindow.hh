#pragma once
#include "_components.hh"
#include "structures.hh"

class InputWindow;
typedef void (*TextboxCompletionEvent)(InputWindow&);

enum class InputExitCode {
	Complete,
	Cancelled
};

enum class InputType {
	Text,
	Selection
};

class InputWindow {
	public:
		// variables
		std::string               title;
		std::string               content;
		std::string               userInput;
		bool                      complete;
		InputExitCode             code;
		Vec2                      position;
		Vec2                      size;
		TextboxCompletionEvent    completionCallback;
		std::vector <std::string> history;
		ssize_t                   historySelection; // value -1 = nothing selected
		InputType                 inputType;
		bool                      resetVars;

		// textbox-only
		size_t                    cursorX;
		size_t                    scrollX;

		// selection-only
		std::vector <std::string> buttons;
		size_t                    selectionScroll;
		size_t                    selectionSelected;
		
		// functions
		InputWindow();
		~InputWindow();

		// util functions
		void Init(std::string p_title, std::string p_content);
		void HandleInput(input_t input);
		void Render();
		void CenterOnScreen();
		void ResetVariables();
};
