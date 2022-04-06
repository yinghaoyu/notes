#ifndef EMDXMLPARSE_H
#define EMDXMLPARSE_H

#include <QtXml/QDomDocument>
#include <QFile>
#include <QStringList>
#include <QDir>
#include <QString>
#include <string>

using namespace std;

class EmdXmlParse
{
public:
    EmdXmlParse();
    bool Init(const QString& path,const QString& name);
    QString GetString(const QString& eleName) const;
    QStringList& GetArray(const QString& arrayName);
    bool ModifyValue(const QString& attributename ,const QString& text_value);
    bool ModifyArrayValue(const QString& ArrayName ,const QString& memName ,const QString& text_value);
    bool ModifyAttribute(const QString& attributename, const QString& newAttrubutename);

    const QString& fileName() const { return m_name; }
    //notify
    void AddNode(const QString nodeName, QString nodeValue = "");
    void GetAllNode(int& count, QStringList& namelist, QStringList& valuelist);
    void GetGroup(const QString& groupName, QStringList& namelist, QStringList& valuelist);
    void AddGroupMem(const QString& groupName, QString& memName, QString& memvalue);
    void DelNode(const QString& nodeName);
    void DelAllNode();
    static bool renameFile(QString oldName, QString newName);

    bool SaveDoc();
    static bool deleteFile(QString );
    void updateName(QString name);
    static bool createFile(QString fileName);
    static QString getFilePath(QString fileName);
    static void createDir(QString path);
private:
    bool ModifyValue(QDomNode node ,const QString& text_value);

    QDomDocument m_doc;    // xml的文档
    QDomNode m_node;
    QStringList m_sArray;  // 用于临时保存数组
    QStringList m_sGroup;  // 用于临时保存小组
    QString m_path;        // xml文件路径
    QString m_name;        // xml文件名
    QString m_fullName;    // xml文件名 + 文件路径
};

#endif // EMDXMLPARSE_H
