/* 
 * File:   MainWindowViewModel.cpp
 * Author: max
 * 
 * Created on 22. Februar 2015, 18:35
 */

#include "MainWindowController.h"
#include "MainWindow.h"
#include "FileFinderService.h"

MainWindowController::MainWindowController( MainWindow &win ) 
: _win( win )
, _ffService( FileFinderService::instance() )
{
    SearchCriteriaPtr pSearchCrit = _ffService.getSearchCriteria();
    HitListPtr pHitList = _ffService.getHitList();
    _win.setModel( pSearchCrit, pHitList );
}


MainWindowController::~MainWindowController( ) {
}

