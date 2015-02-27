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
#include "ApplicationStarter.h"

#include <flx/Flx_ReturnButton.h>
#include <flx/Flx_Message.h>

#include <my/FileHelper.h>

#include <FL/fl_draw.H>
#include <FL/Fl.H>

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
    pSearchCrit->filePattern.add( "*.cpp, *.h" );
    pSearchCrit->includeSubDirs = true;
    pSearchCrit->matchCase = false;
    pSearchCrit->matchWord = false;
    
    HitListPtr pHitList( new HitList() );
    _pHitList = pHitList;
    _win.setModel( pSearchCrit, pHitList );
    _win.signalStart.connect<MainWindowController, &MainWindowController::onStartSearch>( this );
    _win.signalOpen.connect<ApplicationStarter, &ApplicationStarter::onOpenFile>( &_appStarter );
    _win.signalOpenDir.connect<ApplicationStarter, &ApplicationStarter::onOpenDir>( &_appStarter );
}

void MainWindowController::onStartSearch( flx::Flx_ReturnButton &btn, 
                                          SearchCriteriaPtr &pSearchCrit ) 
{
    fl_cursor( FL_CURSOR_WAIT );
    btn.deactivate();
    Fl::wait( 0.5 );
    _pHitList->removeRows();
    if( pSearchCrit->searchPath.isEmpty() || pSearchCrit->filePattern.isEmpty() ) {
        flx::Flx_Message::failed( "Weder Suchpfad noch FilePattern dürfen leer sein" );
    } else {
        FileFinder ff( *pSearchCrit );
        ff.signalMatch.connect<MainWindowController, &MainWindowController::onMatch>( this );
        ff.signalTerminated.connect<MainWindowController, &MainWindowController::onSearchTerminated>( this );
        ff.start();
    }
    fl_cursor( FL_CURSOR_DEFAULT );
    btn.activate();
}

void MainWindowController::onMatch( FileFinder &ff, const EntryPtr &pEntryPtr ) {
    _pHitList->addEntry( pEntryPtr->directory.c_str(), 
                         pEntryPtr->name.c_str(), 
                         pEntryPtr->lastWrite );
}

void MainWindowController::onSearchTerminated( FileFinder &ff, const SearchStat &searchStat ) {
    CharBuffer msg;
    msg.add( "Suche beendet. " ).addInt( searchStat.cntVisited ).add( " Dateien betrachtet, " )
       .addInt( searchStat.cntMatch ).add( " Treffer." );
    _win.setStatus( msg.get() );
}

//void MainWindowController::onOpenFile( MainWindow &, OpenParm &parm ) {
//    ApplicationStarter starter;
//}
//
//void MainWindowController::onOpenDir( MainWindow &, OpenParm &parm ) {
//    
//}

MainWindowController::~MainWindowController( ) {
}

