
#include "MainWindow.h"
#include "FileFinderService.h"
#include "MainWindowController.h"

#include <FL/Fl.H>

//---------------------------------------------------

int main(int argc, char **argv) {
    char *pHome = getenv("HOME");
    MainWindow mainWin;

    MainWindowController ctrl( mainWin );
    
    mainWin.show();
    
    Fl::lock();
    
    return Fl::run();
}
