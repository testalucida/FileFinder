#pragma once

#include <string>
#include <functional>
#include <memory>

struct Entry {
public:
	std::string directory;
	std::string name;
	char lastWrite[30];
	unsigned long size;
	bool isDirectory;
	bool isReadOnly;
	bool isSystem;
	bool isHidden;
	bool isArchived;
};

typedef std::shared_ptr<Entry> EntryPtr;
typedef std::function<void ( const EntryPtr )> ON_HIT_CALLBACK;

class DirectoryIterator {
public:
	DirectoryIterator();
	DirectoryIterator( const std::string &startDir, bool includeSubDirs = true );
	~DirectoryIterator();

	void setHitCallback( ON_HIT_CALLBACK onHitCallback );
	void iterate();
	void stop() { _stop = true; }
private:
	DirectoryIterator( const std::string &startDir, bool includeSubDirs, bool *pStop );
	void iterate( const std::string &dir, const std::string & filePattern );
private:
	std::string _startDir;
	bool _includeSubDirs;
	ON_HIT_CALLBACK _onHitCallback;
	bool _stop;
	bool *_pMasterStop;
};

