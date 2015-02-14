#ifndef _FILEFINDERINPUTMODEL_
#define _FILEFINDERINPUTMODEL_

#include <my/Signal.hpp>

#include <string>


class SearchCriteria {
public:
    my::Signal < SearchCriteria, std::string > signalSearchpathChanged;
public:
	SearchCriteria();
	~SearchCriteria();

	const char *getSearchPath() const;
	void setSearchPath( const char *path );

	const char *getFilePattern() const;
	void setFilePattern( const char * );

	const char *getSearchContent() const;
	void setSearchContent( const char * );

	bool includeSubDirs() const;
	void includeSubDirs( bool include );

	bool matchCase() const;
	void matchCase( bool match );

	bool matchWord() const;
	void matchWord( bool match );

	bool sortEntries() const;
	void sortEntries( bool sort );

	bool stopSearch;

private:
	std::string _searchPath;
	std::string _filePattern;
	std::string _searchContent;
	bool _includeSubDirs;
	bool _matchCase;
	bool _matchWord;
	bool _sortEntries;
};

#endif
