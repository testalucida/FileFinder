/*
 * FileFinder.cpp
 *
 *  Created on: 25.12.2011
 *      Author: max
 */

#include "FileFinder.h"
#include "SearchCriteria.h"

#include <my/convert.h>
#include <my/StringHelper.h>

#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <cstdio>
#include <fstream>

#ifdef WIN32
#define SLASH "\\"
#else
#define SLASH "/"
#endif

using namespace std;
using namespace my;

FileFinder::FileFinder(SearchCriteria &searchCrit)
: _searchCrit(searchCrit)
//, _callCnt(0)
, _cntVisited(0)
, _cntMatch(0)
//, _stop(false)
, _pDirItr(NULL) {
}

//void FileFinder::setSearchCriteria( SearchCriteria &input ) {
//	_searchCrit = input;
//	prepareSearch();
//}

void FileFinder::prepareSearch() {
    _cntVisited = 0;
    _cntMatch = 0;
    setFilePattern( _searchCrit.getFilePattern( ) );
    const char *pContentPattern = _searchCrit.getSearchContent( );
    if( strlen( pContentPattern ) > 0 ) {
        setContentPattern( pContentPattern );
    }
}

void FileFinder::setFilePattern(const char *pPattern) {
    compilePath( pPattern );
}

void FileFinder::setContentPattern(const char *pPattern) {
    compileContent( pPattern );
}

void FileFinder::start() {
    prepareSearch( );
    DirectoryIterator itr( _searchCrit.getSearchPath( ), _searchCrit.includeSubDirs( ) );
    //remember this instance for an eventually call to stop():
    _pDirItr = &itr;
    _pDirItr->signalEntryFound.connect < FileFinder, &FileFinder::onHit > ( this );
    _pDirItr->signalTerminated.connect < FileFinder, &FileFinder::onSearchTerminated > ( this );
    _pDirItr->iterate( );

    if( _searchCrit.sortEntries( ) ) {
        reportSortedEntries( );
    }

    reportEnd( );

#ifndef WIN32
    regfree( &_filePattern );
    const char *pContent = _searchCrit.getSearchContent( );
    if( pContent && strlen( pContent ) > 0 ) {
        regfree( &_contentPattern );
    }
#endif
}

void FileFinder::stop() {
    if( _pDirItr ) {
        _pDirItr->stop( );
    }
}

void FileFinder::onHit(DirectoryIterator &, EntryFoundEvent &entryFound) {
    EntryPtr pEntry = entryFound.entryPtr;
    const char *searchContent = _searchCrit.getSearchContent( );
    bool hasSearchContent = ( searchContent && strlen( searchContent ) > 0 );
    if( pEntry->isDirectory && hasSearchContent ) {
        //it's a directory; can't have content, so it's not
        //interesting.
        return;
    }
    _cntVisited++;
    //	if( _onProgressCallback ) {
    //		reportProgress( pEntry->directory.c_str(), pEntry->name.c_str() );
    //	}

    if( ( pEntry->isDirectory && matchesPattern( pEntry->directory ) ) ||
            ( /*!pEntry->isDirectory &&*/ matchesPattern( pEntry->name ) ) ) {
        if( hasSearchContent ) {
            if( !contentMatchesPattern( pEntry ) ) {
                return;
            }
        }
        _cntMatch++;
        //if( _sortEntries ) {
        if( _searchCrit.sortEntries( ) ) {
            rememberEntry( pEntry );
        } else {
            reportEntry( pEntry );
        }
    }
}

void FileFinder::onSearchTerminated(DirectoryIterator &, NULLPARM &) {
    SearchStat stat = { _cntVisited, _cntMatch };
    signalTerminated.send( *this, stat );
}

FileFinder::~FileFinder() {
    destroy( );
}

void FileFinder::destroy() {
}

void FileFinder::reportProgress(const char *dir, const char *filename) {
    _cntVisited++;
    //if ( _onProgressCallback ) {
    strcpy( _progressMsg, dir );
    strcat( _progressMsg, "/" );
    strcat( _progressMsg, filename );
    //_onProgressCallback( _progressMsg );
    //}
}

void FileFinder::reportEnd() {
    string s( "Suche beendet. " );
    Convert::toString( _cntVisited, s );
    s.append( " Dateien/Verzeichnisse untersucht, " );
    Convert::toString( _cntMatch, s );
    s.append( " Treffer." );
    strcpy( _progressMsg, s.c_str( ) );
    //    if ( _onProgressCallback ) {
    //        _onProgressCallback( _progressMsg );
    //    }
    _cntVisited = 0;
    _cntMatch = 0;
}

