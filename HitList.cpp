#include "HitList.h"


HitList::HitList(void) {
	addColumn( "Treffer" );
	addColumn( "Geändert" );
}


HitList::~HitList(void) {
}

void HitList::addEntry( const char *pDir, const char *pName, const char *pLastWrite ) {
	int row = addRow();
	string s( pDir );
        s.append( "/" );
	s.append( pName );
	setValue( s.c_str(), row, 0 );
	setValue( pLastWrite, row, 1 );
}
