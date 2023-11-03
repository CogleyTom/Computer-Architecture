/**
 * @file debugtablewidget.cpp
 * Implements the debugging memory or registers window
 */

#include "debugtablewidget.h"

QByteArray DebugTableWidget::memoryHeaderState;
bool DebugTableWidget::geometryRegistersSaved;
bool DebugTableWidget::geometryMemorySaved;
QByteArray DebugTableWidget::registerWindowState;

DebugTableWidget::DebugTableWidget(int rows, int columns, QWidget *parent) :
    QTableWidget(rows, columns, parent)
{
    empty = true;
    firstTime = true;
    verticalHeader()->hide();

    setSelectionMode(QAbstractItemView::NoSelection);
    QStringList header;
    header << tr("Register") << tr("Value");
    setHorizontalHeaderLabels(header);
    setWindowTitle(tr("Registers"));
    resizeColumnsToContents();
    if (geometryRegistersSaved) {
        restoreGeometry(registerWindowState);
    }
}

void DebugTableWidget::setRegisterValues(QList<RegistersInfo> registers) //registers
{
    for (int i = 0; i < registers.size(); i++) {
      addRegister(registers[i].name, registers[i].decValue, i);
    }
    show();
    if (firstTime) {
        firstTime = false;
        activateWindow();
    }
}

void DebugTableWidget::addRegister(const QString &name, const QString &decValue, int rowNumber)
{
    empty = false;
    if (item(rowNumber, 2)) {
        item(rowNumber, 0)->setText(name);
        item(rowNumber, 1)->setText(decValue);
    } else {
        QTableWidgetItem *nameItem = new QTableWidgetItem(name);
        QTableWidgetItem *decValueItem = new QTableWidgetItem(decValue);
        QFont monoFont("Courier");
        monoFont.setStyleHint(QFont::Monospace);
        decValueItem->setFont(monoFont);
        setItem(rowNumber, 0, nameItem);
        setItem(rowNumber, 1, decValueItem);
    }
    resizeColumnsToContents();
}

bool DebugTableWidget::isEmpty()
{
    return empty;
}

void DebugTableWidget::closeEvent(QCloseEvent *) {
    emit closeSignal();
}

void DebugTableWidget::mousePressEvent(QMouseEvent *event)
{
    QTableWidget::mousePressEvent(event);
}

void DebugTableWidget::keyPressEvent(QKeyEvent *event)
{
    QTableWidget::keyPressEvent(event);
}

DebugTableWidget::~DebugTableWidget()
{
    registerWindowState = saveGeometry();
    geometryRegistersSaved = true;
}