bool FileFinder::matchesPattern(const string &text) {
    const char *pat = _searchCrit.getFilePattern( );
    if( !pat || strlen( pat ) == 0 ) return true; //no pattern specified

#ifdef WIN32
    const std::sregex_token_iterator end;

    for( std::sregex_token_iterator i( text.cbegin( ), text.cend( ), _filePattern );
            i != end; ++i ) {
        return true;
    }
#else
    regmatch_t matches[1]; //A list of the matches in the string (a list of 1)
    //Compare the string to the expression
    //regexec() returns 0 on match, otherwise REG_NOMATCH
    if( regexec( &_filePattern, text.c_str( ), 1, matches, 0 ) == 0 ) {
        //            printf( "\"%s\" matches characters %d - %d\n",
        //                    sz, matches[0].rm_so, matches[0].rm_eo );
        return true;
    }
#endif
    return false;
}

bool FileFinder::contentMatchesPattern(const EntryPtr pEntry) {
    string line;
    string text;
    string pathnfile( pEntry->directory );
    if( !StringHelper::instance().endsWithSlash( pathnfile ) ) {
        pathnfile.append( SLASH );
    }
    pathnfile.append( pEntry->name );
    ifstream myfile( pathnfile );
    if( myfile.is_open( ) ) {
        while( myfile.good( ) ) {
            getline( myfile, line );
            text.append( line );
        }
        myfile.close( );

#ifdef WIN32
        const std::sregex_token_iterator end;
        for( std::sregex_token_iterator i( text.cbegin( ), text.cend( ), *_pContentPattern );
                i != end; ++i ) {
            return true;
        }
#else
        regmatch_t matches[1]; //A list of the matches in the string (a list of 1)
        //Compare the string to the expression
        //regexec() returns 0 on match, otherwise REG_NOMATCH
        if( regexec( &_contentPattern, text.c_str( ), 1, matches, 0 ) == 0 ) {
            //            printf( "\"%s\" matches characters %d - %d\n",
            //                    sz, matches[0].rm_so, matches[0].rm_eo );
            return true;
        }
#endif
    }

    return false;
}

void FileFinder::reportEntry(const EntryPtr pEntry) {
    signalMatch.send( *this, pEntry );
}

void FileFinder::rememberEntry(const EntryPtr pEntry) {
    _entries.push_back( pEntry );
}

void FileFinder::reportSortedEntries() {
    std::sort( _entries.begin( ), _entries.end( ), [](const EntryPtr pEntry1, const EntryPtr pEntry2) {
        return strcmp( pEntry1->name.c_str( ), pEntry2->name.c_str( ) );
    } );
    for_each( _entries.begin( ), _entries.end( ), [&](const EntryPtr pEntry) {
        reportEntry( pEntry );
    } );
}

const char *FileFinder::makeFullPath(const char *dir, const char *filename, char *fullPath) {
    strcpy( fullPath, dir );
    strcat( fullPath, "/" );
    strcat( fullPath, filename );
    return fullPath;
}

int FileFinder::listsort(const Entry * const & e1, const Entry * const & e2) {
    return strcmp( e1->name.c_str( ), e2->name.c_str( ) );
}

void FileFinder::compilePath(const char *pattern) {
    string dest;
    char last = pattern[strlen( pattern ) - 1];
    bool endsWithWildCard = ( last == '*' );

    if( *pattern != '*' ) {
        dest.append( "^" );
    }
    for(; *pattern; ) {
        if( *pattern == '.' ) {
            dest.append( "\\." );
        } else if( *pattern == '*' ) {
            dest.append( "\\w*" );
        } else {
            char buf[2];
            buf[0] = *pattern;
            buf[1] = '\0';
            dest.append( buf );
        }
        pattern++;
    }
    if( !endsWithWildCard ) {
        dest.append( "$" );
    }

#ifdef WIN32
    _filePattern.assign( dest, std::regex::icase );
#else
    int rc = regcomp( &_filePattern, dest.c_str( ), REG_ICASE );
    if( rc != 0 ) throw ( "could not compile file pattern." );
#endif
}

void FileFinder::compileContent(const char *pattern) {
    string dest;

    if( _searchCrit.matchWord( ) ) {
        dest.append( "\\b" );
    }
    char buf[2];
    buf[1] = '\0';
    for( const char *p = pattern, *p2 = pattern + strlen( pattern ); p < p2; p++ ) {
        //treat metacharacters as literals:
        if( *p == '*' || *p == '.' || *p == '?' ||
                *p == '\\' || *p == '{' || *p == '}' ||
                *p == '[' || *p == ']' ||
                *p == '+' || *p == '-' || *p == ')' ||
                *p == '(' || *p == '^' || *p == '$' ) {
            dest.append( "\\" );
        }
        buf[0] = *p;
        dest.append( buf );
    }

    if( _searchCrit.matchWord( ) ) {
        dest.append( "\\b" );
    }

    if( !_searchCrit.matchCase() ) {
#ifdef WIN32
        _pContentPattern->assign( dest, std::regex::icase );
#else
        int rc = regcomp( &_contentPattern, dest.c_str(), REG_ICASE );
        if( rc != 0 ) throw ( "could not compile content pattern." );
#endif
    } else {
#ifdef WIN32
        _pContentPattern->assign( dest );
#else
        int rc = regcomp( &_contentPattern, dest.c_str(), 0 );
        if( rc != 0 ) throw ( "could not compile content pattern." );
#endif
    }
}
