#include "searchcriteriagroup.h"
#include "SearchCriteria.h"

#include <cstdio>
#include <cstdlib>

#include <FL/fl_draw.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Native_File_Chooser.H>

#include <flx/Flx_Group.h>
#include <flx/flx_boxtypes.h>
#include <flx/Flx_Message.h>

using namespace flx;

SearchCriteriaGroup::
SearchCriteriaGroup( int x, int y, int w, int h )
	: Flx_Group(x, y, w, h )
{
    box( FL_FLAT_BOX );
    color( fl_rgb_color( 220, 220, 220 ) );

    _pInpStartDir = new Flx_Input(139, 10, 401, 23, "Startverzeichnis: ");


    _pBtnOpenFileDlg = new Flx_Button(545, 10, 25, 23, "...");
    _pBtnOpenFileDlg->signalSelected.
    connect < SearchCriteriaGroup, &SearchCriteriaGroup::onOpenFileDlg >( this );

    _pInpPattern = new Flx_Input(139, 36, 401, 23, "Dateiname-Pattern: ");
 

    _pInpSearchContent = new Flx_Input(139, 62, 401, 23, "Volltextsuche nach: ");
  
    
    _pCheckIncludeSubDirs = new Flx_CheckButton(137, 88, 25, 25, " Unterverzeichnisse durchsuchen");
    _pCheckIncludeSubDirs->down_box(FL_DOWN_BOX);
    _pCheckIncludeSubDirs->value( 1 );

    _pCheckWholeWords = new Flx_CheckButton(137, 107, 25, 25, " nur nach ganzen W\303\266rtern suchen");
    _pCheckWholeWords->down_box(FL_DOWN_BOX);

    _pCheckMatchCase = new Flx_CheckButton(137, 126, 25, 25, " Gro\303\237-/Kleinschreibung beachten");
    _pCheckMatchCase->down_box(FL_DOWN_BOX);
    _pCheckMatchCase->labelsize(12);

    _pBtnStart = new Flx_ReturnButton(139, 154, 401, 23, "Suche starten");
    _pBtnStart->signalSelected.connect<SearchCriteriaGroup, &SearchCriteriaGroup::onStart>( this );
 
    end();
}

const char* START_SEARCH = "Suche starten";
const char* STOP_SEARCH = "Suche abbrechen";
const char* WAIT = "Warten...";

void SearchCriteriaGroup::onOpenFileDlg( Flx_Button &, ActionParm & ) {
	Fl_Native_File_Chooser *pChooser =
		new Fl_Native_File_Chooser(Fl_Native_File_Chooser::BROWSE_DIRECTORY);

	pChooser->title( "Startverzeichnis auswählen" );
	pChooser->directory( _pInpStartDir->value() );
    int rc = pChooser->show();
	if( rc == 0 ) {
		const char *dir = pChooser->filename();
		if( dir ) {
			_pInpStartDir->value( dir );
		}
	}
}

void SearchCriteriaGroup::onStart( Flx_ReturnButton &btn, ActionParm & ) {
    fieldsToModel();
    signalStart.send( btn, _pSearchCrit );
}

void SearchCriteriaGroup::setModel( SearchCriteriaPtr pSearchCrit ) {
    _pSearchCrit = pSearchCrit;
    modelToFields();
}

void SearchCriteriaGroup::modelToFields() {
    _pInpStartDir->value( _pSearchCrit->searchPath.get() );
    _pInpPattern->value( _pSearchCrit->filePattern.get() );
    _pInpSearchContent->value( _pSearchCrit->searchContent.get() );
    _pCheckIncludeSubDirs->value( _pSearchCrit->includeSubDirs );
    _pCheckMatchCase->value( _pSearchCrit->matchCase );
    _pCheckWholeWords->value( _pSearchCrit->matchWord );
}

void SearchCriteriaGroup::fieldsToModel() {
    _pSearchCrit->searchPath.clear();
    _pSearchCrit->searchPath.add( _pInpStartDir->value() );
    
    _pSearchCrit->filePattern.clear();
    _pSearchCrit->filePattern.add( _pInpPattern->value() );
    
    _pSearchCrit->searchContent.clear();
    _pSearchCrit->searchContent.add( _pInpSearchContent->value() );
            
    _pSearchCrit->includeSubDirs = _pCheckIncludeSubDirs->value();
    _pSearchCrit->matchCase = _pCheckMatchCase->value();
    _pSearchCrit->matchWord = _pCheckWholeWords->value();
}

SearchCriteriaGroup::~SearchCriteriaGroup() {
}
