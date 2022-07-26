#pragma once
#include "_components.hh"

namespace FS {
	namespace File {
		std::string               Read(std::string fname);
		std::vector <std::string> ReadIntoVector(std::string fname);
		bool                      Exists(std::string fname);
		void                      Create(std::string fname);
		void                      Write(std::string fname, std::string write);
		namespace Binary {
			std::vector <uint8_t> Read(std::string fname);
			void                  Write(
				std::string fname, std::vector <uint8_t> data
			);
			size_t                GetSize(std::string fname);
		}
	}
	namespace Directory {
		bool Exists(std::string dname);
		void Create(std::string dname);
		void Delete(std::string dname);
	}
}
