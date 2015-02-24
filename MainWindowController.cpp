/* 
 * File:   MainWindowViewModel.cpp
 * Author: max
 * 
 * Created on 22. Februar 2015, 18:35
 */

#include "MainWindowController.h"
#include "MainWindow.h"
#include "FileFinderService.h"
#include "FileFinder.h"

#include <flx/Flx_ReturnButton.h>
#include <flx/Flx_Message.h>

#include <my/FileHelper.h>

using namespace my;

MainWindowController::MainWindowController( MainWindow &win ) 
: _win( win )
//, _ffService( FileFinderService::instance() )
{
//    SearchCriteriaPtr pSearchCrit = _ffService.getSearchCriteria();
//    HitListPtr pHitList = _ffService.getHitList();
    
    SearchCriteriaPtr pSearchCrit( new SearchCriteria() );
    //set some defaults
    StringPtr curDir = FileHelper::instance().getCurrentDirectory();
    pSearchCrit->searchPath.add( curDir->c_str() );
    pSearchCrit->filePattern.add( "*.cpp; *.h" );
    pSearchCrit->includeSubDirs = true;
    pSearchCrit->matchCase = false;
    pSearchCrit->matchWord = false;
    
    HitListPtr pHitList( new HitList() );
    
    _win.setModel( pSearchCrit, pHitList );
    _win.signalStart.connect<MainWindowController, &MainWindowController::onStartSearch>( this );
}

void MainWindowController::onStartSearch( flx::Flx_ReturnButton &btn, 
                                          SearchCriteriaPtr &pSearchCrit ) 
{
    if( pSearchCrit->searchPath.isEmpty() || pSearchCrit->filePattern.isEmpty() ) {
        flx::Flx_Message::failed( "Weder Suchpfad noch FilePattern dürfen leer sein" );
    } else {
        FileFinder ff( *pSearchCrit );
        ff.signalMatch.connect<MainWindowController, &MainWindowController::onMatch>( this );
        ff.signalTerminated.connect<MainWindowController, &MainWindowController::onSearchTerminated>( this );
        ff.start();
    }
}

void MainWindowController::onMatch( FileFinder &ff, const EntryPtr &pEntryPtr ) {
    
}

void MainWindowController::onSearchTerminated( FileFinder &ff, const SearchStat &searchStat ) {
    CharBuffer msg;
    msg.add( "Suche beendet. " ).addInt( searchStat.cntVisited ).add( " Dateien betrachtet, " )
       .addInt( searchStat.cntMatch ).add( " gefunden." );
    _win.setStatus( msg.get() );
}

MainWindowController::~MainWindowController( ) {
}

