/**
 * @file MainWindow.cpp
 * Code for all of the windows and features.
 */

#include "mainwindow.h"

#include <string>

using std::string;
using std::to_string;

MainWindow::MainWindow(/*const QStringList &args, */QWidget *parent)
    : QMainWindow(parent), settings("CSE 661", "Homework2")
{
    setWindowTitle("Homework2");
    setWindowIcon(QIcon(":images/mainIcon.png"));

    //! Set save and open directory
    saveOpenDirectory = settings.value("saveopendirectory", QString(Common::applicationDataPath())).toString();

    //! Initial variables
    runProcess = new QProcess();

    initUi();
    createActions();
    createMenus();
    debugShowRegisters();
    openFile();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initUi()
{
    //! Resize
    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(1024, 650)).toSize()); //default 1024x650
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    //! Set maximized
    setWindowState(windowState() | (Qt::WindowState) settings.value("maximized", (int) Qt::WindowNoState).toInt());
    settings.endGroup();

    //! Create form
    splitter = new QSplitter;
    splitter->setOrientation(Qt::Vertical);
    workLayout = new QVBoxLayout;
    workLayout->addWidget(splitter);
    // workLayout->setMargin(0);
    workLayout->setSpacing(0);
    workWidget = new QWidget;
    workWidget->setLayout(workLayout);

    //! Stacked widget
    mainWidget = new QStackedWidget;
    // mainWidget->addWidget(getStartedWidget);
    mainWidget->addWidget(workWidget);
    mainWidget->setCurrentIndex(0); //get started
    setCentralWidget(mainWidget);

    //! Create highlighter
    // highlighter = new Highlighter(assembler);

    //! Create tabs
    tabs = new QTabWidget;
    connect(tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(deleteTab(int)));
    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(changeCurrentTab(int)));
    tabs->setTabsClosable(true);
    tabs->setMovable(true);

    //! Create compiler field
    compilerOut = new TextEdit;
    compilerOut->setReadOnly(true);
    QFont compilerOutFont;
    compilerOutFont.setPointSize(settings.value("fontsize", 12).toInt());
    compilerOut->setFont(compilerOutFont);
    compilerOut->setText(tr("Build log:") + '\n');

    //! Create gdb command widget
    // debugAnyCommandWidget = new DebugAnyCommandWidget;

    //! Add widgets on splitter
    splitter->addWidget(tabs);
    splitter->addWidget(compilerOut);
    // workLayout->addWidget(debugAnyCommandWidget);
    int compilerOutSize = 100;
    // debugAnyCommandWidget->close();
    // debugAnyCommandWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    splitter->setSizes(QList<int>() << splitter->size().height() - compilerOutSize << compilerOutSize);
}

void MainWindow::debugShowRegisters() {
  if (!registersWindow) {
    registersDock = new QDockWidget(tr("Registers"), this);
    registersDock->setAllowedAreas(Qt::AllDockWidgetAreas);

    int regCount = 16;
    registersWindow = new DebugTableWidget(regCount, 3, registersTable, registersDock);
    bool success = connect(this, SIGNAL(printRegisters(QList<DebugTableWidget::RegistersInfo>)),
            registersWindow, SLOT(setRegisterValues(QList<DebugTableWidget::RegistersInfo>)));

    registersDock->setWidget(registersWindow);
    registersDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::RightDockWidgetArea, registersDock);
    registersDock->setObjectName("registersDock");

    if (registersDock) {
      registersDock->show();
    }

    QList<DebugTableWidget::RegistersInfo> registers;
    for (int i = 0; i < regCount; i++) {
      DebugTableWidget::RegistersInfo reg;
      QString num;
      num.setNum(i);
      reg.name = "r" + num;
      registers.append(reg);
    }
    emit printRegisters(registers);
  }
}

