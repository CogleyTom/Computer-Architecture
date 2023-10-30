/**
 * @file PlainTextEdit.h
 * Text editor base class definition file.
 */

#ifndef PLAINTEXTEDIT_H
#define PLAINTEXTEDIT_H

#include <QPlainTextEdit>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QTextBlock>
#include <QTextStream>
#include <QPointer>
#include <QSettings>
#include "common.h"

/*! \brief This defines the base class which the text editor is derived from.
 *
 * The class contains methods that are used in the editor. These range from simple
 * copying and pasting to enabling/disabling the debugger.
 */

class PlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    //! The class constructor creates the editor, given a specified parent QWidget object.
    PlainTextEdit(QWidget *parent = 0);
    ~PlainTextEdit();

    //! Creates a menu
    QMenu *createMenu();

    //! Defines a structure to keep track of a watched variable.
    struct Watch {
        QString name;
        int type;
        int size;
        int arraySize;
        bool address;
    };

protected:
    void contextMenuEvent(QContextMenuEvent *e);

private:
    PlainTextEdit::Watch variableOnCurrentLine();

    QPointer<QMenu> contextMenu;
    //! Creates a comment
    QAction *commentAction;
    //! Removes a comment
    QAction *uncommentAction;
    //! Undo the last action
    QAction *undoAction;
    //! Do the previous action again.
    QAction *redoAction;
    //! Cut a selected text string.
    QAction *cutAction;
    //! Copy a selected text string.
    QAction *copyAction;
    //! Paste the clipboard contents.
    QAction *pasteAction;
    //! Delete a selected text string.
    QAction *deleteAction;
    //! Select all of the text.
    QAction *selectAllAction;
    //! Add a watch on a variable.
    QAction *addWatchAction;
    //! Used to keep track if the debugger is enabled.
    bool debugEnabled;

public slots:
    //! Method that comments the user selected code.
    void commentSelectedCode();
    //! Method for uncommenting a previously commented code block.
    void uncommentSelectedCode();

    void deleteSelected();
    //! Method for adding a variable watch.
    void addWatch();
    //! Used to enable the debugger.
    void setDebugEnabled();
    //! Used to disable the debugger.
    void setDebugDisabled();

signals:
    //! Signal which emited when watch variable action is selected.
    void addWatchSignal(PlainTextEdit::Watch variable);
};

#endif // PLAINTEXTEDIT_H
