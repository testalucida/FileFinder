#ifndef _HITLIST_H_
#define _HITLIST_H_

#include <my/TableData.h>
#include <my/DirectoryIterator.h>

#include <memory>

class HitList : public my::TableData {
public:
	HitList(void);
	~HitList(void);
	void addEntry( const char *pDir, const char *pName, const char *pLastWrite );
};

typedef std::shared_ptr<HitList> HitListPtr;
        
#endif