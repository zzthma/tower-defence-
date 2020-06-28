#ifndef LISTREADER_H
#define LISTREADER_H


#include <QXmlStreamReader>

class listreader
{
public:
    listreader();

    bool read(QIODevice *device);
    const QList<QVariant> data() const;

    QString errorString() const;

private:
    void readPList();
    void readArray(QList<QVariant> &array);
    void readDict(QList<QVariant> &array);
    void readKey(QMap<QString, QVariant> &dict);

private:
    QXmlStreamReader	_xmlReader;
    QList<QVariant>		_data;
};


#endif // LISTREADER_H
