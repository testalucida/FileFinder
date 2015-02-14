/* 
 * File:   ApplicationStarter.h
 * Author: max
 *
 * Created on 14. September 2013, 13:28
 */

#ifndef APPLICATIONSTARTER_H
#define	APPLICATIONSTARTER_H

class MainWindow;
struct OpenParm;

class ApplicationStarter {
public:
    ApplicationStarter();
    void onOpenFile( MainWindow &, OpenParm & );
    void onOpenDir( MainWindow &, OpenParm & );
    virtual ~ApplicationStarter();
private:
    //void openApplication( const char *app, const char *path );
};

#endif	/* APPLICATIONSTARTER_H */

