// ipmlib.h

#pragma once

#include <string>
#include <sstream>
#include <vector>

#define DIR_SEPARATOR char('\\')
#define ISO_EXT std::string(".iso")

using namespace System;

namespace ipmlib {

	public ref class ipm
	{
	public:
		ipm() { progress = 0; };
		~ipm() {};
		void createISO(System::String^ path);
		int getProgress() { return progress; }
	private:
		void slashReplace(std::string& path) {
			auto lastcharacter = path.end() - 1;
			if (*lastcharacter == DIR_SEPARATOR) {
				path.erase(lastcharacter);
			}
			auto it = find(path.begin(), path.end(), DIR_SEPARATOR);
			while (it != path.end()) {
				auto it2 = path.insert(it, DIR_SEPARATOR);
				it = find(it2 + 2, path.end(), DIR_SEPARATOR);
			}
		}
		std::string getISOName(std::string path) {
			std::stringstream ss(path);
			std::vector<std::string> elems;
			std::string item;
			while (std::getline(ss, item, DIR_SEPARATOR)) {
				elems.push_back(item);
			}
			auto size = elems.size();
			return elems.at(size - 1);
		}
		int progress;
	};
}
