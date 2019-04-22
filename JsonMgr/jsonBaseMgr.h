#ifndef JSONXMGR_H
#define JSONXMGR_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QMap>
#include <QDateTime>
#include <QDebug>

#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

class JsonMgr : public QObject
{
    Q_OBJECT
public:
    explicit JsonMgr(QObject *parent = Q_NULLPTR);
    ~JsonMgr();

    void SetMap(QMap<QString, QMap<QString, QString>*>*);

    void Recorde2Json();
    void Load2Map();

signals:

public slots:

private:
    QMap<QString, QMap<QString, QString>*>* settingMap;
};

#endif // JSONXMGR_H
