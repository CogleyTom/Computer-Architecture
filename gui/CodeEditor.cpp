/**
 * @file CodeEditor.cpp
 * Implements the code editing area
 */

#include "CodeEditor.h"

CodeEditor::CodeEditor(QWidget *parent, bool withBeakpoints) :
    PlainTextEdit(parent), debugImage(":/images/debugLine.png"),
    breakpointImage(":/images/breakpoint.png"),
    settings("CSE 661", "Homework2")
{
    hasBreakpoints = withBeakpoints;
    prevBlockCount = -1;
    setDebugMode(false);
    debugAreaWidth = 3 + debugImage.width() + 1;
    setWordWrapMode(QTextOption::NoWrap);
    firstTopMargin = contentOffset().y();
    lineNumberArea = new LineNumberArea(this);
    lineNumberPanelColor = settings.value("linenumberpanelcolor", QColor(235, 235, 235)).value<QColor>();

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(shiftBreakpoints(int)));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
    currentDebugLine = -1;
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits + debugAreaWidth;

    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
        //! Indirectly invokes CodeEditor::lineNumberAreaPaintEvent() throw virtual LineNumberArea::paintEvent()

    //! Viewport - visible part of widget
    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    // !Paint on line number area
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), lineNumberPanelColor);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(settings.value("linenumberfontcolor", QColor(30, 30, 30)).value<QColor>());
            painter.drawText(0, top, lineNumberArea->width() - debugAreaWidth, fontMetrics().height(),
                             Qt::AlignRight, number);

            if (blockNumber + 1 == currentDebugLine)
                //! Blocks counting starts with 0, line number is equivalent (block number + 1)
                //if QTextOption::NoWrap is not set, lines count - visible lines, block count - lines divided by '\n'
                painter.drawPixmap(lineNumberArea->width() - debugAreaWidth + 3,
                                   top + fontMetrics().height() / 2 - debugImage.height() / 2,
                                   debugImage.width(), debugImage.height(), debugImage);
            if (breakpoints.contains(blockNumber + 1) && hasBreakpoints)
                painter.drawPixmap(lineNumberArea->width() - debugAreaWidth + 3,
                                   top + fontMetrics().height() / 2 - breakpointImage.height() / 2,
                                   breakpointImage.width(), breakpointImage.height(), breakpointImage);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void CodeEditor::lineNumberAreaMousePressEvent(QMouseEvent *event)
{
    if (hasBreakpoints) {
        //! If mouse click has been made - add breakpoint
        //! Counting line number
        int lineNumber = 0;
        int sumHeight = 0;
        QTextBlock block = firstVisibleBlock();
        while (block.isValid() && event->y() > sumHeight) {
            sumHeight += blockBoundingGeometry(block).height();
            block = block.next();
            lineNumber++;
        }
        lineNumber += verticalScrollBar()->value(); //+ invisible lines

        if (lineNumber <= document()->lineCount()) {
            //blocks counting starts with 0
            lineNumber = document()->findBlockByLineNumber(lineNumber - 1).blockNumber() + 1; //line number to paint

            //add or remove line number in list
            if (breakpoints.contains(lineNumber)) {
                breakpoints.removeOne(lineNumber);
                emit breakpointsChanged(lineNumber, false);
            } else {
                breakpoints.append(lineNumber);
                emit breakpointsChanged(lineNumber, true);
            }

            repaintLineNumberArea();
        }
    }
}

void CodeEditor::setBreakpointOnCurrentLine()
{
    if (hasBreakpoints) {
        int lineNumber = textCursor().blockNumber() + 1;
        if (lineNumber <= document()->lineCount()) {
            //blocks counting starts with 0
            lineNumber = document()->findBlockByLineNumber(lineNumber - 1).blockNumber() + 1; //line number to paint

            //add or remove line number in list
            if (breakpoints.contains(lineNumber)) {
                breakpoints.removeOne(lineNumber);
                emit breakpointsChanged(lineNumber, false);
            } else {
                breakpoints.append(lineNumber);
                emit breakpointsChanged(lineNumber, true);
            }

            repaintLineNumberArea();
        }
    }
}