void MainWindow::createActions() {
    QSettings keySettings("keys.ini", QSettings::IniFormat);

    newAction = new QAction(tr("New"), this);
    QString key = keySettings.value("new", "default").toString();
    QKeySequence stdKey(QKeySequence::New);
    if (key == "default")
        key = stdKey.toString();
    newAction->setShortcut(key);
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(tr("Open"), this);
    key = keySettings.value("open", "default").toString();
    stdKey = QKeySequence(QKeySequence::Open);
    if (key == "default")
        key = stdKey.toString();
    openAction->setShortcut(key);
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

    closeAction = new QAction(tr("Close file"), this);
    key = keySettings.value("close", "default").toString();
    stdKey = QKeySequence(QKeySequence::Close);
    if (key == "default")
        key = stdKey.toString();
    closeAction->setShortcut(key);
    connect(closeAction, SIGNAL(triggered()), this, SLOT(closeFile()));
    
    saveAction = new QAction(tr("Save"), this);
    key = keySettings.value("save", "default").toString();
    stdKey = QKeySequence(QKeySequence::Save);
    if (key == "default")
        key = stdKey.toString();
    saveAction->setShortcut(key);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAsAction = new QAction(tr("Save as"), this);
    key = keySettings.value("saveAs", "default").toString();
    stdKey = QKeySequence(QKeySequence::SaveAs);
    if (key == "default")
        key = stdKey.toString();
    saveAsAction->setShortcut(key);
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAsFile()));

    buildAction = new QAction(tr("Build this"), this);
    key = keySettings.value("build", "default").toString();
    stdKey = QKeySequence(QString("Ctrl+F9"));
    if (key == "default")
        key = stdKey.toString();
    buildAction->setShortcut(key);
    //connect(buildAction, SIGNAL(triggered()), this, SLOT(buildProgram()));

    runAction = new QAction(tr("Build and run"), this);
    key = keySettings.value("run", "default").toString();
    stdKey = QKeySequence(QString("F9"));
    if (key == "default")
        key = stdKey.toString();
    runAction->setShortcut(key);
    //connect(runAction, SIGNAL(triggered()), this, SLOT(runProgram()));

    runExeAction = new QAction(tr("Run in new window"), this);
    //connect(runExeAction, SIGNAL(triggered()), this, SLOT(runExeProgram()));

    stopAction = new QAction(tr("Stop"), this);
    //connect(stopAction, SIGNAL(triggered()), this, SLOT(stopProgram()));
    //! Enable in runProgram(), disable in stop
    stopAction->setEnabled(false);

    debugAction = new QAction(tr("Debug"), this);
    key = keySettings.value("debug", "default").toString();
    stdKey = QKeySequence(QString("F5"));
    if (key == "default")
        key = stdKey.toString();
    debugAction->setShortcut(key);
    debugKey = key;
    //connect(debugAction, SIGNAL(triggered()), this, SLOT(debug()));

    debugNextNiAction = new QAction(tr("Step over"), this);
    key = keySettings.value("stepOver", "default").toString();
    stdKey = QKeySequence(QString("F10"));
    if (key == "default")
        key = stdKey.toString();
    debugNextNiAction->setShortcut(key);
    //connect(debugNextNiAction, SIGNAL(triggered()), this, SLOT(debugNextNi()));

    debugNextAction = new QAction(tr("Step into"), this);
    key = keySettings.value("stepInto", "default").toString();
    stdKey = QKeySequence(QString("F11"));
    if (key == "default")
        key = stdKey.toString();
    debugNextAction->setShortcut(key);
    //connect(debugNextAction, SIGNAL(triggered()), this, SLOT(debugNext()));

    debugShowRegistersAction = new QAction(tr("Show registers"), this);
    key = keySettings.value("showRegisters", "default").toString();
    stdKey = QKeySequence(QString("Ctrl+R"));
    if (key == "default")
        key = stdKey.toString();
    debugShowRegistersAction->setShortcut(key);
    debugShowRegistersAction->setCheckable(true);
    //connect(debugShowRegistersAction, SIGNAL(toggled(bool)), this, SLOT(debugShowRegisters()), Qt::QueuedConnection);

    debugShowMemoryAction = new QAction(tr("Show memory"), this);
    key = keySettings.value("showMemory", "default").toString();
    stdKey = QKeySequence(QString("Ctrl+M"));
    if (key == "default")
        key = stdKey.toString();
    debugShowMemoryAction->setShortcut(key);
    debugShowMemoryAction->setCheckable(true);
    //connect(debugShowMemoryAction, SIGNAL(toggled(bool)), this, SLOT(debugShowMemory()), Qt::QueuedConnection);

    // disableDebugActions(true);
}

void MainWindow::closeFile()
{
    deleteTab(tabs->currentIndex());
}

