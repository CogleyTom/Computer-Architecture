/**
 * @file CodeEditor.h
 * Contains definitions for objects pertaining to the code editing section
 */

#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPainter>
#include <QTextBlock>
#include <QScrollBar>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include "PlainTextEdit.h"
#include <QUrl>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class LineNumberArea;

class CodeEditor : public PlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = 0, bool withBeakpoints = true);
    ~CodeEditor();

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    void lineNumberAreaMousePressEvent(QMouseEvent *event);
    int lineNumberAreaWidth();
    void repaintLineNumberArea();
    int currentDebugLine;
    bool debugMode;

public slots:
    void updateDebugLine(int number);
    void putTab();
    void deleteTab();
    void highlightCurrentLine();
    void highlightDebugLine(int lineNumber);
    void setDebugMode(bool mode);
    QList<int> *getBreakpoints();
    void setBreakpointOnCurrentLine();

protected:
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *e);

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    
private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &, int);
    void shiftBreakpoints(int blockCount);

private:
    QWidget *lineNumberArea;
    int debugAreaWidth;
    QPixmap debugImage;
    QPixmap breakpointImage;
    //! Breakpoint line numbers
    QList<int> breakpoints;
    int firstTopMargin;
    bool hasBreakpoints;
    int prevBlockCount;
    QSettings settings;
    QColor lineNumberPanelColor;

signals:
    void breakpointsChanged(quint64 lineNumber, bool isAdded);
    void fileOpened(QString path);
};


class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) {
        codeEditor->lineNumberAreaPaintEvent(event);
    }
    void mousePressEvent(QMouseEvent *event) {
        codeEditor->lineNumberAreaMousePressEvent(event);
    }

private:
    CodeEditor *codeEditor;
};

#endif
