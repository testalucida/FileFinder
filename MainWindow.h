#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "OpenParm.h"

//#include <my/Signal.hpp>

#include "SearchCriteria.h"
#include "HitList.h"

#include <flx/Flx_Window.h>

#include <my/Signal.hpp>

namespace flx {
    class Flx_Group;
    class Flx_Output;
    class Flx_Table;
    class Flx_ContextMenu;
    class Flx_ReturnButton;
    class Flx_Button;
    struct MenuItemAction;
}

class SearchCriteriaGroup;


class MainWindow : public flx::Flx_Window {
public:
    my::Signal < flx::Flx_ReturnButton, SearchCriteriaPtr > signalStartStop;
//    my::Signal < flx::Flx_Button, SearchCriteriaPtr > signalCancel;
    my::Signal<MainWindow, OpenParm> signalOpen;
    my::Signal<MainWindow, OpenParm> signalOpenDir;
    
public:
    MainWindow( );
    virtual ~MainWindow();
    void setModel( SearchCriteriaPtr pSearchCrit, HitListPtr pHitList );
    void onCellMenuItem( flx::Flx_ContextMenu &, flx::MenuItemAction & );
    void onStartStop( flx::Flx_ReturnButton &, SearchCriteriaPtr & );
//    void onCancel( flx::Flx_Button &, SearchCriteriaPtr & );
    void setStatus( const char *pMsg );
private:
    SearchCriteriaGroup *_pSearchCritGrp;
    flx::Flx_Group *_pLowerGrp;
    flx::Flx_Table *_pResultTbl;
    flx::Flx_Output *_pStatusMsg;
    HitListPtr _pHitList;

};

#endif // MAINWINDOW_H
