#include "layer3.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>

void ProcessController::transition(std::string alphabet) {
    this->ucip->transition(alphabet);
}


void ProcessGateway::parseProcessFile(std::string fileName, ProcessManager* pm, SensorsManager* sm) {
    QFile file(QString::fromStdString(fileName));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cout << file.errorString().toStdString() << std::endl;
    }
    QString val = file.readAll();
    file.close();
    QJsonObject jsonObj = QJsonDocument::fromJson(val.toUtf8()).object();

    QJsonObject sensors = jsonObj.value("sensors").toObject();
    for (QString k: sensors.keys()) {
        std::string sensorId = k.toStdString();
        QJsonObject sensor = sensors.value(k).toObject();
        std::string sensorName = sensor.value("name").toString().toStdString();
        sm->createSensor(sensorId, sensorName);
    }

    QJsonObject states = jsonObj.value("states").toObject();
    if (!states.contains("start")) { return; }



}
