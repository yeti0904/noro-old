#pragma once
#include "_components.hh"
#include "editor.hh"
#include "textbox.hh"
#include "alert.hh"
#include "config.hh"

class App {
	public:
		// variables
		bool                      run;
		bool                      wasInit;
		std::vector <std::string> args;
		EditorWindow              editorWindow;
		Textbox                   textbox;
		bool                      textboxFocused;
		uint8_t                   FPSLimit;
		Alert                     alert;
		AppConfig                 config;
		
		// functions
		App(int argc, char** argv);
		void Update();
		void Render();
		~App();
};
