#include "mainwindow.h"
#include "ui_mainwindow.h"

//构造函数
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //创建设置UI
    ui->setupUi(this);

    //准备数据容器
    this->settingMap = new QMap<QString, QMap<QString, QString>*>();

    //构建ui与mgr之间的管理关系
    this->tabletWidgetSettingMgr = new TableWidgetBaseMgr(this);
    this->tabletWidgetSettingMgr->SetTableWidget(ui->tableWidgetSetting);

    this->tabletWidgetUnitMgr = new TableWidgetBaseMgr(this);
    this->tabletWidgetUnitMgr->SetTableWidget(ui->tableWidgetUnit);

    this->jsonMgr = new JsonMgr(this);
    this->jsonMgr->SetMap(this->settingMap);

    //读取已存在的配置文件
    this->jsonMgr->Load2Map();

    //将数据导入界面
    this->LoadMap();

    //连接所有的按钮的点击行为
    this->connect(ui->btnAddSetting, SIGNAL(clicked()), this, SLOT(onBtnAddSettingClickedSlot()));
    this->connect(ui->btnDelSetting, SIGNAL(clicked()), this, SLOT(onBtnDelSettingClickedSlot()));
    this->connect(ui->btnEditSetting, SIGNAL(clicked()), this, SLOT(onBtnEditSettingClickedSlot()));

    this->connect(ui->btnAddUnit, SIGNAL(clicked()), this, SLOT(onBtnAddUnitClickedSlot()));
    this->connect(ui->btnDelUnit, SIGNAL(clicked()), this, SLOT(onBtnDelUnitClickedSlot()));
    this->connect(ui->btnEditUnit, SIGNAL(clicked()), this, SLOT(onBtnEditUnitClickedSlot()));

    this->connect(ui->btnDo, SIGNAL(clicked()), this, SLOT(onBtnDoClickedSlot()));

    //配置集表切换效果
    this->connect(ui->tableWidgetSetting, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetSettingItemClickedSlot(QTableWidgetItem*)));
    this->connect(ui->tableWidgetSetting, SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)), this, SLOT(onTableWidgetSettingCurrentItemChangedSlot(QTableWidgetItem*, QTableWidgetItem*)));
}

//析构函数
MainWindow::~MainWindow()
{
    delete ui;
}

//将数据导入界面
void MainWindow::LoadMap()
{
    //容错，没有表，无需导入
    if(0 == this->settingMap->size()) return;

    //解析map，循环遍历
    for (QMap<QString, QMap<QString, QString>*>::iterator iter = this->settingMap->begin(); iter != this->settingMap->end(); iter++)
    {
        //添加配置集内容表
        this->tabletWidgetSettingMgr->AddSetting(iter.key());
    }
    //加载配置表（节省效率，只需要加载最后一张）
    this->tabletWidgetUnitMgr->AddUnitMap(this->settingMap->last());
}

//添加设置主题
void MainWindow::onBtnAddSettingClickedSlot()
{
    //弹出对话框，要求为配置输入一个名称
    QString name = QInputDialog::getText(this, QStringLiteral("输入一个名称"), QStringLiteral("设置集名称："));
    if(name.isEmpty()) return;

    //判断一下该配置是否存在
    if(this->settingMap->contains(name)) return;

    //先将信息存入Map
    this->settingMap->insert(name, new QMap<QString, QString>());

    //如果成功获取一个名称，要用接口，保存到界面和配置文件中
    this->tabletWidgetSettingMgr->AddSetting(name);

    //记录到json文件中
    this->jsonMgr->Recorde2Json();
}

//删除设置主题
void MainWindow::onBtnDelSettingClickedSlot()
{
    //获取该条数据记录
    QString name = this->tabletWidgetSettingMgr->GetCurrentSetting();
    if (name.isEmpty()) return;

    //先删除map中的数据
    this->settingMap->erase(this->settingMap->find(name));

    //删除ui中的数据
    this->tabletWidgetSettingMgr->DelSetting();

    //记录到json文件中
    this->jsonMgr->Recorde2Json();
}

//编辑设置主题
void MainWindow::onBtnEditSettingClickedSlot()
{
    //先获取要编辑的那条设置集
    QString oldName = this->tabletWidgetSettingMgr->GetCurrentSetting();
    if (oldName.isEmpty()) return;

    //弹出对话框，要求为配置输入一个名称
    QString name = QInputDialog::getText(this, QStringLiteral("输入一个名称"), QStringLiteral("设置集名称："), QLineEdit::Normal, oldName);
    if(name.isEmpty()) return;

    //将map中旧的设置改为新的
    this->settingMap->insert(name, this->settingMap->value(oldName));
    this->settingMap->erase(this->settingMap->find(oldName));

    //将ui中的数据改掉
    this->tabletWidgetSettingMgr->EditSetting(name);

    //记录到json文件中
    this->jsonMgr->Recorde2Json();
}

