#include "util.hh"

std::string Util::StringReplaceAll(std::string src, std::string replaceWhat, std::string replaceWith) {
	std::string ret = src;
	while (ret.find(replaceWhat) != std::string::npos) {
		ret.replace(ret.find(replaceWhat), replaceWhat.length(), replaceWith);
	}
	return ret;
}
