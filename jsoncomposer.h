#ifndef JSONCOMPOSER_H
#define JSONCOMPOSER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QHash>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QDir>
#include <cassert>

//local
#include "htmlcomposer.h"

class JSONComposer : public QObject, protected QJsonObject
{
    Q_OBJECT
public:
    explicit JSONComposer(QObject *parent = nullptr);
    void put(const QHash<QString, QString>&);
    void save(QWidget* window, const QString& encoding);
signals:

private:
    QString RadarName;
    QJsonDocument jd;
};

#endif // JSONCOMPOSER_H
