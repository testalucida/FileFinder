#include "searchcriteriagroup.h"
#include "SearchCriteria.h"

#include <cstdio>
#include <cstdlib>

#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Native_File_Chooser.H>

#include <flx/Flx_Group.h>
#include <flx/flx_boxtypes.h>
#include <flx/Flx_Message.h>

using namespace flx;

SearchCriteriaGroup::
SearchCriteriaGroup( int x, int y, int w, int h, SearchCriteria &crit )
	: Flx_Group(x, y, w, h )
	, _searchCrit( crit )
{
    box( FLX_GRADIENT_BORDER_BOX );
    color( fl_lighter( fl_lighter( FL_GREEN ) ) );

    _pInpStartDir = new Flx_Input(139, 10, 401, 23, "Startverzeichnis: ");
    _pInpStartDir->value( _searchCrit.getSearchPath() );
    _pInpStartDir->signalEnterPressed.
            connect < SearchCriteriaGroup, &SearchCriteriaGroup::onInputEnter >( this );

    _pBtnOpenFileDlg = new Flx_Button(545, 10, 25, 23, "...");
    _pBtnOpenFileDlg->signalSelected.
    connect < SearchCriteriaGroup, &SearchCriteriaGroup::onOpenFileDlg >( this );

    _pInpPattern = new Flx_Input(139, 36, 401, 23, "Suche Dateien: ");
    _pInpPattern->value( "*" );
    _pInpPattern->signalEnterPressed.
            connect < SearchCriteriaGroup, &SearchCriteriaGroup::onInputEnter >( this );

    _pInpSearchContent = new Flx_Input(139, 62, 401, 23, "Suche nach Inhalt: ");
    _pInpSearchContent->signalEnterPressed.
            connect < SearchCriteriaGroup, &SearchCriteriaGroup::onInputEnter >( this );
    
    _pCheckIncludeSubDirs = new Flx_CheckButton(137, 88, 25, 25, " Unterverzeichnisse durchsuchen");
    _pCheckIncludeSubDirs->down_box(FL_DOWN_BOX);
    _pCheckIncludeSubDirs->value( 1 );

    _pCheckWholeWords = new Flx_CheckButton(137, 107, 25, 25, " nur nach ganzen W\303\266rtern suchen");
    _pCheckWholeWords->down_box(FL_DOWN_BOX);

    _pCheckMatchCase = new Flx_CheckButton(137, 126, 25, 25, " Gro\303\237-/Kleinschreibung beachten");
    _pCheckMatchCase->down_box(FL_DOWN_BOX);
    _pCheckMatchCase->labelsize(12);

    _pBtnStart = new Flx_ReturnButton(139, 154, 401, 23, "Suche starten");
    _pBtnStart->signalSelected.
    connect < SearchCriteriaGroup, &SearchCriteriaGroup::onStartButtonPressed > ( this );

    end();
}

void SearchCriteriaGroup::setStartDirectory( const char *pPath ) {
    _pInpStartDir->value(( pPath ) );
}

Flx_ReturnButton &SearchCriteriaGroup::getStartButton() const {
    return *_pBtnStart;
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

/** Start-Button gedrückt */
void SearchCriteriaGroup::
onStartButtonPressed( Flx_ReturnButton &btn, ActionParm &parm ) {
    if( strcmp( _pBtnStart->label(), STOP_SEARCH ) == 0 ) {
        _searchCrit.stopSearch = true;
        _pBtnStart->label( WAIT );
        Fl::wait();
        signalStartStop.send( btn, _searchCrit );
        return;
    }

    if( strlen( _pInpStartDir->value() ) <= 0  ||
	strlen( _pInpPattern->value() ) <= 0 )
    {
        Flx_Message::failed( "Fehler:\n\nSowohl Startverzeichnis wie Suchmuster müssen angegeben sein." );
	return;
    }

    _pBtnStart->label( STOP_SEARCH );
    Fl::wait();

    _searchCrit.setSearchPath( _pInpStartDir->value() );
    _searchCrit.setFilePattern( _pInpPattern->value() );
    _searchCrit.setSearchContent( _pInpSearchContent->value() );
    _searchCrit.includeSubDirs( ( _pCheckIncludeSubDirs->value() == 1 ) );
    _searchCrit.matchWord( ( _pCheckWholeWords->value() == 1 ) );
    _searchCrit.matchCase( ( _pCheckMatchCase->value() == 1 ) );

    _searchCrit.stopSearch = false;
    signalStartStop.send( btn, _searchCrit );

    //Suche beendet:
    _pBtnStart->label( START_SEARCH );
}

void SearchCriteriaGroup::onInputEnter( Flx_Input &, ActionParm & ) {
    ActionParm parm;
    onStartButtonPressed( *_pBtnStart, parm );
}

my::Signal < Flx_ReturnButton, SearchCriteria > &SearchCriteriaGroup::
getStartStopSignal() {
    return signalStartStop;
}

SearchCriteriaGroup::~SearchCriteriaGroup() {
}
