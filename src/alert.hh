#pragma once
#include "_components.hh"

class Alert {
	public:
		// variables
		std::string text;
		bool        active;
		int16_t     timer;

		// functions
		Alert();
		~Alert();
		void NewAlert(std::string p_text, int16_t p_timer);
		void UpdateTimer(int16_t frameTime); // frameTime must match same unit as timer
		void Render();
};
