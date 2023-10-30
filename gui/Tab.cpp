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

    //! Setting opcode and output fields
    input = new TextEdit;
    output = new TextEdit;
    input->setReadOnly(true);
    output->setReadOnly(true);
    inputLayout = new QVBoxLayout;
    outputLayout = new QVBoxLayout;
    inputLayout->addWidget(input);
    outputLayout->addWidget(output);

    setDockOptions(QMainWindow::AllowNestedDocks | QMainWindow::AnimatedDocks);

    QDockWidget *inputDock = new QDockWidget(tr("Input"), this);
    inputDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    inputWidget = new QWidget(inputDock);
    inputWidget->setLayout(inputLayout);
    inputDock->setWidget(inputWidget);
    inputDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::RightDockWidgetArea, inputDock);
    inputDock->setObjectName("inputDock");

    QDockWidget *outputDock = new QDockWidget(tr("Output"), this);
    outputDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    outputWidget = new QWidget(outputDock);
    outputWidget->setLayout(outputLayout);
    outputDock->setWidget(outputWidget);
    outputDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::RightDockWidgetArea, outputDock);
    outputDock->setObjectName("outputDock");

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
    input->setFont(logFont);
    output->setFont(logFont);
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
    out << input->toPlainText();
    outfile.close();
}

/*! \brief
 *
 *
*/
void Tab::loadOutputFromFile(const QString &filePath)
{
    QFile outputFile;
    outputFile.setFileName(filePath);
    outputFile.open(QIODevice::ReadOnly);
    QTextStream programOut(&outputFile);
    QString out = programOut.readAll();
    output->setPlainText(out);
    outputFile.close();
}

/*! \brief
 *
 *
*/
void Tab::appendOutput(QString msg)
{
    msg = output->toPlainText().right(2000) + msg.right(2000);
    msg = msg.right(2000);
    output->setText(msg);
    //! Scroll
    QTextCursor cursor = output->textCursor();
    cursor.movePosition(QTextCursor::End);
    output->setTextCursor(cursor);
}

/*! \brief
 *
 *
*/
void Tab::clearOutput()
{
    output->clear();
}
/*! \brief Destructor for the Tab objects.
 *
 *
*/
Tab::~Tab()
{
    delete input;
    delete output;

    delete inputLayout;
    delete outputLayout;

    delete code;

    delete inputWidget;
    delete outputWidget;
}
