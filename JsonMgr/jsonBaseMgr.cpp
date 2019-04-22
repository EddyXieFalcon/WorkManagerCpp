#include "jsonBaseMgr.h"

JsonMgr::JsonMgr(QObject *parent) :
    QObject(parent)
{

}

JsonMgr::~JsonMgr()
{

}

//设置map，以便于同步数据到文件
void JsonMgr::SetMap(QMap<QString, QMap<QString, QString>*>* map)
{
    this->settingMap = map;
}

//将map数据写入json
void JsonMgr::Recorde2Json()
{
    //以覆盖只写方式打开同目录下的json文件，若该文件不存在则会自动创建
    QFile file("setting.json");
    file.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate);

    //如果map为空，写入空文本
    if (this->settingMap->isEmpty())
    {
        //文件已经被清空，直接关闭
        file.close();
        return;
    }

    //创建json文本容器，顶层的map，放入一个json对象中
    QJsonObject jsonObject;

    //解析map，构成json文本
    for(QMap<QString, QMap<QString, QString>*>::iterator iterFirst = this->settingMap->begin(); iterFirst != this->settingMap->end(); iterFirst++)
    {
        //所有的设置项，放入一个数组中，准备容器
        QJsonArray jsonArry;

        //循环遍历配置集中的每一个配置
        for(QMap<QString, QString>::iterator iterSecond = iterFirst.value()->begin(); iterSecond != iterFirst.value()->end(); iterSecond++)
        {
            //每个配置都是一个对象
            QJsonObject jsonUnitObject;
            jsonUnitObject.insert(iterSecond.key(), iterSecond.value());

            //将对象放入数组
            jsonArry.push_back(jsonUnitObject);
        }

        //为设置集的json对象填装数据
        jsonObject.insert(iterFirst.key(), jsonArry);
    }

    //使用QJsonDocument设置该json对象
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObject);

    //将json以文本形式写入文件并关闭文件。
    file.write(jsonDoc.toJson());
    file.close();
}

//将json数据加载入map
void JsonMgr::Load2Map()
{
    //打开json文件
    QFile loadFile("setting.json");
    if(!loadFile.open(QIODevice::ReadOnly)) return;

    //将数据抽出，并关闭文件
    QByteArray allData = loadFile.readAll();
    loadFile.close();

    //将数据解析为json格式
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));
    if(json_error.error != QJsonParseError::NoError) return;

    //解析第一层数据集
    QJsonObject rootObj = jsonDoc.object();
    QStringList keys = rootObj.keys();
    for(int i = 0; i < keys.size(); i++)
    {
        //放入map
        this->settingMap->insert(keys.at(i), new QMap<QString, QString>());

        //解析第二层
        QJsonArray jsonArry = rootObj.value(keys.at(i)).toArray();
        for (int j = 0; j < jsonArry.size(); j++)
        {
            //解析每一个配置的key和value
            QJsonObject subObj = jsonArry.at(j).toObject();
            QStringList subKeys = subObj.keys();
            for(int k = 0; k < subKeys.size(); k++)
            {
                //放入map
                this->settingMap->value(keys.at(i))->insert(subKeys.at(k), subObj[subKeys.at(k)].toString());
            }
        }
    }
}
