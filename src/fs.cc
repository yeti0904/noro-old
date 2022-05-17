#include "fs.hh"

// File functions
std::string FS::File::Read(std::string fname) {
	std::ifstream fhnd(fname);
	std::string   ret;
	std::string   line;

	while (getline(fhnd, line)) {
		ret += line + '\n';
	}
	fhnd.close();

	ret.erase(ret.length() - 1, 1);

	return ret;
}

std::vector <std::string> FS::File::ReadIntoVector(std::string fname) {
	std::ifstream             fhnd(fname);
	std::vector <std::string> ret;
	std::string               line;

	while (getline(fhnd, line)) {
		ret.push_back(line);
	}
	fhnd.close();

	return ret;
}

bool FS::File::Exists(std::string fname) {
	std::ifstream fhnd(fname);

	if (fhnd.good()) {
		fhnd.close();
		return true;
	}

	return false;
}

void FS::File::Create(std::string fname) {
	std::ofstream fhnd(fname);
	fhnd.close();
}

void FS::File::Write(std::string fname, std::string write) {
	std::ofstream fhnd(fname);

	fhnd << write;

	fhnd.close();
}

// Directory functions
bool FS::Directory::Exists(std::string dname) {
	DIR* dhnd = opendir(dname.c_str());

	if (dhnd) {
		closedir(dhnd);
		return true;
	}

	return false;
}

void FS::Directory::Create(std::string dname) {
	mkdir(dname.c_str(), 0777);
}