void CodeEditor::repaintLineNumberArea()
{
    //repaint
    QRect lineNumberAreaRect(lineNumberArea->x(), lineNumberArea->y(),
                             lineNumberArea->width(), lineNumberArea->height());
    emit updateRequest(lineNumberAreaRect, 0);
}

QList<int> *CodeEditor::getBreakpoints()
{
    return &breakpoints;
}

void CodeEditor::highlightCurrentLine()
{
    if (!debugMode) {
        if (settings.value("currentlinemode", true).toBool()) {
            QList<QTextEdit::ExtraSelection> extraSelections;

            if (!isReadOnly()) {
                QTextEdit::ExtraSelection selection;

                QColor lineColor = settings.value("currentlinecolor", QColor(232, 232, 255)).value<QColor>();

                selection.format.setBackground(lineColor);
                selection.format.setProperty(QTextFormat::FullWidthSelection, true);
                selection.cursor = textCursor();
                selection.cursor.clearSelection();
                extraSelections.append(selection);
            }

            setExtraSelections(extraSelections);
        } else {
            QList<QTextEdit::ExtraSelection> extraSelections; //empty
            setExtraSelections(extraSelections);
        }
    }
}

void CodeEditor::highlightDebugLine(int lineNumber)
{
    if (debugMode) {
        QList<QTextEdit::ExtraSelection> extraSelections;

        if (!isReadOnly() && lineNumber > 0) {
            QTextEdit::ExtraSelection selection;

            QColor lineColor = settings.value("debuglinecolor", QColor(235, 200, 40)).value<QColor>();

            selection.format.setBackground(lineColor);
            selection.format.setProperty(QTextFormat::FullWidthSelection, true);
            selection.cursor = QTextCursor(document());
            selection.cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, lineNumber - 1);
            selection.cursor.clearSelection();
            setTextCursor(selection.cursor); //scroll to debugging line
            extraSelections.append(selection);
        }

        setExtraSelections(extraSelections);
    }
}

void CodeEditor::updateDebugLine(int number)
{
    //number > 0 => highlight line
    //number == -1 => exit from debug mode
    //number == -2 => does not highlight any line, but does not exit from debug mode
    //last case for waiting program stops on next instruction or breakpoint
    if (number == -1)
        setDebugMode(false);
    else
        setDebugMode(true);
    if (number != -2)
        currentDebugLine = number;

    //create rectangle of line number area and highlight debug line throw updateRequest()
    QRect lineNumberAreaRect(lineNumberArea->x(), lineNumberArea->y(),
                             lineNumberArea->width(), lineNumberArea->height());
    emit updateRequest(lineNumberAreaRect, 0);

    highlightDebugLine(number);
    highlightCurrentLine();
}

void CodeEditor::setDebugMode(bool mode)
{
    debugMode = mode;
}

void CodeEditor::putTab()
{
    QTextCursor cursor = textCursor();
    if (cursor.selectionEnd() - cursor.selectionStart() <= 0) {
        int spacesToAdd = 4 - cursor.columnNumber() % 4;
        QString insertion(spacesToAdd, ' ');
        insertPlainText(insertion);
    } else {
        QString insertion = "    ";
        QTextBlock firstBlock = document()->findBlock(cursor.selectionStart());
        QTextBlock lastBlock = document()->findBlock(cursor.selectionEnd() - 1);

        cursor.setPosition(firstBlock.position());
        cursor.beginEditBlock();
        do {
            cursor.insertText(insertion);
        } while (cursor.movePosition(QTextCursor::NextBlock) && cursor.position() <= lastBlock.position());
        cursor.endEditBlock();
    }
}

