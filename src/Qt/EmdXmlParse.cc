#include "emdxmlparse.h"
#include <QDebug>

EmdXmlParse::EmdXmlParse()
{
}
bool EmdXmlParse::Init(const QString& path,const QString& name)
{
   m_fullName = path + QString::fromStdWString(L"/")+name;
   QFile file(m_fullName);
   m_path = path;
   m_name = name;
   if(!file.open(QIODevice::ReadOnly))
   {
       return false;
   }

   if(!m_doc.setContent(&file))
   {
       file.close();
       return false;
   }
   file.close();  // 加载完毕后应关闭文件
   QDomElement root =  m_doc.firstChildElement();

   if(root.isNull())
       return false;

   if( root.tagName() != QString::fromStdWString(L"canary"))  // 做基本的节点校验
       return false;

   m_node = root.firstChild();

   if(m_node.isNull())
       return false;

   QString sFirstNode = m_node.nodeName();

   if((sFirstNode + QString::fromStdWString(L".xml")) != name) // 基本的文件名校验
       return false;

   return true;
}

// 获取元素对应的text
QString EmdXmlParse::GetString(const QString& eleName) const
{
    QString tagName = QString::fromStdWString(L"s");

    QDomElement ele= m_node.firstChildElement(tagName);
    while(!ele.isNull())
    {
        if(ele.attribute("name") == eleName)
             return ele.text();
        ele = ele.nextSiblingElement(tagName);
    }
    return "";
}

// 获取数组所有元素
QStringList& EmdXmlParse::GetArray(const QString& arrayName)
{
    m_sArray.clear();
    QString tagname = QString::fromStdWString(L"array");
    QDomElement arrayNode= m_node.firstChildElement(tagname);
    while(!arrayNode.isNull())
    {
        if(arrayNode.attribute("name") == arrayName)
        {
            QString child_tagname = QString::fromStdWString(L"s");

            QDomElement ele = arrayNode.firstChildElement(child_tagname);
            while(!ele.isNull())  // 有子节点就说明是数组
            {
                m_sArray.append(ele.text());
                ele = ele.nextSiblingElement(child_tagname);
            }
            break;
        }
        arrayNode = arrayNode.nextSiblingElement(tagname);
    }

    return m_sArray;
}

// 修改属性字段值
bool EmdXmlParse::ModifyValue(const QString& attributename ,const QString& text_value)
{
    QString tagname = QString::fromStdWString(L"s");

    QDomElement ele= m_node.firstChildElement(tagname);
    while(!ele.isNull())
    {
        if(ele.attribute("name") == attributename)
        {
            QDomNode node = ele.firstChild();
            if(node.isNull())
            {
                QDomText text= m_doc.createTextNode(text_value);
                ele.appendChild(text);  //
            }
            node = ele.firstChild();
            return ModifyValue(node , text_value);
        }
        ele = ele.nextSiblingElement(tagname);
    }
    return false;
}

// 修改属性名
bool EmdXmlParse::ModifyAttribute(const QString& attributename, const QString& newAttrubutename)
{
    QString tagname = QString::fromStdWString(L"s");

    QDomElement ele= m_node.firstChildElement(tagname);
    while(!ele.isNull())
    {
        if(ele.attribute("name") == attributename)
        {
            ele.setAttribute("name", newAttrubutename);
            return true;
        }
        ele = ele.nextSiblingElement(tagname);
    }
    return false;

}

// 重新命名文件
bool EmdXmlParse::renameFile(QString oldName, QString newName)
{
    QFile file(oldName);
    bool result = file.rename(newName);
    return result;
}

// 修改数组的值
bool EmdXmlParse::ModifyArrayValue(const QString& ArrayName ,const QString& memName ,const QString& text_value)
{
    QString tagname = QString::fromStdWString(L"array");
    QDomElement arrayNode= m_node.firstChildElement(tagname);
    while(!arrayNode.isNull())
    {
        if(arrayNode.attribute("name") == ArrayName)
        {
            QString child_tagname = QString::fromStdWString(L"s");

            QDomElement ele = arrayNode.firstChildElement(child_tagname);
            while(!ele.isNull())
            {
                if(ele.attribute("name") == memName)
                {
                    QDomNode node = ele.firstChild();
                    return ModifyValue(node , text_value);
                }
                ele = ele.nextSiblingElement(child_tagname);
            }
            break;
        }
        arrayNode = arrayNode.nextSiblingElement(tagname);
    }
    return false;
}

// 修改某节点的值
bool EmdXmlParse::ModifyValue(QDomNode node ,const QString& text_value)
{
    if(node.isNull())
        return false;
    QFile file(m_path + QString::fromStdWString(L"/")+m_name);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))
        return false;
    node.setNodeValue(text_value);
    QTextStream out(&file);
    out.setCodec("UTF-8");
    m_doc.save(out,4,QDomNode::EncodingFromTextStream);
    file.close();

    return true;
}

// 返回所有组成员属性和值
void EmdXmlParse::GetAllNode(int& count, QStringList& namelist, QStringList& valuelist)
{
    QString tagName = QString::fromStdWString(L"s");
    QDomElement node= m_node.firstChildElement(tagName);
    while(!node.isNull())
    {
         namelist << node.attribute("name");
         valuelist << node.text();
         node = node.nextSiblingElement(tagName);
         count++;
    }
}

