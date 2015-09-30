#pragma once
#include <vector>

struct FSFile {

	int size;
	int created;
	char name[128];
};


class Filesystem {

typedef std::vector<FSFile> Files;

public:
	Filesystem() : _mounted(false) {}
	~Filesystem() {}
	bool mount(const char* directoryName);
	void refresh();
	const size_t numFiles() const {
		return _files.size();
	}
	const FSFile& getFile(int index) const;
	int getAvailableLevels(int* levelArray);
private:
	Filesystem(const Filesystem& other) {}
	const char* _directoryName;
	bool _mounted;
	Files _files;
};

