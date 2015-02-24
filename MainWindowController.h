/* 
 * File:   MainWindowViewModel.h
 * Author: max
 *
 * Created on 22. Februar 2015, 18:35
 */

#ifndef MAINWINDOWVIEWMODEL_H
#define	MAINWINDOWVIEWMODEL_H

#include "SearchCriteria.h"
#include "HitList.h"
#include "FileFinder.h"

class MainWindow;
class FileFinderService;

namespace flx {
    class Flx_ReturnButton;
}

class MainWindowController {
public:
    MainWindowController( MainWindow &win );
    ~MainWindowController();
    void onStartSearch( flx::Flx_ReturnButton &, SearchCriteriaPtr & );
    void onMatch( FileFinder &, const EntryPtr & );
    void onSearchTerminated( FileFinder &, const SearchStat & );
private:
    MainWindow &_win;
//    FileFinderService &_ffService;
    HitListPtr _pHitList;
};

#endif	/* MAINWINDOWVIEWMODEL_H */

