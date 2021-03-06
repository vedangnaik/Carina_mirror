SOURCES += \
    $$PWD/controllers/ActuatorsController.cpp \
    $$PWD/controllers/StatesController.cpp \
    $$PWD/gateways/ProcessFileParser.cpp \
    $$PWD/gateways/SensorValuesGateway.cpp \
    $$PWD/presenters/ActuatorsPresenter.cpp \
    $$PWD/presenters/SensorsPresenter.cpp \
    $$PWD/presenters/StatesPresenter.cpp

HEADERS += \
    $$PWD/controllers/ActuatorsController.h \
    $$PWD/controllers/StatesController.h \
    $$PWD/gateways/ProcessFileParser.h \
    $$PWD/gateways/SensorValuesGateway.h

INCLUDEPATH += \
    $$PWD/controllers \
    $$PWD/presenters \
    $$PWD/gateways