bool MainWindow::saveFile(int index, bool openSaveAs)
{
    Tab *tab;
    if (index == -1)
        tab = (Tab *) tabs->currentWidget();
    else
        tab = (Tab *) tabs->widget(index);
    QString filePath = tab->getCurrentFilePath();
    if (filePath.isEmpty()) {
        if (openSaveAs) {
            return saveAsFile(index);
        } else {
            return false;
        }
    } else {
        tab->saveCodeToFile(filePath);
        return true;
    }
}

bool MainWindow::saveAsFile(int index)
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), saveOpenDirectory,
                                                    tr("Assembler source files (*.asm);;All files (*)"));
    if (fileName.isEmpty()) {
        return false;
    }
    saveOpenDirectory = QFileInfo(fileName).absoluteDir().absolutePath();
    Tab *tab;
    if (index == -1)
        tab = (Tab *) tabs->currentWidget();
    else
        tab = (Tab *) tabs->widget(index);
    tab->saveCodeToFile(fileName);
    setTabPath(fileName, index);
    return true;
}

bool MainWindow::deleteTab(int index, bool saveFileName)
{
    settings.setValue("tabgeometry", ((Tab *) tabs->widget(index))->saveGeometry());
    settings.setValue("tabwindowstate", ((Tab *) tabs->widget(index))->saveState());
    if (tabs->count() == 1) {
      //! Get started
      mainWidget->setCurrentIndex(0);
    }
    Tab *tabForDeleting = (Tab *) tabs->widget(index);
    if (!saveFileName || tabForDeleting->getCurrentFilePath().isEmpty()) {
        tabs->removeTab(index);
        delete tabForDeleting;
    } else {
        //! If tab did not delete
        tabs->setCurrentIndex(tabs->currentIndex() + 1);
    }
    return true;
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(newAction);
    fileMenu->addSeparator();
    fileMenu->addAction(openAction);
    fileMenu->addAction(closeAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    buildMenu = menuBar()->addMenu(tr("Build"));
    buildMenu->addAction(buildAction);
    buildMenu->addSeparator();
    buildMenu->addAction(runAction);
    #ifdef Q_OS_WIN32
        buildMenu->addAction(runExeAction);
    #endif
    buildMenu->addSeparator();
    buildMenu->addAction(stopAction);
    debugMenu = menuBar()->addMenu(tr("Debug"));
    debugMenu->addAction(debugAction);
    debugMenu->addSeparator();
    debugMenu->addAction(debugNextNiAction);
    debugMenu->addAction(debugNextAction);
    debugMenu->addSeparator();
    debugMenu->addAction(debugShowRegistersAction);
    debugMenu->addAction(debugShowMemoryAction);
    debugMenu->addSeparator();
    debugMenu->addAction(stopAction);
}

void MainWindow::newFile()
{
    if (tabs->count() > 0) {
        settings.setValue("tabgeometry", ((Tab *) tabs->currentWidget())->saveGeometry());
        settings.setValue("tabwindowstate", ((Tab *) tabs->currentWidget())->saveState());
    }
    //! Tabs
    mainWidget->setCurrentIndex(1);
    Tab *tab = new Tab;
    tabs->addTab(tab, tr("New"));
    tabs->setCurrentWidget(tab);
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), saveOpenDirectory,
                                                    tr("Assembler source files (*.asm);;All files (*)"));
    if (fileName.isEmpty()) {
        return;
    }
    saveOpenDirectory = QFileInfo(fileName).absoluteDir().absolutePath();
    openFile(fileName);
}

void MainWindow::openFile(QString path)
{
    if (path.isEmpty()) {
        return;
    }

    newFile();
    Tab *curTab = (Tab *) tabs->currentWidget();
    curTab->loadCodeFromFile(path);
    setTabPath(path);
}

void MainWindow::setTabPath(const QString &filePath, int index)
{
    //! Taking name of file (without path)
    int i;
    if ((i = filePath.lastIndexOf('/')) == -1)
        i = filePath.lastIndexOf('\\');
    QString caption = filePath.mid(i + 1);
    if (index == -1) {
        tabs->setTabToolTip(tabs->currentIndex(), filePath);
        tabs->setTabText(tabs->currentIndex(), caption);
    } else {
        tabs->setTabToolTip(index, filePath);
        tabs->setTabText(index, caption);
    }
}
