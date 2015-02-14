#include "SearchCriteria.h"

#include <my/FileHelper.h>

SearchCriteria::SearchCriteria(void)
    : stopSearch( false )
    , _filePattern( "*" )
	, _includeSubDirs( true )
	, _matchCase( true )
	, _matchWord( false )
	, _sortEntries( false )
{
	_searchPath.append( *(my::FileHelper::instance().getCurrentDirectory()) );
}


SearchCriteria::~SearchCriteria(void) {
}

const char *SearchCriteria::getSearchPath() const {
	return _searchPath.c_str();
}

void SearchCriteria::setSearchPath( const char *path ) {
	_searchPath.clear();
	_searchPath.append( path );
	signalSearchpathChanged.send( *this, _searchPath );
}

const char *SearchCriteria::getFilePattern() const {
	return _filePattern.c_str();
}

void SearchCriteria::setFilePattern( const char * pPattern ) {
	_filePattern = pPattern;
}

const char *SearchCriteria::getSearchContent() const {
	return _searchContent.c_str();
}

void SearchCriteria::setSearchContent( const char * pContent ) {
	_searchContent = pContent;
}

bool SearchCriteria::includeSubDirs() const {
	return _includeSubDirs;
}

void SearchCriteria::includeSubDirs( bool include ) {
	_includeSubDirs = include;
}


bool SearchCriteria::matchCase() const {
	return _matchCase;
}

void SearchCriteria::matchCase( bool match ) {
	_matchCase = match;
}


bool SearchCriteria::matchWord() const {
	return _matchWord;
}

void SearchCriteria::matchWord( bool match ) {
	_matchWord = match;
}

bool SearchCriteria::sortEntries() const {
	return _sortEntries;
}

void SearchCriteria::sortEntries( bool sort ) {
	_sortEntries = sort;
}

