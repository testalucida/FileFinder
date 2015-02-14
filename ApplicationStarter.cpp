/* 
 * File:   ApplicationStarter.cpp
 * Author: max
 * 
 * Created on 14. September 2013, 13:28
 */

#include "ApplicationStarter.h"
#include "OpenParm.h"
#include "MainWindow.h"

#include <my/FileHelper.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <vector>
#include <string>

using namespace std;
using namespace my;

ApplicationStarter::ApplicationStarter( ) {
    
}

void ApplicationStarter::onOpenFile( MainWindow &, OpenParm &parm ) {
    #ifdef WIN32
        ShellExecute(0, 0, L"c:\\outfile.txt", 0, 0 , SW_SHOW );
    #else
        struct stat attr;
        int rc;
        vector<const char*>::iterator itr;
        for( itr = parm.completePaths.begin(); itr != parm.completePaths.end(); itr++ ) {
            const char *path = *itr;
            if( ( rc = stat( path, &attr ) ) == 0 ) {
                if( S_ISREG( attr.st_mode ) ) {
                    string command( "xdg-open " );
                    command.append( path );
                    system( command.c_str() );
                }
            }
        }
    #endif
}

void ApplicationStarter::onOpenDir( MainWindow &, OpenParm &parm ) {
    #ifdef WIN32
        
    #else
        struct stat attr;
        int rc;
        vector<const char*>::const_iterator itr;
        for( itr = parm.completePaths.begin(); itr != parm.completePaths.end(); itr++ ) {
            const char *path = *itr;
            string dirRem, subDir;
            FileHelper::instance().splitLastSubDir( path, dirRem, subDir );
            string command( "xdg-open " );
            command.append( dirRem.c_str() );
            system( command.c_str() );
            //execl( "/usr/bin/xfe", "xfe", dirRem.c_str(), NULL );
        } //for
    #endif
}



ApplicationStarter::~ApplicationStarter( ) {
}

