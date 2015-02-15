#include "MainWindow.h"
#include "searchcriteriagroup.h"
#include "SearchCriteria.h"
#include "HitList.h"

#include <flx/Flx_Table.h>
#include <flx/Flx_Output.h>
#include <flx/Flx_Group.h>
#include <flx/flx_colors.h>
#include <flx/flx_boxtypes.h>

using namespace flx;

enum MenuIdents {
    MENU_IDENT_OPEN_WITH = 100,
    MENU_IDENT_OPEN_FOLDER
};

MainWindow::MainWindow( SearchCriteria &searchCrit, HitList &hitlist )
: Flx_Window( 500, 100, 575, 440, "Dateien suchen und finden" )
{
    _pSearchCritGrp = new SearchCriteriaGroup(0, 0, 575, 187, searchCrit );

    _pLowerGrp = new Flx_Group(0, 187, 575, 256);
        {
			_pResultTbl = new Flx_Table( 0, 187, 575, 228 );
			_pResultTbl->setNiceDefaults();
			_pResultTbl->setTableData( hitlist );
			_pResultTbl->col_width( 0, 400 );
			_pResultTbl->col_width( 1, 175 );
            _pResultTbl->setSelectionMode( FLX_SELECTION_MULTIROW );
            Flx_ContextMenu &cellMenu = _pResultTbl->getCellContextMenu();
            cellMenu.addMenuItem( "Öffnen mit Editor", 0, MENU_IDENT_OPEN_WITH, 0 );
            cellMenu.addMenuItem( "Öffnen des enthaltenden Ordners",  
                    0, MENU_IDENT_OPEN_FOLDER, 1, NULL, NULL, FL_MENU_DIVIDER );
            _pResultTbl->signalContextMenuItemSelected.
                    connect < MainWindow, &MainWindow::onCellMenuItem >( this );

            Flx_Group::current()->resizable(_pResultTbl);
        } // Fl_Table* _pResultTbl
        {
            _pStatusMsg = new Flx_Output(0, 415, 575, 25, "Statusmessage");
            _pStatusMsg->textfont(3);
            _pStatusMsg->box( FL_THIN_DOWN_BOX );
            _pStatusMsg->color( fl_rgb_color( 200, 200, 200 ) );
        } // Fl_Output* _pStatusMsg
    _pLowerGrp->end();
    Flx_Group::current()->resizable(_pLowerGrp);
}

void MainWindow::setStartDirectory( const char *pDir ) {
    _pSearchCritGrp->setStartDirectory( pDir );
}

void MainWindow::onStart( Flx_ReturnButton &, ActionParm & ) {

}

my::Signal < Flx_ReturnButton, SearchCriteria > &MainWindow::
getStartStopSignal() {
    return _pSearchCritGrp->getStartStopSignal();
}

void MainWindow::onCellMenuItem( Flx_ContextMenu &, flx::MenuItemAction & mia ) {
    const std::vector<int> &selrows = _pResultTbl->getSelectedRows();
    OpenParm parm;
    std::vector<int>::const_iterator itr;
    for( itr = selrows.begin(); itr != selrows.end(); itr++ ) {
        parm.completePaths.push_back( _pResultTbl->getTableData().getValue( *itr, 0 ) );
    }

    if( mia.ident == MENU_IDENT_OPEN_WITH ) {
        signalOpen.send( *this, parm );
    } else if( mia.ident == MENU_IDENT_OPEN_FOLDER ) {
        signalOpenDir.send( *this, parm );
    }
}

MainWindow::~MainWindow()
{
    //dtor
}
