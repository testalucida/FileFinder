#include "FileFinderController.h"

#include "SearchCriteria.h"
#include "HitList.h"
#include "FileFinder.h"

#include <my/convert.h>

#include <flx/Flx_Output.h>

#include <FL/Fl.H>

#include <functional>

using namespace std;
using namespace flx;


FileFinderController::FileFinderController(
	SearchCriteria &searchCrit, HitList &list, Flx_Output &statusBar )
	: _searchCrit( searchCrit )
	, _ff( *( new FileFinder( searchCrit ) ) )
	, _hitList( list )
	, _statusBar( statusBar )
{
    _ff.signalMatch.connect < FileFinderController,
                              &FileFinderController::onMatch >( this );
    _ff.signalTerminated.connect < FileFinderController,
                                   &FileFinderController::onSearchTerminated > ( this );
}


FileFinderController::~FileFinderController(void) {
    delete &_ff;
}


void FileFinderController::onStartStopSearch( Flx_ReturnButton &btn, SearchCriteria & ) {
	if( !_searchCrit.stopSearch ) {
		_hitList.removeRows();
		_ff.start();
//		if( _onFinishedCallback ) {
//			_onFinishedCallback();
//		}
	} else {
		//Suche abgebrochen
//		if( _onCancelCallback ) {
//			_onCancelCallback();
//		}
		_ff.stop();
	}
}

void FileFinderController::onMatch( FileFinder &, const EntryPtr &pEntry ) {
	_hitList.addEntry( pEntry->directory.c_str(), pEntry->name.c_str(), pEntry->lastWrite );
}

void FileFinderController::onSearchTerminated( FileFinder &, const SearchStat &stat ) {
    string msg( "Suche beendet. " );
    msg.append( *my::Convert::ToString( stat.cntVisited ) );
    msg.append( " Dateien untersucht, " );
    msg.append( *my::Convert::ToString( stat.cntMatch ) );
    msg.append( " Treffer gefunden." );
    _statusBar.value( msg.c_str() );
}

void FileFinderController::onProgress( const char *pMsg ) {
	_statusBar.value( pMsg );
	Fl::wait();
}
