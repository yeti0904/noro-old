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
	bool                      IsWordChar(char ch);
	std::vector <std::string> GetThemes();
	std::vector <std::string> GetRecordings();
	size_t                    GetDigits(size_t number);
	std::string               BaseName(std::string path);
	std::string               CurrentTime();
}
