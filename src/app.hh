#pragma once
#include "_components.hh"
#include "editor.hh"
#include "inputWindow.hh"
#include "alert.hh"
#include "config.hh"
#include "themes.hh"

class App {
	public:
		// variables
		bool                      run;
		bool                      wasInit;
		std::vector <std::string> args;
		EditorWindow              editorWindow;
		InputWindow               textbox;
		bool                      textboxFocused;
		uint8_t                   FPSLimit;
		Alert                     alert;
		AppConfig                 config;
		INI::Structure <char>     settings;
		Theme                     theme;
		cmrc::embedded_filesystem embeddedFS;

		// recording variables
		bool                   isRecording;
		bool                   isPlayingBack;
		std::vector <uint16_t> recordingData;
		size_t                 playBackIterator;
		
		// functions
		App(int argc, char** argv);
		void Update();
		void Render();
		void HandleInput(input_t input);
		void UpdateThemes();
		~App();

		// util functions
		void UpdateConfig();
		void SaveConfig();
};
