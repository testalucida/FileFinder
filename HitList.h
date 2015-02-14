#pragma once
#include <my/TableData.h>
#include <my/DirectoryIterator.h>

class HitList : public my::TableData {
public:
	HitList(void);
	~HitList(void);
	void addEntry( const char *pDir, const char *pName, const char *pLastWrite );
};

