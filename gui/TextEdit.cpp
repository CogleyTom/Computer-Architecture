/**
 * @file TextEdit.cpp
 * Implemets text editor
 */

#include "TextEdit.h"

TextEdit::TextEdit(QWidget *parent) :
    QTextEdit(parent)
{
    setAcceptRichText(false);
    QSettings settings("CSE 661", "Homework2");
    QPalette palette = this->palette();
    palette.setColor(QPalette::Base, settings.value("backgroundcolor", QColor(255, 255, 255)).value<QColor>());
    palette.setColor(QPalette::Text, settings.value("fontcolor", QColor(30, 30, 30)).value<QColor>());
    setPalette(palette);

    undoAction = new QAction(tr("Undo"), this);
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, SIGNAL(triggered()), this, SLOT(undo()));

    redoAction = new QAction(tr("Redo"), this);
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, SIGNAL(triggered()), this, SLOT(redo()));

    cutAction = new QAction(tr("Cut"), this);
    cutAction->setShortcut(QKeySequence::Cut);
    connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));

    copyAction = new QAction(tr("Copy"), this);
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAction = new QAction(tr("Paste"), this);
    pasteAction->setShortcut(QKeySequence::Paste);
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    deleteAction = new QAction(tr("Delete"), this);
    deleteAction->setShortcut(QKeySequence::Delete);
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteSelected()));

    selectAllAction = new QAction(tr("Select all"), this);
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    connect(selectAllAction, SIGNAL(triggered()), this, SLOT(selectAll()));

    clearAction = new QAction(tr("Clear"), this);
    connect(clearAction, SIGNAL(triggered()), this, SLOT(clear()));

    undoAction->setEnabled(false);
    redoAction->setEnabled(false);
    connect(this, SIGNAL(undoAvailable(bool)), undoAction, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(redoAvailable(bool)), redoAction, SLOT(setEnabled(bool)));
}

void TextEdit::contextMenuEvent(QContextMenuEvent *e)
{
    contextMenu = new QMenu;
    QTextCursor textCursor = this->textCursor();

    //!if nothing selected
    if (textCursor.selectionEnd() - textCursor.selectionStart() <= 0) {
        cutAction->setEnabled(false);
        copyAction->setEnabled(false);
        deleteAction->setEnabled(false);
    } else {
        cutAction->setEnabled(true);
        copyAction->setEnabled(true);
        deleteAction->setEnabled(true);
    }

    if (isReadOnly()) {
        undoAction->setVisible(false);
        redoAction->setVisible(false);
        cutAction->setVisible(false);
        pasteAction->setVisible(false);
        deleteAction->setVisible(false);
        clearAction->setVisible(true);
    } else {
        undoAction->setVisible(true);
        redoAction->setVisible(true);
        cutAction->setVisible(true);
        pasteAction->setVisible(true);
        deleteAction->setVisible(true);
        clearAction->setVisible(false);
    }

    contextMenu->addAction(undoAction);
    contextMenu->addAction(redoAction);
    contextMenu->addSeparator();
    contextMenu->addAction(cutAction);
    contextMenu->addAction(copyAction);
    contextMenu->addAction(pasteAction);
    contextMenu->addAction(deleteAction);
    contextMenu->addSeparator();
    contextMenu->addAction(selectAllAction);
    contextMenu->addAction(clearAction);

    contextMenu->exec(e->globalPos());
    if (!contextMenu.isNull())
        delete contextMenu;
}

void TextEdit::deleteSelected()
{
    QTextCursor textCursor = this->textCursor();
    textCursor.removeSelectedText();
}

TextEdit::~TextEdit()
{
    delete undoAction;
    delete redoAction;
    delete cutAction;
    delete copyAction;
    delete pasteAction;
    delete deleteAction;
    delete selectAllAction;
    delete clearAction;
}
