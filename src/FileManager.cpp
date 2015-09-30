#include "FileManager.h"
#include <windows.h>
#include <utils\Log.h>
#include <assert.h>

bool Filesystem::mount(const char* directoryName) {
	WIN32_FIND_DATAA ffd;
	HANDLE hFind = FindFirstFileA(directoryName, &ffd);

	if (INVALID_HANDLE_VALUE == hFind) {
		LOGEC("Filesystem") << "Cannot mount directory '" << directoryName << "'";
		return false;
	}
	_directoryName = directoryName;
	_mounted = true;
	refresh();	
	return true;
}

void Filesystem::refresh() {
	assert(_mounted);
	std::string directory = _directoryName;
	WIN32_FIND_DATAA ffd;
	_files.clear();
	HANDLE hFind = INVALID_HANDLE_VALUE;
	if (directory.find_last_of("\\") != directory.length()) {
		directory.append("\\");
	}
	hFind = FindFirstFileA((directory + "*").c_str(), &ffd);
	if (INVALID_HANDLE_VALUE != hFind) {
		do {
			if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				FSFile f;
				sprintf_s(f.name,128,ffd.cFileName);
				_files.push_back(f);
			}
		} while (FindNextFileA(hFind, &ffd) != 0);
		FindClose(hFind);
	}
}

const FSFile& Filesystem::getFile(int index) const {
	assert(index >= 0 && index < _files.size());
	return _files[index];
}

int Filesystem::getAvailableLevels(int* levelArray) {
	int cnt = 0;
	char buffer[128];
	for (size_t i = 0; i < numFiles(); ++i) {
		const FSFile& f = getFile(i);
		strcpy(buffer, f.name + 1);
		levelArray[cnt++] = atoi(buffer);
	}
	return cnt;
}