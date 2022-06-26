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
