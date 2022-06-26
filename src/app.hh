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
		std::vector <input_t>     recordingData;
		bool                      isRecording;
		bool                      recordingPlayingBack;
		size_t                    recordingPlaybackIndex;
		
		// functions
		App(int argc, char** argv);
		void Update();
		void Render();
		~App();

		// util functions
		void UpdateConfig();
		void SaveConfig();
		void SaveRecording(std::string name);
		void PlayRecording(std::string name);
};
