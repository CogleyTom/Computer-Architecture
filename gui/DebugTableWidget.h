/**
 * @file DebugTableWidget.h
 * Defines the debugging window (memory or registers table).
 */

#ifndef DEBUGTABLEWIDGET_H
#define DEBUGTABLEWIDGET_H

#include <QTableWidget>
#include <QHeaderView>
#include <QMouseEvent>
#include <QAction>
#include <QMenu>
#include <QByteArray>
#include "PlainTextEdit.h"

/**
 * ! \brief This class represents memory or registers table.
 *
 * This class contains the methods and variables relevant to memory and register windows under the debugger.
 * Methods include adding variable watches, modifying variable contents, and adding registers.
 * */

class DebugTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    struct RegistersInfo {
        QString name;
        QString decValue;
    };

    explicit DebugTableWidget(int rows, int columns, QWidget *parent = 0);
    ~DebugTableWidget();
    bool isEmpty();
    static QByteArray memoryHeaderState;
    static QByteArray registerWindowState;
    static bool geometryMemorySaved;
    static bool geometryRegistersSaved;

protected:
    void closeEvent(QCloseEvent *);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

signals:
    void closeSignal();

public slots:
    void addRegister(const QString &name, const QString &decValue, int rowNumber);
    void setRegisterValues(QList<RegistersInfo> registers);

private:
    int contextMenuLineNumber;
    bool empty;
    bool firstTime;
};

#endif // DEBUGTABLEWIDGET_H
