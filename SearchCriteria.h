#ifndef _FILEFINDERINPUTMODEL_
#define _FILEFINDERINPUTMODEL_

#include <my/CharBuffer.h>

#include <string>
#include <memory>

struct SearchCriteria {
	my::CharBuffer searchPath;
	my::CharBuffer filePattern;
	my::CharBuffer searchContent;
	bool includeSubDirs;
	bool matchCase;
	bool matchWord;
	bool sortEntries;
};

typedef std::shared_ptr<SearchCriteria> SearchCriteriaPtr;
#endif
