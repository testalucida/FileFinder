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
#include "ApplicationStarter.h"

class MainWindow;
class FileFinderService;
struct OpenParm;

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
//    void onOpenFile( MainWindow &, OpenParm & );
//    void onOpenDir( MainWindow &, OpenParm & );
private:
    MainWindow &_win;
//    FileFinderService &_ffService;
    HitListPtr _pHitList;
    ApplicationStarter _appStarter;
};

#endif	/* MAINWINDOWVIEWMODEL_H */

