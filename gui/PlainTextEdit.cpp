/**
 * @file ruplaintextedit.cpp
 * Implements the text editor base class
 */

#include "PlainTextEdit.h"


PlainTextEdit::PlainTextEdit(QWidget *parent) :
    QPlainTextEdit(parent)
{
    QSettings settings("CSE 661", "Homework2");
    QPalette palette = this->palette();
    palette.setColor(QPalette::Base, settings.value("backgroundcolor", QColor(255, 255, 255)).value<QColor>());
    palette.setColor(QPalette::Text, settings.value("fontcolor", QColor(30, 30, 30)).value<QColor>());
    setPalette(palette);

    setDebugDisabled();

    QSettings keySettings(Common::applicationDataPath() + "/keys.ini", QSettings::IniFormat);

    commentAction = new QAction(tr("Comment"), this);
    QString key = keySettings.value("comment", "default").toString();
    QKeySequence stdKey = QKeySequence(QString("Shift+Ctrl+A"));
    if (key == "default")
        key = stdKey.toString();
    commentAction->setShortcut(key);
    connect(commentAction, SIGNAL(triggered()), this, SLOT(commentSelectedCode()));

    uncommentAction = new QAction(tr("Remove comment"), this);
    key = keySettings.value("uncomment", "default").toString();
    stdKey = QKeySequence(QString("Shift+Ctrl+Q"));
    if (key == "default")
        key = stdKey.toString();
    uncommentAction->setShortcut(key);
    connect(uncommentAction, SIGNAL(triggered()), this, SLOT(uncommentSelectedCode()));

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

    addWatchAction = new QAction(this);
    connect(addWatchAction, SIGNAL(triggered()), this, SLOT(addWatch()));

    undoAction->setEnabled(false);
    redoAction->setEnabled(false);
    connect(this, SIGNAL(undoAvailable(bool)), undoAction, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(redoAvailable(bool)), redoAction, SLOT(setEnabled(bool)));
}

QMenu * PlainTextEdit::createMenu()
{
    QMenu *menu = new QMenu;
    QTextCursor textCursor = this->textCursor();

    //! if nothing selected
    if (textCursor.selectionEnd() - textCursor.selectionStart() <= 0) {
        commentAction->setEnabled(false);
        uncommentAction->setEnabled(false);
        cutAction->setEnabled(false);
        copyAction->setEnabled(false);
        deleteAction->setEnabled(false);
    } else {
        commentAction->setEnabled(true);
        uncommentAction->setEnabled(true);
        cutAction->setEnabled(true);
        copyAction->setEnabled(true);
        deleteAction->setEnabled(true);
    }

    if (isReadOnly()) {
        cutAction->setVisible(false);
        pasteAction->setVisible(false);
        deleteAction->setVisible(false);
    } else {
        cutAction->setVisible(true);
        pasteAction->setVisible(true);
        deleteAction->setVisible(true);
    }

    QString variableName = variableOnCurrentLine().name;
    if (variableName.isEmpty()) {
        addWatchAction->setVisible(false);
    } else {
        addWatchAction->setVisible(true);
        addWatchAction->setText(tr("Watch '%1'").arg(variableName));
        if (!debugEnabled) {
            addWatchAction->setEnabled(false);
        } else {
            addWatchAction->setEnabled(true);
        }
    }

    menu->addAction(addWatchAction);
    menu->addSeparator();
    menu->addAction(commentAction);
    menu->addAction(uncommentAction);
    menu->addSeparator();
    menu->addAction(undoAction);
    menu->addAction(redoAction);
    menu->addSeparator();
    menu->addAction(cutAction);
    menu->addAction(copyAction);
    menu->addAction(pasteAction);
    menu->addAction(deleteAction);
    menu->addSeparator();
    menu->addAction(selectAllAction);

    return menu;
}

void PlainTextEdit::contextMenuEvent(QContextMenuEvent *e)
{
    QMouseEvent leftClick(QEvent::MouseButtonPress, e->pos(), Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    mousePressEvent(&leftClick);
    contextMenu = createMenu();
    contextMenu->exec(e->globalPos());
    if (!contextMenu.isNull())
        delete contextMenu;
}

void PlainTextEdit::deleteSelected()
{
    QTextCursor textCursor = this->textCursor();
    textCursor.removeSelectedText();
}

void PlainTextEdit::commentSelectedCode()
{
    QTextCursor cursor = textCursor();
    QString selected = cursor.selectedText().replace(QString(QChar::ParagraphSeparator),
                                                    QString(QChar::ParagraphSeparator) + QString(';'));
    selected.insert(0, QChar(';'));
    cursor.removeSelectedText();
    cursor.insertText(selected);
}

void PlainTextEdit::uncommentSelectedCode()
{
    QTextCursor cursor = textCursor();
    QString selected = cursor.selectedText().replace(QString(QChar::ParagraphSeparator) + QString(';'),
                                                     QString(QChar::ParagraphSeparator));
    if (selected[0] == QChar(';'))
        selected.remove(0, 1);
    cursor.removeSelectedText();
    cursor.insertText(selected);
}

PlainTextEdit::Watch PlainTextEdit::variableOnCurrentLine()
{
    QTextBlock block = document()->findBlock(textCursor().position());
    QString text = block.text();
    QString variableName;
    int variableSize = 0;
    int arraySize = 0;
    Watch w;
    w.name = variableName;
    w.type = 0;
    w.size = variableSize;
    if (arraySize == 1)
        arraySize = 0;
    w.arraySize = arraySize;
    w.address = false;
    return w;
}

void PlainTextEdit::addWatch()
{
    emit addWatchSignal(variableOnCurrentLine());
}

void PlainTextEdit::setDebugDisabled()
{
    debugEnabled = false;
}

void PlainTextEdit::setDebugEnabled()
{
    debugEnabled = true;
}

PlainTextEdit::~PlainTextEdit()
{
    delete commentAction;
    delete uncommentAction;
    delete undoAction;
    delete redoAction;
    delete cutAction;
    delete copyAction;
    delete pasteAction;
    delete deleteAction;
    delete selectAllAction;
    delete addWatchAction;
}
