#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "OpenParm.h"

#include <my/Signal.hpp>

#include <flx/Flx_Window.h>

namespace flx {
    class Flx_Group;
    class Flx_Output;
    class Flx_Table;
    class Flx_ReturnButton;
    class ActionParm;
    class Flx_ContextMenu;
    struct MenuItemAction;
}

class SearchCriteriaGroup;
class SearchCriteria; //Model für SearchCriteriaGroup
class HitList;

class MainWindow : public flx::Flx_Window {
public:
    /** open one or more files in an appropriate editor*/
    my::Signal < MainWindow, OpenParm > signalOpen;
    /** open one or more directories containing one 
        or more selected table entries in an 
        appropriate file explorer */
    my::Signal < MainWindow, OpenParm > signalOpenDir;
public:
    MainWindow( SearchCriteria &, HitList & );
    virtual ~MainWindow();
    /** Default-Belegung für Startverzeichnis */
    void setStartDirectory( const char * );
    //callback, wenn der start-Button gedrückt wurde:
    void onStart( flx::Flx_ReturnButton &, flx::ActionParm & );
    my::Signal < flx::Flx_ReturnButton, SearchCriteria > &getStartStopSignal();
    flx::Flx_Output &getStatusBar() { return *_pStatusMsg; }
    void onCellMenuItem( flx::Flx_ContextMenu &, flx::MenuItemAction & );
protected:
private:
    SearchCriteriaGroup *_pSearchCritGrp;
    flx::Flx_Group *_pLowerGrp;
    flx::Flx_Table *_pResultTbl;
    flx::Flx_Output *_pStatusMsg;

};

#endif // MAINWINDOW_H
