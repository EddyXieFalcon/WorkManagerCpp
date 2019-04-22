#ifndef TABLEWIDGETBASEMGR_H
#define TABLEWIDGETBASEMGR_H

#include <QTableWidget>
#include <QObject>
#include <QString>
#include <QMap>
#include <QDebug>

class TableWidgetBaseMgr : public QObject
{
    Q_OBJECT
public:
    explicit TableWidgetBaseMgr(QObject *parent = Q_NULLPTR);
    ~TableWidgetBaseMgr();

    void SetTableWidget(QTableWidget*);
    void AddSetting(QString);
    void DelSetting();
    void EditSetting(QString);

    void AddUnitMap(QMap<QString, QString>*);
    void ClearContents();

    bool HasSelectedSetting();
    QString GetCurrentSetting();

signals:

public slots:

private:
    QTableWidget* tableWidget;
};

#endif // TABLEWIDGETBASEMGR_H
