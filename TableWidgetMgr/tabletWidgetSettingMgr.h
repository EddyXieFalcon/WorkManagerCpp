#ifndef TABLETWIDGETSETTINGMGR_H
#define TABLETWIDGETSETTINGMGR_H

#include "tableWidgetBaseMgr.h"

class TabletWidgetSettingMgr : public TableWidgetBaseMgr
{
    Q_OBJECT
public:
    explicit TabletWidgetSettingMgr(QObject *parent = Q_NULLPTR);
    ~TabletWidgetSettingMgr();

signals:

public slots:

private:
};

#endif // TABLETWIDGETSETTINGMGR_H