void CodeEditor::deleteTab()
{
    QTextCursor cursor = textCursor();
    if (cursor.selectionEnd() - cursor.selectionStart() <= 0) {
        int spacesToDelete = 4 - (4 - cursor.columnNumber() % 4) % 4;
        for (int i = 0; i < spacesToDelete; ++i) {
            if (cursor.columnNumber() != 0 && document()->characterAt(cursor.position() - 1) == ' ') {
                cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 1);
            } else {
                break;
            }
        }
        cursor.removeSelectedText();
    } else {
        QTextBlock firstBlock = document()->findBlock(cursor.selectionStart());
        QTextBlock lastBlock = document()->findBlock(cursor.selectionEnd() - 1);
        cursor.setPosition(firstBlock.position());
        cursor.beginEditBlock();
        do {
            int spacesToDelete = 4;
            for (int i = 0; i < spacesToDelete; ++i) {
                if (document()->characterAt(cursor.position()) == ' ') {
                    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);
                } else {
                    break;
                }
            }
            cursor.removeSelectedText();
        } while (cursor.movePosition(QTextCursor::NextBlock) && cursor.position() <= lastBlock.position());
        cursor.endEditBlock();
    }
}

void CodeEditor::keyPressEvent(QKeyEvent *e)
{
    QString curString = textCursor().block().text();
    int indentWidth = -1;
    switch (e->key()) {
    case (Qt::Key_Tab) :
        if (! (e->modifiers() & Qt::ShiftModifier))
            putTab();
        break;
    case (Qt::Key_Enter) :
    case (Qt::Key_Return) :
        indentWidth++;
        if(curString.size() > 0) {
          while (curString[indentWidth] == ' ') {
            indentWidth++;
            if (curString.size() == indentWidth) {
              break;
            }
          }
        }
        QPlainTextEdit::keyPressEvent(e);
        for (int i = 0; i < indentWidth; i++)
            insertPlainText(" ");
        break;
    case (Qt::Key_Q) :
        if ((e->modifiers() & Qt::ShiftModifier) && (e->modifiers() & Qt::ControlModifier))
            commentSelectedCode();
        else
            QPlainTextEdit::keyPressEvent(e);
        break;
    case (Qt::Key_A) :
        if ((e->modifiers() & Qt::ShiftModifier) && (e->modifiers() & Qt::ControlModifier))
            uncommentSelectedCode();
        else
            QPlainTextEdit::keyPressEvent(e);
        break;
    default:
        QPlainTextEdit::keyPressEvent(e);
    }
}

void CodeEditor::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    } else {
        PlainTextEdit::dragEnterEvent(event);
    }
}

void CodeEditor::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        foreach (const QUrl &url, event->mimeData()->urls())
        {
            const QString &fileName = url.toLocalFile();

            if (fileName.isEmpty())
                continue;

            emit fileOpened(fileName);
        }
        event->acceptProposedAction();
    } else {
        PlainTextEdit::dropEvent(event);
    }
}

void CodeEditor::shiftBreakpoints(int blockCount)
{
    if (prevBlockCount == -1) {
        prevBlockCount = this->blockCount();
        return;
    }
    //remove breakpoints in range
    int changedLine = textCursor().blockNumber() + 1; //blocks starts from 0
    if (blockCount < prevBlockCount) {
        for (int i = changedLine + 1; i <= changedLine + prevBlockCount - blockCount; i++) {
            breakpoints.removeOne(i);
        }
    }

    //shift breakpoints
    if (blockCount > prevBlockCount) {
        for (int i = 0; i < breakpoints.size(); i++) {
            if (breakpoints[i] >= changedLine)
                breakpoints[i] += blockCount - prevBlockCount;
        }
    } else {
        for (int i = 0; i < breakpoints.size(); i++) {
            if (breakpoints[i] > changedLine)
                breakpoints[i] += blockCount - prevBlockCount;
        }
    }
    prevBlockCount = blockCount;
    repaintLineNumberArea();
}

CodeEditor::~CodeEditor()
{
    delete lineNumberArea;
}
