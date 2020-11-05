#ifndef USECASES_H
#define USECASES_H

#include "src/entities/entities.h"

//====

class PMOC {
public:
    virtual void displayState(State* s) = 0;
    virtual ~PMOC() {};
};

class PMIC {
public:
    virtual void createProcess(std::vector<State*> states) = 0;
    virtual void transition(std::string transition) = 0;
    virtual ~PMIC() {};
};

class ProcessManager : public PMIC {
public:
    void createProcess(std::vector<State*> Q);
    void transition(std::string transition);
    void setOutputContract(PMOC* pmoc);
private:
    Process* p = NULL;
    PMOC* pmoc;
};

//====

class SMOC {
public:
};

class SMIC {
public:
    virtual void addSensors(std::vector<Sensor*> sensors) = 0;
};

class SensorsManager : public SMIC {
public:
    void addSensors(std::vector<Sensor*> sensors);
private:
    std::map<std::string, Sensor*> sensors;
    SMOC* smoc;
};

//====

class AMOC {
public:
};

class AMIC {
public:
    virtual void addActuators(std::vector<Actuator*> actuators) = 0;
};

class ActuatorsManager : public AMIC {
public:
    void addActuators(std::vector<Actuator*> actuators);
private:
    std::map<std::string, Actuator*> actuators;
    AMOC* amoc;
};

#endif // USECASES_H
