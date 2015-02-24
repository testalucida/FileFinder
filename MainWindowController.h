/* 
 * File:   MainWindowViewModel.h
 * Author: max
 *
 * Created on 22. Februar 2015, 18:35
 */

#ifndef MAINWINDOWVIEWMODEL_H
#define	MAINWINDOWVIEWMODEL_H

class MainWindow;
class FileFinderService;

class MainWindowController {
public:
    MainWindowController( MainWindow &win );
    ~MainWindowController();
private:
    MainWindow &_win;
    FileFinderService &_ffService;
};

#endif	/* MAINWINDOWVIEWMODEL_H */

