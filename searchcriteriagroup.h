#ifndef SEARCHCRITERIAGROUP_H
#define SEARCHCRITERIAGROUP_H

#include "SearchCriteria.h"

#include <my/Signal.hpp>

#include <flx/Flx_Group.h>
#include <flx/Flx_Input.h>
#include <flx/Flx_CheckButton.h>
#include <flx/Flx_Button.h>
#include <flx/Flx_ReturnButton.h>

//#include <FL/Fl_Return_Button.H>

using namespace flx;

class SearchCriteria;

class SearchCriteriaGroup : public Flx_Group  {
public:
    my::Signal < Flx_ReturnButton, SearchCriteriaPtr > signalStart;
public:
    SearchCriteriaGroup( int x, int y, int w, int h );
    ~SearchCriteriaGroup();
    void onOpenFileDlg( Flx_Button &, ActionParm & );
    void setModel( SearchCriteriaPtr pSearchCrit );
private:
    void onStart( Flx_ReturnButton &, ActionParm & );
    void modelToFields();
    void fieldsToModel();
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
    SearchCriteriaPtr _pSearchCrit;
};

#endif // SEARCHCRITERIAGROUP_H
