/**
 * @file MainWindow.h
 * Main window class definition file.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QProcess>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QTextBrowser>
#include <QSettings>
#include <QSplitter>
#include <QWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QMutex>
#include "tab.h"

/*! \brief The MainWindow class defines the actions and behavior of the main user interface.
 *
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(/*const QStringList &args, */QWidget *parent = 0);
    ~MainWindow();

    void initUi();
    void createActions();
    void createMenus();
    void setTabPath(const QString &filePath, int index = -1);

private:
    //ui
    QStackedWidget *mainWidget;
    QSplitter *splitter;
    QTabWidget *tabs;
    QVBoxLayout *workLayout;
    QWidget *workWidget;
    QProcess *runProcess;
    QSettings settings;
    TextEdit *compilerOut;
    QString debugKey;

    //! Menus and Actions
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *debugMenu;
    QMenu *buildMenu;
    QMenu *settingsMenu;
    QMenu *helpMenu;
    QAction *newAction;
    QAction *openAction;
    QAction *closeAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *saveExeAction;
    QAction *exitAction;
    QAction *findAction;
    QAction *commentAction;
    QAction *uncommentAction;
    QAction *undoAction;
    QAction *redoAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *deleteAction;
    QAction *selectAllAction;
    QAction *putTabAction;
    QAction *deleteTabAction;
    QAction *buildAction;
    QAction *runAction;
    QAction *runExeAction;
    QAction *stopAction;
    QAction *debugAction;
    QAction *debugNextAction;
    QAction *debugNextNiAction;
    QAction *debugToggleBreakpointAction;
    QAction *debugShowRegistersAction;
    QAction *debugShowMemoryAction;
    QAction *settingsAction;
    QAction *helpAction;
    QAction *aboutAction;


    QString saveOpenDirectory;

public slots:
    //! Actions and Menus
    void newFile();
    void openFile();
    void closeFile();
    bool saveFile(int index = -1, bool openSaveAs = true);
    bool saveAsFile(int index = -1);
    void openFile(QString path);
    bool deleteTab(int index, bool saveFileName = false);
};

#endif