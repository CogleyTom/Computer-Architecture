/**
 * @file debugtablewidget.cpp
 * Implements the debugging memory or registers window
 */

#include "debugtablewidget.h"

QByteArray DebugTableWidget::memoryHeaderState;
bool DebugTableWidget::geometryRegistersSaved;
bool DebugTableWidget::geometryMemorySaved;
QByteArray DebugTableWidget::registerWindowState;

DebugTableWidget::DebugTableWidget(int rows, int columns, DebugTableWidgetType widgetType, QWidget *parent) :
    QTableWidget(rows, columns, parent)
{
    empty = true;
    type = widgetType;
    firstTime = true;
    verticalHeader()->hide();

    if (type == memoryTable) {
        setSelectionMode(QAbstractItemView::NoSelection);
        QStringList header;
        header << tr("Variable or expression") << tr("Value") << tr("Type");
        setHorizontalHeaderLabels(header);
        setWindowTitle(tr("Memory"));
        horizontalHeader()->setHighlightSections(false);
        resizeColumnsToContents();
    }

    if (type == registersTable) {
        setSelectionMode(QAbstractItemView::NoSelection);
        QStringList header;
        header << tr("Register") << tr("Hex") << tr("Info");
        setHorizontalHeaderLabels(header);
        setWindowTitle(tr("Registers"));
        resizeColumnsToContents();
        if (geometryRegistersSaved)
            restoreGeometry(registerWindowState);
    }
}

void DebugTableWidget::initializeMemoryWindow(const QList<PlainTextEdit::Watch> &watches)
{
    if (type == memoryTable) {
        horizontalHeader()->restoreState(memoryHeaderState);
        int i;
        for (i = 0; i < watches.size(); i++)
            addVariable(watches[i], i);
        addVariable(tr("Add variable..."), i);
        connect(this, SIGNAL(cellChanged(int,int)), this, SLOT(changeMemoryWindow(int,int)), Qt::QueuedConnection);
    }
}

void DebugTableWidget::setRegisterValues(QList<RegistersInfo> registers) //registers
{
    for (int i = 0; i < registers.size(); i++) {
      addRegister(registers[i].name, registers[i].hexValue, registers[i].decValue, i);
    }
    show();
    if (firstTime) {
        firstTime = false;
        activateWindow();
    }
}

void DebugTableWidget::changeMemoryWindow(int row, int column)
{
    disconnect(this, SIGNAL(cellChanged(int,int)), this, SLOT(changeMemoryWindow(int,int)));
    QString validText = item(row, 0)->text().remove("\\");
    item(row, 0)->setText(validText);
    if (row == rowCount() - 1 && column == 0)
        addVariable(tr("Add variable..."), rowCount());
    if (column == 0)
        emit debugShowMemory();
    connect(this, SIGNAL(cellChanged(int,int)), this, SLOT(changeMemoryWindow(int,int)), Qt::QueuedConnection);
}

void DebugTableWidget::changeVariableValue(const QString &value, int rowNumber, bool isValid)
{
    if (type == memoryTable) {
        if (isValid)
            item(rowNumber, 1)->setText(value);
        else
            item(rowNumber, 1)->setText(tr("Wrong variable or address: \"%1\"").arg(item(rowNumber, 0)->text()));
        resizeColumnsToContents();
    }
}

void DebugTableWidget::deleteVariable()
{
    if (contextMenuLineNumber >= rowCount() - 1) //last line
        return;
    removeRow(contextMenuLineNumber);
    resizeColumnsToContents();
}

void DebugTableWidget::addVariable(const QString &variableName, int rowNumber)
{
    empty = false;
    if (type == memoryTable) {
        if (rowNumber == -1)
            rowNumber = rowCount() - 1;
        insertRow(rowNumber);
        QTableWidgetItem *name = new QTableWidgetItem(variableName);
        QTableWidgetItem *value = new QTableWidgetItem;
        setItem(rowNumber, 0, name);
        setItem(rowNumber, 1, value);
        resizeColumnsToContents();
    }
}

void DebugTableWidget::addVariable(const PlainTextEdit::Watch &variable, int rowNumber)
{
    empty = false;
    if (type == memoryTable) {
        if (rowNumber == -1)
            rowNumber = rowCount() - 1;
        insertRow(rowNumber);
        QTableWidgetItem *name = new QTableWidgetItem(variable.name);
        QTableWidgetItem *value = new QTableWidgetItem;
        setItem(rowNumber, 0, name);
        setItem(rowNumber, 1, value);
        resizeColumnsToContents();
    }
}

void DebugTableWidget::addRegister(const QString &name, const QString &hexValue, const QString &decValue, int rowNumber)
{
    empty = false;
    if (type == registersTable) {
        if (item(rowNumber, 2)) {
            item(rowNumber, 0)->setText(name);
            if (hexValue.isEmpty())
                item(rowNumber, 1)->setText("");
            else
                item(rowNumber, 1)->setText(hexValue);
            item(rowNumber, 2)->setText(decValue);
        } else {
            QTableWidgetItem *nameItem = new QTableWidgetItem(name);
            QTableWidgetItem *hexValueItem;
            if (hexValue.isEmpty())
                hexValueItem = new QTableWidgetItem("");
            else
                hexValueItem = new QTableWidgetItem(hexValue);
            QTableWidgetItem *decValueItem = new QTableWidgetItem(decValue);
            QFont monoFont("Courier");
            monoFont.setStyleHint(QFont::Monospace);
            hexValueItem->setFont(monoFont);
            decValueItem->setFont(monoFont);
            setItem(rowNumber, 0, nameItem);
            setItem(rowNumber, 1, hexValueItem);
            setItem(rowNumber, 2, decValueItem);
        }
        resizeColumnsToContents();
    }
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
    if (event->button() == Qt::RightButton && type == memoryTable) {
        QTableWidgetItem *clicked_item = itemAt(event->pos());
        if (!clicked_item) // background clicked
            return;
        contextMenuLineNumber = clicked_item->row();
        setCurrentCell(contextMenuLineNumber, 0);
        if (contextMenuLineNumber >= rowCount() - 1) //last line
            return;
        QMenu *menu = new QMenu;
        QAction *deleteAction = new QAction(tr("Delete watch"), this);
        connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteVariable()));
        menu->addAction(deleteAction);
        menu->exec(event->globalPos());
        delete deleteAction;
        delete menu;
    } else
        QTableWidget::mousePressEvent(event);
}

void DebugTableWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        contextMenuLineNumber = currentIndex().row();
        deleteVariable();
    } else
        QTableWidget::keyPressEvent(event);
}

DebugTableWidget::~DebugTableWidget()
{
    if (type == memoryTable) {
        memoryHeaderState = horizontalHeader()->saveState();
        geometryMemorySaved = true;
    }
    if (type == registersTable) {
        registerWindowState = saveGeometry();
        geometryRegistersSaved = true;
    }
}
