/**
 * @file TextEdit.h
 * Defines text editor
 */

#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QPointer>
#include <QSettings>

/*! \brief The TextEdit defines the methods specified by QT's QTextEdit class. These are things like
 * copying/pasting, edit, undo, etc.
 *
 *
*/

class TextEdit : public QTextEdit
{
    Q_OBJECT

public:
    TextEdit(QWidget *parent = 0);
    ~TextEdit();

protected:
    void contextMenuEvent(QContextMenuEvent *e);

private:
    QPointer<QMenu> contextMenu;
    QAction *undoAction;
    QAction *redoAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *deleteAction;
    QAction *selectAllAction;
    QAction *clearAction;

public slots:
    void deleteSelected();
};

#endif // TEXTEDIT_H
