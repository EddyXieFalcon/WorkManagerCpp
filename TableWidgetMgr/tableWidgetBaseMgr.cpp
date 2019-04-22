#include "tableWidgetBaseMgr.h"

TableWidgetBaseMgr::TableWidgetBaseMgr(QObject *parent) :
    QObject(parent)
{

}

TableWidgetBaseMgr::~TableWidgetBaseMgr()
{

}

void TableWidgetBaseMgr::SetTableWidget(QTableWidget* table)
{
    this->tableWidget = table;
}

//添加一个新的配置集/配置
void TableWidgetBaseMgr::AddSetting(QString settingName)
{
    //先统计一下总行数
    int rowIndex = this->tableWidget->rowCount();
    //新添加一行
    this->tableWidget->setRowCount(rowIndex + 1);
    //为新的一行添加文本
    this->tableWidget->setItem(rowIndex, 0, new QTableWidgetItem(settingName));
    //自动将新添加的一行选中
    this->tableWidget->setCurrentCell(rowIndex, 0);
}

//删除一个配置集/配置
void TableWidgetBaseMgr::DelSetting()
{
    //必须有一个配置集被选中
    if (0 <= this->tableWidget->currentRow())
    {
        //删除当前选中的配置集
        this->tableWidget->removeRow(this->tableWidget->currentRow());

        //默认将最后一行选中
        this->tableWidget->setCurrentCell(this->tableWidget->rowCount() - 1 , 0);

        return;
    }

    //如果只有一行，清空表
    if (0 == this->tableWidget->currentRow())
    {
        this->tableWidget->clearContents();
        return;
    }
}

//编辑一个配置集/配置
void TableWidgetBaseMgr::EditSetting(QString settingName)
{
    //必须有一个配置集被选中
    if (0 <= this->tableWidget->currentRow())
    {
        //删除当前选中的配置集
        this->tableWidget->item(this->tableWidget->currentRow(), 0)->setText(settingName);
    }

    //默认将最后一行选中
    this->tableWidget->setCurrentCell(this->tableWidget->rowCount() - 1 , 0);
}

//添加一组配置集/配置
void TableWidgetBaseMgr::AddUnitMap(QMap<QString, QString>* unitMap)
{
    //先清空表格
    this->tableWidget->clearContents();

    //重新设置表格大小
    this->tableWidget->setRowCount(unitMap->size());

    //循环遍历map
    int row = 0;
    for (QMap<QString, QString>::iterator iter = unitMap->begin(); iter != unitMap->end(); iter++, row++)
    {
        //添加内容
        this->tableWidget->setItem(row, 0, new QTableWidgetItem(iter.key()));
    }

    //默认将最后一行选中
    this->tableWidget->setCurrentCell(this->tableWidget->rowCount() - 1 , 0);
}

//清空表
void TableWidgetBaseMgr::ClearContents()
{
    this->tableWidget->clearContents();
}

//是否有配置集被选中
bool TableWidgetBaseMgr::HasSelectedSetting()
{
    return 0 <= this->tableWidget->currentRow();
}

//获取当前行的文本信息
QString TableWidgetBaseMgr::GetCurrentSetting()
{
    //如果当前有选中
    if (0 <= this->tableWidget->currentRow())
    {
        return this->tableWidget->currentItem()->text();
    }
    //如果当前没有选中
    else
    {
        return QString("");
    }
}
