/**
 * @file Tab.h
 * Defines the Tab class.
 */

#ifndef TAB_H
#define TAB_H

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSettings>
#include <QTextStream>
#include <QFile>
#include <QMainWindow>
#include <QDockWidget>
#include "CodeEditor.h"
#include "TextEdit.h"

/*! \brief The Tab class represents tab in window.
*/

class Tab : public QMainWindow
{
    Q_OBJECT

public:
    explicit Tab(QWidget *parent = 0);
    virtual ~Tab();

    QTextDocument *getCodeDocument();
    void saveCodeToFile(const QString &filePath, bool changeCodeModifiedFlag = true, bool debugMode = false);
    void saveInputToFile(const QString &filePath);
    void loadOutputFromFile(const QString &filePath);
    void loadCodeFromFile(const QString &filePath);
    void appendOutput(QString msg);
    QString getCurrentFilePath();
    void clearOutput();
    void setFonts();

    CodeEditor *code;


private: 
    //! Text fields
    QVBoxLayout *inputLayout;
    QVBoxLayout *outputLayout;
    TextEdit *input;
    TextEdit *output;
    QWidget *inputWidget;
    QWidget *outputWidget;

    QString currentFilePath;
};

#endif // TAB_H
