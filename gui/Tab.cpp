/**
 * @file Tab.cpp
 * Implementation of the Tab class.
 */

#include "tab.h"

Tab::Tab(QWidget *parent) :
    QMainWindow(parent),
    currentFilePath("")
{
    //! Setting code field
    code = new CodeEditor;
    setCentralWidget(code);

    //! Setting instruction and memory fields
    instruction = new TextEdit;
    memory = new TextEdit;
    instruction->setReadOnly(true);
    memory->setReadOnly(true);
    instructionLayout = new QVBoxLayout;
    memoryLayout = new QVBoxLayout;
    instructionLayout->addWidget(instruction);
    memoryLayout->addWidget(memory);

    setDockOptions(QMainWindow::AllowNestedDocks | QMainWindow::AnimatedDocks);

    QDockWidget *instructionDock = new QDockWidget(tr("Instruction"), this);
    instructionDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    instructionWidget = new QWidget(instructionDock);
    instructionWidget->setLayout(instructionLayout);
    instructionDock->setWidget(instructionWidget);
    instructionDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::RightDockWidgetArea, instructionDock);
    instructionDock->setObjectName("instructionDock");

    QDockWidget *memoryDock = new QDockWidget(tr("Memory"), this);
    memoryDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    memoryWidget = new QWidget(memoryDock);
    memoryWidget->setLayout(memoryLayout);
    memoryDock->setWidget(memoryWidget);
    memoryDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::RightDockWidgetArea, memoryDock);
    memoryDock->setObjectName("memoryDock");

    //! Setting io and code fonts
    setFonts();

    //! Restore state
    QSettings settings("CSE 661", "Homework2");
    restoreGeometry(settings.value("tabgeometry").toByteArray());
    restoreState(settings.value("tabwindowstate").toByteArray());
}

/*! \brief
 *
 *
*/
void Tab::setFonts()
{
    QSettings settings("CSE 661", "Homework2");

    QFont codeFont;
    codeFont.setPointSize(settings.value("fontsize", 12).toInt());
    codeFont.setFamily(settings.value("fontfamily", QString("Courier")).value<QString>());
    codeFont.setStyleHint(QFont::Monospace);
    code->setFont(codeFont);
    code->repaintLineNumberArea();

    QFont logFont;
    logFont.setPointSize(settings.value("fontsize", 12).toInt());
    instruction->setFont(logFont);
    memory->setFont(logFont);
}




/*! \brief
 *
 *
*/
QTextDocument * Tab::getCodeDocument()
{
    return code->document();
}

/*! \brief
 *
 *
*/
QString Tab::getCurrentFilePath()
{
    return currentFilePath;
}

/*! \brief
 *
 *
*/
void Tab::saveCodeToFile(const QString &filePath, bool changeCodeModifiedFlag, bool debugMode)
{
    QFile outfile;
    outfile.setFileName(filePath);
    outfile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&outfile);
    out << code->toPlainText();
    if (changeCodeModifiedFlag) {
        currentFilePath = filePath;
        code->document()->setModified(false);
    }
    outfile.close();
}

/*! \brief
 *
 *
*/
void Tab::loadCodeFromFile(const QString &filePath)
{
    QFile file;
    file.setFileName(filePath);
    file.open(QIODevice::ReadOnly);
    QTextStream text(&file);
    // text.setCodec("UTF-8");
    QString source = text.readAll();
    code->setPlainText(source);
    currentFilePath = filePath;
    code->document()->setModified(false);
    file.close();
}

/*! \brief
 *
 *
*/
void Tab::saveInputToFile(const QString &filePath)
{
    QFile outfile;
    outfile.setFileName(filePath);
    outfile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&outfile);
    out << instruction->toPlainText();
    outfile.close();
}

void Tab::printInstruction(const QString &code) {
    instruction->setPlainText(code);
}

void Tab::printMemory(const QString &mem) {
    memory->setPlainText(mem);
}

/*! \brief
 *
 *
*/
void Tab::clearOutput()
{
    memory->clear();
}
/*! \brief Destructor for the Tab objects.
 *
 *
*/
Tab::~Tab()
{
    delete instruction;
    delete memory;

    delete instructionLayout;
    delete memoryLayout;

    delete code;

    delete instructionWidget;
    delete memoryWidget;
}
