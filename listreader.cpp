#include "listreader.h"

listreader::listreader()
{

}

#include <QIODevice>
#include <QVariant>
#include <QMap>


bool listreader::read(QIODevice *device)
{
    _xmlReader.setDevice(device);

    if (_xmlReader.readNextStartElement())
    {
        if (_xmlReader.name() == "plist" && _xmlReader.attributes().value("version") == "1.0")
            readPList();
        else
            _xmlReader.raiseError("The file is not an PList version 1.0 file.");
    }

    return _xmlReader.error();
}

const QList<QVariant> listreader::data() const
{
    return _data;
}

QString listreader::errorString() const
{
    return QString("%1\nLine %2, column %3")
            .arg(_xmlReader.errorString())
            .arg(_xmlReader.lineNumber())
            .arg(_xmlReader.columnNumber());
}

void listreader::readPList()
{
    Q_ASSERT(_xmlReader.isStartElement() && _xmlReader.name() == "plist");

    while (_xmlReader.readNextStartElement())
    {
        if (_xmlReader.name() == "array")
            readArray(_data);
        else if (_xmlReader.name() == "dict")
            readDict(_data);
        else
            _xmlReader.skipCurrentElement();
    }
}

void listreader::readArray(QList<QVariant> &array)
{
    Q_ASSERT(_xmlReader.isStartElement() && _xmlReader.name() == "array");

    while (_xmlReader.readNextStartElement())
    {
        if (_xmlReader.name() == "array")
        {
            QList<QVariant> subArray;
            readArray(subArray);
            array.push_back(subArray);
        }
        else if (_xmlReader.name() == "dict")
        {
            readDict(array);
        }
        else
        {
            _xmlReader.skipCurrentElement();
        }
    }
}

void listreader::readDict(QList<QVariant> &array)
{
    Q_ASSERT(_xmlReader.isStartElement() && _xmlReader.name() == "dict");

    QMap<QString, QVariant> dict;
    while (_xmlReader.readNextStartElement())
    {

        if (_xmlReader.name() == "key")
            readKey(dict);
        else
            _xmlReader.skipCurrentElement();
    }

    array.push_back(dict);
}

void listreader::readKey(QMap<QString, QVariant> &dict)
{
    Q_ASSERT(_xmlReader.isStartElement() && _xmlReader.name() == "key");
    QString key = _xmlReader.readElementText();
    Q_ASSERT(_xmlReader.readNextStartElement());
    QString value = _xmlReader.readElementText();

    dict.insertMulti(key, value);
}
