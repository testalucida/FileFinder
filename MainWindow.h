#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include "OpenParm.h"

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
    struct MenuItemAction;
}

class SearchCriteriaGroup;


class MainWindow : public flx::Flx_Window {
public:
    my::Signal < flx::Flx_ReturnButton, SearchCriteriaPtr > signalStart;
public:
    MainWindow( );
    virtual ~MainWindow();
    void setModel( SearchCriteriaPtr pSearchCrit, HitListPtr pHitList );
    void onCellMenuItem( flx::Flx_ContextMenu &, flx::MenuItemAction & );
    void onStart( flx::Flx_ReturnButton &, SearchCriteriaPtr & );
    void setStatus( const char *pMsg );
private:
    SearchCriteriaGroup *_pSearchCritGrp;
    flx::Flx_Group *_pLowerGrp;
    flx::Flx_Table *_pResultTbl;
    flx::Flx_Output *_pStatusMsg;
    HitListPtr _pHitList;

};

#endif // MAINWINDOW_H
