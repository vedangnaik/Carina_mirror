#pragma once

#include "src/L1/entities/Sensor.h"
#include "src/L2/services/easylogging++.h"
#include <map>
#include <stdexcept>


class SMOC {
public:
    virtual void notify(const std::string id, const float value) = 0;
    virtual ~SMOC() {};
};

class SMIC {
public:
    virtual float getSensorValue(std::string id) = 0;
    virtual void setSensorValue(std::string id, float value) = 0;
    virtual std::vector<std::string> getSensorIDs() = 0;
    virtual ~SMIC() {};
};

class SensorsManager : public SMIC {
public:
    SensorsManager(const std::map<std::string, Sensor*> sensors) : sensors(sensors) {};
    float getSensorValue(std::string id);
    void setSensorValue(std::string id, float value);
    std::vector<std::string> getSensorIDs();
    void setOutputContract(SMOC* smoc) {
        this->smoc = smoc;
    }
private:
    const std::map<std::string, Sensor*> sensors;
    SMOC* smoc;
};


