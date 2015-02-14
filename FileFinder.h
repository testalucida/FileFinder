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


//typedef std::function<void( const char* msg )> ONPROGRESS_CALLBACK;
//typedef std::function<void( const EntryPtr entry )> ONMATCH_CALLBACK;
//typedef std::function<void( const char *pDir )> ONDIRECTORYFOUND_CALLBACK;

typedef std::shared_ptr<my::Entry> EntryPtr;

struct SearchStat {
    int cntVisited;
    int cntMatch;
};

class FileFinder {
public:
    //my::Signal < FileFinder, std::string > signalProgress;
    my::Signal < FileFinder, const EntryPtr > signalMatch;
    my::Signal < FileFinder, const SearchStat > signalTerminated;
public:
	/** Default ctor uses default SearchCriteria */
	FileFinder( SearchCriteria &searchCrit );

	/** sets the SearchCriteria
		containing all conditions for the next search */
	//void setSearchCriteria( SearchCriteria &model );
	/** start search */
	void start();
	/** set a callback function called whenever the next
	    entry is examined */
    //void setProgressCallback( ONPROGRESS_CALLBACK cb );
    /** stops searching */
    void stop();
	/** Dtor */
	virtual ~FileFinder();
	///** if entries are to be reported in alphabetically order */
	//void sortEntries( bool sortEntries );
	/** registers the one and only match callback */
//	void setMatchCallback( ONMATCH_CALLBACK cb );
//	/** sets a directory callback.
//	 *  You will need this when implementing multithreaded file search.
//	 *  Switch includeSubDirs to false and create a new FileFinder
//	 *  in a new thread on every found directory entry.  */
//	void setDirectoryCallback( ONDIRECTORYFOUND_CALLBACK cb );

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
	/** handles each entry met: reports immediately or remembers it,
	 * according to the sort option. */
	//void handleEntry( const char *dir, const my::EntryPtr pEntry );
	/** checks if the given file or directory name satisfies the defined pattern */
	bool matchesPattern( const std::string &text );
	/** checks if a file's content matches a given pattern */
	bool contentMatchesPattern( const my::EntryPtr pEntry );
	/** creates a new Entry instance (for reporting purposes) */
	//const Entry &createEntry( const char *dir, const Entry *pEntry );
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
	/** compiles a given non-regex path pattern */
	void compilePath( const char *pattern );
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
    //int _callCnt;
    int _cntVisited;
    int _cntMatch;
    //std::vector<const char*> _dirList;

    char _progressMsg[512];
    //bool _stop;
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
