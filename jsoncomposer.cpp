#include "jsoncomposer.h"
#include <QDebug>
#include <iostream>

JSONComposer::JSONComposer(QObject *parent) : QObject(parent), QJsonObject(){}

void JSONComposer::put(const QHash<QString, QString> &DataTable){

    assert(!DataTable.isEmpty());
    for(QHash<QString, QString>::const_iterator it = DataTable.cbegin();
        it != DataTable.cend(); ++it){
        this->insert(it.key(), QJsonValue::fromVariant(it.value()));
    }
    RadarName = DataTable["radar_name"];
    qDebug() << *this;
}

void JSONComposer::save(QWidget* window, const QString& encoding){
    jd.setObject(*this);
    QDir SaveDir;
    SaveDir.mkdir("parsed_json_objects");
    QString FileName = QFileDialog::getSaveFileName(window, tr("Save File"),
                               QDir::currentPath() + "/parsed_json_objects/" + RadarName +".json",
                               tr("JSON (*.json);;Text (*.txt);;All files (*.*)"));
    QFile file(FileName);
    file.open(QIODevice::WriteOnly);
    QTextStream txtStream(&file);
    txtStream.setCodec(encoding.toStdString().c_str());
    txtStream << jd.toJson();
    txtStream.flush();
    file.close();
    //qDebug() << jd.toJson();
}
