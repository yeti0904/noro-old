#pragma once
#include "_components.hh"

namespace Util {
	std::string StringReplaceAll(std::string src, std::string replaceWhat, 
		std::string replaceWith
	);
	bool                      IsNumber(std::string str);
	bool                      IsBool(std::string str);
	bool                      StringEndsWith(std::string src, std::string suffix);
	bool                      CharacterMassCompare(char ch, std::string compares);
	input_t                   FixInput(input_t input);
	std::vector <std::string> GetThemes();
	std::vector <std::string> GetRecordings();
}
