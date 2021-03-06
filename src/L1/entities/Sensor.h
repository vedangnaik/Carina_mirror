#pragma once

#include <string>
#include <vector>

enum class SensorOption {

};

struct SensorCheck {
    int a;
    int b;
};

class Sensor {
public:
    Sensor(std::string id, std::string name) : id(id), name(name), values({0.0}) {};
    const std::string id;
    const std::string name;
    std::vector<float> values;
};
