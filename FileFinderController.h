#ifndef _FILEFINDERCONTROLLER_
#define _FILEFINDERCONTROLLER_

#include <my/DirectoryIterator.h>

class SearchCriteria;
class FileFinder;
class HitList;
class SearchStat;

namespace flx {
	class Flx_Output;
	class Flx_ReturnButton;
}

class FileFinderController {
public:
	FileFinderController( SearchCriteria &, HitList &, flx::Flx_Output & );
	~FileFinderController(void);
	/** callback f�r ON_START_STOP_SEARCH */
	void onStartStopSearch( flx::Flx_ReturnButton &, SearchCriteria & );
	void onMatch( FileFinder &, const my::EntryPtr & );
	void onSearchTerminated( FileFinder &, const SearchStat & );
private:
	void onProgress( const char *pMsg );
private:
	SearchCriteria &_searchCrit;
	FileFinder &_ff;
	HitList &_hitList;
	flx::Flx_Output &_statusBar;
//	ON_FINISHED_SEARCH _onFinishedCallback;
//	ON_CANCEL_SEARCH _onCancelCallback;
};

#endif
