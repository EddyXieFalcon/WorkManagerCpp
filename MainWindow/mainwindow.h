#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QProcess>
#include <QDebug>

#include "tableWidgetBaseMgr.h"
#include "jsonBaseMgr.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void LoadMap();

private slots:
    //所有的按钮的槽函数
    void onBtnAddSettingClickedSlot();
    void onBtnDelSettingClickedSlot();
    void onBtnEditSettingClickedSlot();
    void onBtnAddUnitClickedSlot();
    void onBtnDelUnitClickedSlot();
    void onBtnEditUnitClickedSlot();
    void onBtnDoClickedSlot();

    void onTableWidgetSettingItemClickedSlot(QTableWidgetItem*);
    void onTableWidgetSettingCurrentItemChangedSlot(QTableWidgetItem*, QTableWidgetItem*);

private:
    Ui::MainWindow *ui;

    TableWidgetBaseMgr* tabletWidgetSettingMgr;
    TableWidgetBaseMgr* tabletWidgetUnitMgr;
    JsonMgr* jsonMgr;

    QMap<QString, QMap<QString, QString>*>* settingMap;
};

#endif // MAINWINDOW_H
