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
#include "config.h"
#include "test/threads.h"

class MainWindow;
class FileFinderService;
struct OpenParm;

namespace flx {
    class Flx_ReturnButton;
    class Flx_Button;
}

class MainWindowController {
public:
    MainWindowController( MainWindow &win );
    ~MainWindowController();
    void onStartStopSearch( flx::Flx_ReturnButton &, SearchCriteriaPtr & );
    void onMatch( FileFinder &, const EntryPtr & );
    void onSearchTerminated( FileFinder &, const SearchStat & );
    void startSearch( /*SearchCriteriaPtr &pSearchCrit */ );
    void onCanProceed( FileFinder &, CanProceedParm & );
private:
    MainWindow &_win;
    bool _isThreadRunning, _isThreadCancelled;
    HitListPtr _pHitList;
    ApplicationStarter _appStarter;
    SearchCriteriaPtr _pSearchCrit;
    Fl_Thread _searchThread;
};

#endif	/* MAINWINDOWVIEWMODEL_H */

