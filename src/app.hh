#pragma once
#include "_components.hh"
#include "editor.hh"

class App {
	public:
		// variables
		bool                      run;
		bool                      wasInit;
		std::vector <std::string> args;
		EditorWindow              editorWindow;
		
		// functions
		App(int argc, char** argv);
		void Update();
		void Render();
		~App();
};
