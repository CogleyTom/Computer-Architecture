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
    void loadCodeFromFile(const QString &filePath);
    QString getCurrentFilePath();
    void clearOutput();
    void setFonts();
    void printInstruction(const QString &code);
    void printMemory(const QString &mem);

    CodeEditor *code;


private: 
    //! Text fields
    QVBoxLayout *instructionLayout;
    QVBoxLayout *memoryLayout;
    TextEdit *instruction;
    TextEdit *memory;
    QWidget *instructionWidget;
    QWidget *memoryWidget;

    QString currentFilePath;
};

#endif // TAB_H
