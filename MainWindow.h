#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include "OpenParm.h"

//#include <my/Signal.hpp>

#include "SearchCriteria.h"
#include "HitList.h"

#include <flx/Flx_Window.h>

namespace flx {
    class Flx_Group;
    class Flx_Output;
    class Flx_Table;
    class Flx_ContextMenu;
    struct MenuItemAction;
}

class SearchCriteriaGroup;


class MainWindow : public flx::Flx_Window {

public:
    MainWindow( );
    virtual ~MainWindow();
    void setModel( SearchCriteriaPtr pSearchCrit, HitListPtr pHitList );
    void onCellMenuItem( flx::Flx_ContextMenu &, flx::MenuItemAction & );
private:
    SearchCriteriaGroup *_pSearchCritGrp;
    flx::Flx_Group *_pLowerGrp;
    flx::Flx_Table *_pResultTbl;
    flx::Flx_Output *_pStatusMsg;
    HitListPtr _pHitList;

};

#endif // MAINWINDOW_H
