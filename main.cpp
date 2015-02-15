
#include "MainWindow.h"
#include "SearchCriteria.h"
#include "FileFinder.h"
#include "FileFinderController.h"
#include "HitList.h"
#include "ApplicationStarter.h"

#include <my/TableData.h>
#include <my/FileHelper.h>

#include <flx/Flx_Window.h>
#include <flx/Flx_Group.h>
#include <flx/Flx_Table.h>
#include <flx/Flx_Output.h>
#include <flx/Flx_Button.h>

#include <FL/Fl.H>

//---------------------------------------------------

int main(int argc, char **argv) {

    SearchCriteria searchCrit;
    HitList hitlist;

	MainWindow mainWin( searchCrit, hitlist );
	FileFinderController ffc( searchCrit, hitlist, mainWin.getStatusBar() );
    ApplicationStarter appStarter;
    
	mainWin.getStartStopSignal().connect < FileFinderController,
        &FileFinderController::onStartStopSearch > ( &ffc );
    
    
    mainWin.signalOpen.
            connect < ApplicationStarter, &ApplicationStarter::onOpenFile >
                    ( &appStarter );
    
    mainWin.signalOpenDir.
            connect < ApplicationStarter, &ApplicationStarter::onOpenDir >
                    ( &appStarter );
    
    mainWin.show();

    return Fl::run();
}
