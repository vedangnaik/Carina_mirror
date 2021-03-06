#pragma once

#include "SensorValuesGateway.h"
#include "AbstractDAQ.h"
#include "easylogging++.h"
#include <map>
#include <vector>
#include <mutex>
#include <QObject>
#include <iostream>
#include <QTimer>

class DAQManager : public QObject {
    Q_OBJECT
public:
    DAQManager(std::vector<AbstractDAQ*> DAQDevices, SVGIC& svgic);
    void startAcquisition();
    void getLatestData();
    void stopAcquisition();
    std::vector<std::string> getSensorIDs();

    const std::vector<AbstractDAQ*> DAQDevices;
    std::map<std::string, std::pair<AbstractDAQ*, unsigned int>> sensorToDAQLinks;
private:
    SVGIC& svgic;
    QTimer* DAQReadTimer;
    std::mutex sensorLinksMutex;
};
