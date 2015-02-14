#ifndef SEARCHCRITERIAGROUP_H
#define SEARCHCRITERIAGROUP_H

#include <my/Signal.hpp>

#include <flx/Flx_Group.h>
#include <flx/Flx_Input.h>
#include <flx/Flx_CheckButton.h>
#include <flx/Flx_Button.h>
#include <flx/Flx_ReturnButton.h>

#include <FL/Fl_Return_Button.H>

using namespace flx;

class SearchCriteria;

class SearchCriteriaGroup : public Flx_Group  {
public:
    my::Signal < Flx_ReturnButton, SearchCriteria > signalStartStop;
public:
    SearchCriteriaGroup( int x, int y, int w, int h, SearchCriteria & );
    ~SearchCriteriaGroup();
    /** callback für den Start-Stop-Button */
    void onStartButtonPressed( Flx_ReturnButton &btn, ActionParm &parm );
    /** callback für die Flx_Input-Felder -- bewirkt Start der Suche */
    void onInputEnter( Flx_Input &, ActionParm & );
    void onOpenFileDlg( Flx_Button &, ActionParm & );
    void setStartDirectory( const char *pPath );
    Flx_ReturnButton &getStartButton() const;
    my::Signal < Flx_ReturnButton, SearchCriteria > &getStartStopSignal();
private:
    Flx_Input *_pInpStartDir;
    Flx_Button *_pBtnOpenFileDlg;
    Flx_Input *_pInpPattern;
    Flx_Input *_pInpSearchContent;
    Flx_CheckButton *_pCheckIncludeSubDirs;
    Flx_CheckButton *_pCheckWholeWords;
    Flx_CheckButton *_pCheckMatchCase;
    Flx_ReturnButton *_pBtnStart;
    Flx_Group *_dummyGrp;
    SearchCriteria &_searchCrit;
};

#endif // SEARCHCRITERIAGROUP_H
