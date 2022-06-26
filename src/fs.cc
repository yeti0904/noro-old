#include "fs.hh"
#include "iohandle.hh"

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

// Binary file functions
size_t FS::File::Binary::GetSize(std::string fname) {
	struct stat results;

	if (stat(fname.c_str(), &results) == 0) {
		return results.st_size;
	}
	else {
		IOHandle::Quit();
		perror("[ERROR] Error while calling stat()");
		exit(1);
	}
}

AllocatedPointer FS::File::Binary::Read(std::string fname) {
	std::ifstream    fhnd(fname, std::ios::binary);
	AllocatedPointer ret;

	ret.Alloc(GetSize(fname) + 1);

	fhnd.read((char*) ret.ptr, ret.size);

	fhnd.close();

	return ret;
}

void FS::File::Binary::Write(std::string fname, void* content, size_t size) {
	std::ofstream fhnd(fname, std::ios::binary);

	fhnd.write((const char*) content, size);

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
