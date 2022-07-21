#include "util.hh"

std::string Util::StringReplaceAll(std::string src, std::string replaceWhat, 
	std::string replaceWith
) {
	std::string ret = src;
	while (ret.find(replaceWhat) != std::string::npos) {
		ret.replace(ret.find(replaceWhat), replaceWhat.length(), replaceWith);
	}
	return ret;
}

bool Util::IsNumber(std::string str) {
	if (str == "-") {
		return false;
	}
	if (str == "") {
		return false;
	}
	for (size_t i = 0; i<str.length(); ++i) {
		if (!(
			((str[i] >= '0') && (str[i] <= '9')) ||
			((i == 0) && (str[i] == '-'))
		)) {
			return false;
		}
	}
	return true;
}

bool Util::IsBool(std::string str) {
	return (str == "true") || (str == "false");
}

bool Util::StringEndsWith(std::string src, std::string suffix) {
	// check if src ends with suffix
	if (src.length() < suffix.length()) {
		return false;
	}
	for (size_t i = 0; i < suffix.length(); i++) {
		if (src[src.length() - suffix.length() + i] != suffix[i]) {
			return false;
		}
	}
	return true;
}

bool Util::CharacterMassCompare(char ch, std::string compares) {
	for (auto& compare : compares) {
		if (compare == ch) {
			return true;
		}
	}
	return false;
}

input_t Util::FixInput(input_t input) {
	if (input == 27) {
		std::vector <input_t> sequence = {input};
		input = -1;
		do {
			input = getch();
			sequence.push_back(input);
		} while (input != -1);

		if (sequence.size() == 7) { // 7 because -1 included
			if (
				(sequence[1] == '[') && (sequence[2] == '1') && (sequence[3] == ';')
			) {
				switch (sequence[4]) {
					case '5': {
						// CTRL + arrow key
						switch (sequence[5]) {
							case 'A': {
								return CTRL_UP;
							}
							case 'B': {
								return CTRL_DOWN;
							}
							case 'C': {
								return CTRL_RIGHT;
							}
							case 'D': {
								return CTRL_LEFT;
							}
						}
					}
				}
			}
		}
	}
	return input;
}

bool Util::IsWordChar(char ch) {
	return (ch == '_') || isalnum(ch);
}

std::vector <std::string> Util::GetThemes() {
	std::vector <std::string> themes;
	DIR*                      dhnd;
	struct dirent*            dir;
	std::string               themedir = std::string(getenv("HOME")) +
		"/.config/noro/themes/";

	dhnd = opendir(themedir.c_str());

	if (dhnd) {
		while ((dir = readdir(dhnd)) != NULL) {
			if (StringEndsWith(dir->d_name, ".ini")) {
				themes.push_back(std::string(dir->d_name).substr(
					0, std::string(dir->d_name).find('.')
				));
			}
		}
	}

	return themes;
}

std::vector <std::string> Util::GetRecordings() {
	std::vector <std::string> recordings;
	DIR*                      dhnd;
	struct dirent*            dir;
	std::string               recordingsdir = std::string(getenv("HOME")) +
		"/.config/noro/recordings/";

	dhnd = opendir(recordingsdir.c_str());

	if (dhnd) {
		while ((dir = readdir(dhnd)) != NULL) {
			if (dir->d_type == DT_REG) {
				recordings.push_back(std::string(dir->d_name).substr(
					0, std::string(dir->d_name).find('.')
				));
			}
		}
	}

	return recordings;
}
