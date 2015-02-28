/*
 * FileFinder.h
 *
 *  Created on: 25.12.2011
 *      Author: max
 */
#ifndef FILEFINDER_H_
#define FILEFINDER_H_

#include "SearchCriteria.h"

#include <my/DirectoryIterator.h>
#include <my/Signal.hpp>

#include <string>
#include <vector>
#ifdef WIN32
#include <regex>
#else
#include <sys/types.h>
#include <regex.h>
#endif

typedef std::shared_ptr<my::Entry> EntryPtr;

struct SearchStat {
    int cntVisited;
    int cntMatch;
};

struct CanProceedParm {
    bool canProceed;
};

class FileFinder {
public:
    my::Signal < FileFinder, const EntryPtr > signalMatch;
    my::Signal < FileFinder, const SearchStat > signalTerminated;
    my::Signal < FileFinder, CanProceedParm > signalCanProceed;
public:
	/** Default ctor uses default SearchCriteria */
	FileFinder( SearchCriteria &searchCrit );
	/** start search */
	void start();
    /** stops searching */
    void stop();
	/** Dtor */
	virtual ~FileFinder();
private:
	void prepareSearch();
	/** sets a non-regex searchpattern */
	void setFilePattern( const char * );
	/** sets a non-regex searchpattern for content search */
	void setContentPattern( const char * );
    /** calls the registered ProgressCallback, if any */
    void reportProgress( const char *dir, const char *filename );
    void reportEnd();
	/** constructs a complete path and filename out of
	 * given separate path- and file name */
	const char* makeFullPath( const char *dir, const char *filename, char *fullPath );
	/** checks if the given file or directory name satisfies the defined pattern */
	bool matchesPattern( const std::string &text );
	/** checks if a file's content matches a given pattern */
	bool contentMatchesPattern( const my::EntryPtr pEntry );
	/** call listeners on each found entry */
	void reportEntry( const my::EntryPtr pEntry );
	/** if entries are to be reported in alphabetically order
	 *  they have to be collected and sorted after having
	 *  finished walking. */
	void rememberEntry( const my::EntryPtr pEntry );
	/** report collected and ordered entries */
	void reportSortedEntries();
	/** callback sort function for List::sort() */
	static int listsort( const my::Entry* const &d1, const my::Entry* const &d2 );
	/** Compiles a given non-regex file pattern e.g. m*n.cpp; *.h 
     *  Separate multiple patterns by ',' or ';'
     *  Wildcards ('*') are allowed
     */
	void compilePattern( const char *pattern );
	/** compiles a given non-regex content pattern */
	void compileContent( const char *pattern );
	/** destroys all heap allocated objects */
	void destroy();
	/** called by DirectoryIterator on each hit */
	void onHit( my::DirectoryIterator &, my::EntryFoundEvent & );
	/** called by DirectoryIterator when search is terminated */
	void onSearchTerminated( my::DirectoryIterator &, my::NULLPARM & );
private:
    std::string _user;
    std::string _home;
	SearchCriteria &_searchCrit;

    std::vector<EntryPtr> _entries;
    int _cntVisited;
    int _cntMatch;
    char _progressMsg[512];
#ifdef WIN32
	std::regex _filePattern;
	std::regex *_pContentPattern;
#else
    regex_t _filePattern;
    regex_t _contentPattern;
#endif
	my::DirectoryIterator *_pDirItr;
};

#endif /* FILEFINDER_H_ */
