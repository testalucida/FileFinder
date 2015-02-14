#include "DirectoryIterator.h"

#include <io.h>
#include <time.h>

using namespace std;

DirectoryIterator::DirectoryIterator() 
	: _includeSubDirs( true ),
	  _onHitCallback( NULL ),
	  _stop( false ),
	  _pMasterStop( &_stop )
{
}

DirectoryIterator::DirectoryIterator( const std::string &startDir, bool includeSubDirs )
	: _startDir( startDir ),
	  _includeSubDirs( includeSubDirs ),
	  _onHitCallback( NULL ),
	  _stop( false )
	  ,_pMasterStop( &_stop )
{
}

DirectoryIterator::DirectoryIterator( const std::string &startDir, bool includeSubDirs, bool *pStop )
	: _startDir( startDir ),
	  _includeSubDirs( includeSubDirs ),
	  _onHitCallback( NULL ),
	  _pMasterStop( pStop )
{
}

DirectoryIterator::~DirectoryIterator() {
}

void DirectoryIterator::setHitCallback( ON_HIT_CALLBACK onHitCallback ) {
	_onHitCallback = onHitCallback;
}

void DirectoryIterator::iterate() {
	if( !_startDir.empty() ) {
		unsigned int pos = _startDir.find_last_of( '\\' );
		if( pos < _startDir.size() - 1 ) {
			_startDir.append( "\\" );
		}
	} 
	//_startDir.append( "*.*" );
	iterate( _startDir, "*.*" );
}

void DirectoryIterator::iterate( const std::string &dir, const std::string & filePattern ) {
	if( *_pMasterStop ) {
		return;
	}

	struct _finddata_t c_file;
	intptr_t hFile;
	string searchFor = dir + filePattern;
	if( ( hFile = _findfirst( searchFor.c_str(), &c_file ) ) > -1L )  {
		do {
			if( _onHitCallback && 
				!strcmp( c_file.name, "." ) == 0 && 
				!strcmp( c_file.name, ".." ) == 0 ) 
			{
				EntryPtr pEntry( new Entry() );
				pEntry->directory = dir;
				pEntry->isArchived =  ( c_file.attrib & _A_ARCH )   ? true : false;
				pEntry->isDirectory =  ( c_file.attrib & _A_SUBDIR )   ? true : false;
				pEntry->isHidden =  ( c_file.attrib & _A_HIDDEN )   ? true : false;
				pEntry->isReadOnly =  ( c_file.attrib & _A_RDONLY )   ? true : false;
				pEntry->isSystem =  ( c_file.attrib & _A_SYSTEM )   ? true : false;
				pEntry->name = c_file.name;
				ctime_s( pEntry->lastWrite, _countof( pEntry->lastWrite ), &c_file.time_write );
				pEntry->size = c_file.size;
				_onHitCallback( pEntry );
				if( _includeSubDirs && pEntry->isDirectory ) {
					string subdir = dir;
					subdir.append( pEntry->name );
					DirectoryIterator itr( subdir, true, _pMasterStop );
					if( _onHitCallback ) {
						itr.setHitCallback( _onHitCallback );
					}
					itr.iterate();
				}
			}
		} while( _findnext( hFile, &c_file ) == 0 );

		_findclose( hFile );
	}
}