// 返回数组所有元素的属性名和值
void EmdXmlParse::GetGroup(const QString& groupName, QStringList& namelist, QStringList& valuelist)
{
    QString tagBeginName = QString::fromStdWString(L"group");
    QString tagEndName = QString::fromStdWString(L"/group");
    QDomElement groupNode= m_node.firstChildElement(tagBeginName);
    while(!groupNode.isNull())
    {
        if(groupNode.attribute("name") == groupName)
        {
            QString child_tagname = QString::fromStdWString(L"s");

            QDomElement ele = groupNode.firstChildElement(child_tagname);
            while(!ele.isNull()&&ele.tagName()!=tagEndName)
            {
                namelist.append(ele.attribute("name"));
                valuelist.append(ele.text());
                ele = ele.nextSiblingElement();
            }
            break;
        }
        groupNode = groupNode.nextSiblingElement(tagBeginName);
    }
}

// 新建节点，如果已存在就返回
void EmdXmlParse::AddNode(const QString nodeName, QString nodeValue)
{
    QString tagName = QString::fromStdWString(L"s");
    QDomElement node = m_node.firstChildElement(tagName);
    while(!node.isNull())
    {
        if(node.attribute("name") == nodeName)
        {
            return;  // 存在该节点就返回
        }
        node = node.nextSiblingElement(tagName);
    }
    // 新建节点并添加到尾部
    QDomElement newEle = m_doc.createElement(tagName);
    newEle.setAttribute("name", nodeName);
    QDomText text= m_doc.createTextNode(nodeValue);
    newEle.appendChild(text);
    m_node.appendChild(newEle);
}

// 删除对应的tag节点
void EmdXmlParse::DelNode(const QString& nodeName)
{
    QString tagName = QString::fromStdWString(L"s");
    QDomNodeList list = m_doc.elementsByTagName(tagName);
    for (int i = 0; i < list.count(); i++)
    {
        if (list.at(i).toElement().attribute("name") == nodeName)
        {
            m_node.toElement().removeChild(list.at(i));
            break;
        }
    }
}

// 从尾部开始删除所有的节点
void EmdXmlParse::DelAllNode()
{
    QString tagName = QString::fromStdWString(L"s");
    QDomNodeList list = m_doc.elementsByTagName(tagName);
    int count = list.count();
    for (int i = count - 1; i >= 0; i--)
    {
        m_node.toElement().removeChild(list.at(i));
    }
}

void EmdXmlParse::AddGroupMem(const QString& groupName, QString& memName, QString& memvalue)
{
    QString tagBeginName = QString::fromStdWString(L"group");
    QString tagEndName = QString::fromStdWString(L"/group");
    QDomElement groupNode= m_node.firstChildElement(tagBeginName);
    while(!groupNode.isNull())
    {
        if(groupNode.attribute("name") == groupName)
        {
            QString child_tagname = QString::fromStdWString(L"s");

            QDomElement ele = groupNode.firstChildElement(child_tagname);
            while(!ele.isNull() && ele.tagName() != tagEndName)
            {
                if (ele.attribute("name") == memName)
                {
                    return;
                }
                ele = ele.nextSiblingElement();
            }

            QDomElement newEle = m_doc.createElement(child_tagname);
            newEle.setAttribute("name", memName);
            QDomText text;
            text = m_doc.createTextNode(memvalue);
            newEle.appendChild(text);
            groupNode.appendChild(newEle);

            //SaveDoc();
            break;
        }
        groupNode = groupNode.nextSiblingElement(tagBeginName);
    }
}

// 保存xml格式到文件
bool EmdXmlParse::SaveDoc()
{
    QFile file(m_path + QString::fromStdWString(L"/")+m_name);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))
        return false;
    QTextStream out(&file);
    out.setCodec("UTF-8");
    m_doc.save(out,4,QDomNode::EncodingFromTextStream);
    file.close();
    return true;
}

// 创建默认路径及文件
bool EmdXmlParse::createFile(QString fileName)
{
    bool bCreate = false;
    QString dirPath = getFilePath(fileName);
    QDir dir(dirPath);
    if(!dir.exists())
    {
        createDir(dirPath);
    }

    wstring wsPath = fileName.toStdWString();
    const wchar_t *filePath = wsPath.data();
    try
    {
        FILE *file = _wfopen(filePath, L"w+,ccs=utf-8");
        if(NULL != file)
        {
            bCreate = true;
            fclose(file);

            QDomDocument doc;
            QDomElement rdsEle = doc.createElement(QString("canary"));  // 用于校验

            QDomElement nameEle = doc.createElement(QString("group"));//eleName

            QDomText text = doc.createTextNode("");
            rdsEle.appendChild(text);
            nameEle.appendChild(text);

            doc.appendChild(rdsEle);
            rdsEle.appendChild(nameEle);

            //SaveDoc();
            QFile file(fileName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))
                return false;
            QTextStream out(&file);
            out.setCodec("UTF-8");
            doc.save(out,4,QDomNode::EncodingFromTextStream);
            file.close();

            bCreate =  true;


        }
    }
    catch (...)
    {
    }

    return bCreate;
}

// 删除文件
bool EmdXmlParse::deleteFile(QString fullName)
{
    try
    {
        QFile file(fullName);
        if (file.remove())
        {
            return true;
        }
    }
    catch (...)
    {

    }

    return false;
}

// 获取文件路径
QString EmdXmlParse::getFilePath(QString fileName)
{
    int index = fileName.lastIndexOf("/");
    QString path;
    path = fileName.left(index + 1);
    return path;
}

// 递归创建路径
void EmdXmlParse::createDir(QString path)
{
    QDir dir(path);
    if(!dir.exists())
    {
        dir.mkpath(path);
    }
}

// 更新文件名
void EmdXmlParse::updateName(QString name)
{
    m_name = name;
    m_fullName = m_path + QString::fromStdWString(L"/")+ m_name;
}
