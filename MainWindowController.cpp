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


static void *startSearch( void *parm ) {
    ((MainWindowController*)parm)->startSearch();
    return NULL;
}

MainWindowController::MainWindowController( MainWindow &win ) 
: _win( win )
, _isThreadRunning( false )
, _isThreadCancelled( false )
{
//    SearchCriteriaPtr pSearchCrit = _ffService.getSearchCriteria();
//    HitListPtr pHitList = _ffService.getHitList();
    
    SearchCriteriaPtr pSearchCrit( new SearchCriteria() );
    //set some defaults
    StringPtr curDir = FileHelper::instance().getCurrentDirectory();
#ifdef _DEBUG
    pSearchCrit->searchPath.add( curDir->c_str() );
#else 
    pSearchCrit->searchPath.add( getenv("HOME") );
#endif
    pSearchCrit->filePattern.add( "*.cpp, *.h" );
    pSearchCrit->includeSubDirs = true;
    pSearchCrit->matchCase = false;
    pSearchCrit->matchWord = false;
    
    HitListPtr pHitList( new HitList() );
    _pHitList = pHitList;
    _win.setModel( pSearchCrit, pHitList );
    _win.signalStartStop.connect<MainWindowController, &MainWindowController::onStartStopSearch>( this );
    _win.signalOpen.connect<ApplicationStarter, &ApplicationStarter::onOpenFile>( &_appStarter );
    _win.signalOpenDir.connect<ApplicationStarter, &ApplicationStarter::onOpenDir>( &_appStarter );
}

void MainWindowController::onStartStopSearch( flx::Flx_ReturnButton &btn, 
                                          SearchCriteriaPtr &pSearchCrit ) 
{
    _pHitList->removeRows();
    
    if( !_isThreadRunning ) {
        //kein Thread läuft, Suche starten
        _win.setStatus( "" );
        fl_cursor( FL_CURSOR_WAIT );
        _win.setStartStopLabel( STARTSTOPLABEL_STOP, false );
        Fl::wait( 0.5 );
       
        //startSearch( pSearchCrit );
        _pSearchCrit = pSearchCrit;
        fl_create_thread( _searchThread, ::startSearch, this );
    } else {
        //Thread aktiv, versuchen, Suche abzubrechen
        _isThreadCancelled = true;
        _win.setStartStopLabel( STARTSTOPLABEL_WAIT, true );
        Fl::wait( 0.5 );
    }
}

void MainWindowController::startSearch( /*SearchCriteriaPtr &pSearchCrit*/ ) {
    if( _pSearchCrit->searchPath.isEmpty() || _pSearchCrit->filePattern.isEmpty() ) {
        flx::Flx_Message::failed( "Weder Suchpfad noch FilePattern dürfen leer sein" );
    } else {
        FileFinder ff( *_pSearchCrit );
        ff.signalMatch.connect<MainWindowController, &MainWindowController::onMatch>( this );
        ff.signalTerminated.connect<MainWindowController, &MainWindowController::onSearchTerminated>( this );
        ff.signalCanProceed.connect<MainWindowController, &MainWindowController::onCanProceed>( this );
        _isThreadRunning = true;
        ff.start();        
    }
}

void MainWindowController::onCanProceed( FileFinder &, CanProceedParm &parm ) {
    parm.canProceed = !_isThreadCancelled;
}

void MainWindowController::onMatch( FileFinder &ff, const EntryPtr &pEntryPtr ) {
    
    Fl::lock();
    
    _pHitList->addEntry( pEntryPtr->directory.c_str(), 
                         pEntryPtr->name.c_str(), 
                         pEntryPtr->lastWrite );
    
    Fl::unlock();
    Fl::awake( (void*)NULL );
}

void MainWindowController::onSearchTerminated( FileFinder &ff, const SearchStat &searchStat ) {
    CharBuffer msg;
    if( _isThreadCancelled ) {
        msg.add( "Suche abgebrochen." );
    } else {
        msg.add( "Suche beendet. " ).addInt( searchStat.cntVisited ).add( " Dateien betrachtet, " )
           .addInt( searchStat.cntMatch ).add( " Treffer." );
    }
    
    Fl::lock();
    _win.setStatus( msg.get() );
    fl_cursor( FL_CURSOR_DEFAULT );
    _win.setStartStopLabel( STARTSTOPLABEL_START, false );
    Fl::unlock();
    Fl::awake( (void*)NULL );
    
    _isThreadRunning = false;
    _isThreadCancelled = false;
   
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