//添加设置配置
void MainWindow::onBtnAddUnitClickedSlot()
{
    //如果没有当前没有选中然和设置集，不予添加
    QString name = this->tabletWidgetSettingMgr->GetCurrentSetting();
    if (name.isEmpty()) return;

    //添加一个设置
    QString filePath = QFileDialog::getOpenFileName(this, QStringLiteral("打开一个快捷方式"), "C:/ProgramData/Microsoft/Windows/Start Menu/Programs", QStringLiteral("快捷方式(*.lnk)"));
    if (filePath.isEmpty()) return;
    QString fileName = filePath.split("/").last().replace(".lnk", "");

    //判断一下该配置项是否存在
    if(this->settingMap->contains(fileName)) return;
    if(this->settingMap->value(name)->contains(fileName)) return;

    //将配置的设置放入map中
    this->settingMap->value(name)->insert(fileName, filePath);

    //将配置的设置访如ui界面中
    this->tabletWidgetUnitMgr->AddUnitMap(this->settingMap->value(name));

    //记录到json文件中
    this->jsonMgr->Recorde2Json();
}

//删除设置配置
void MainWindow::onBtnDelUnitClickedSlot()
{
    //获取该条数据记录
    QString settingName = this->tabletWidgetSettingMgr->GetCurrentSetting();
    QString fileName = this->tabletWidgetUnitMgr->GetCurrentSetting();
    if (settingName.isEmpty() || fileName.isEmpty()) return;

    //先删除map中的数据
    this->settingMap->value(settingName)->erase(this->settingMap->value(settingName)->find(fileName));

    //删除ui中的数据
    this->tabletWidgetUnitMgr->DelSetting();

    //记录到json文件中
    this->jsonMgr->Recorde2Json();
}

//编辑设置配置
void MainWindow::onBtnEditUnitClickedSlot()
{
    //获取该条数据记录
    QString settingName = this->tabletWidgetSettingMgr->GetCurrentSetting();
    QString fileName = this->tabletWidgetUnitMgr->GetCurrentSetting();
    if (settingName.isEmpty() || fileName.isEmpty()) return;

    //弹出对话框，要求为配置输入一个名称
    QString name = QInputDialog::getText(this, QStringLiteral("输入一个名称"), QStringLiteral("快捷方式名称："), QLineEdit::Normal, fileName);
    if(name.isEmpty()) return;

    //将map中旧的设置改为新的
    this->settingMap->value(settingName)->insert(name, this->settingMap->value(settingName)->value(fileName));
    this->settingMap->value(settingName)->erase(this->settingMap->value(settingName)->find(fileName));

    //将ui中的数据改掉
    this->tabletWidgetUnitMgr->AddUnitMap(this->settingMap->value(settingName));

    //记录到json文件中
    this->jsonMgr->Recorde2Json();
}

//执行按钮
void MainWindow::onBtnDoClickedSlot()
{
    //依据当前被选中的配置集，一次打开软件快捷方式
    QString name = this->tabletWidgetSettingMgr->GetCurrentSetting();
    if(!this->settingMap->contains(name)) return;
    for (QMap<QString, QString>::iterator iter = this->settingMap->value(name)->begin(); iter != this->settingMap->value(name)->end(); iter++)
    {
        //创建打开软件的进程
        QProcess process(this);

        //配置启动工具cmd
        process.setProgram("cmd");

        //配置要启动的软件的快捷方式的文件路径
        QStringList argument;
        argument<<"/c"<<iter.value();
        process.setArguments(argument);

        //非阻塞启动
        process.startDetached();
    }

    //如果勾选了“完成后关闭”，关闭界面
    if(ui->chbEnableClose->isChecked())
    {
        this->close();
    }
}

//配置集表切换
void MainWindow::onTableWidgetSettingItemClickedSlot(QTableWidgetItem* item)
{
    //获取配置集名字
    QString name = item->text();

    //判断该配置集是否存在
    if (this->settingMap->contains(name))
    {
        //将配置的设置访如ui界面中
        this->tabletWidgetUnitMgr->AddUnitMap(this->settingMap->value(name));
    }
}

void MainWindow::onTableWidgetSettingCurrentItemChangedSlot(QTableWidgetItem* currentItem, QTableWidgetItem* previousItem)
{
    //容错，当删除最后一个配置集的时候，传来的单元格对象为空
    if (Q_NULLPTR == currentItem)
    {
        this->tabletWidgetUnitMgr->ClearContents();
        return;
    }

    //获取配置集名字
    QString name = currentItem->text();
    Q_UNUSED(previousItem);

    //判断该配置集是否存在
    if (this->settingMap->contains(name))
    {
        //将配置的设置访如ui界面中
        this->tabletWidgetUnitMgr->AddUnitMap(this->settingMap->value(name));
    }
}

