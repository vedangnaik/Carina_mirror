#include "GSManager.h"
#include "ui_gsmainwindow.h"
#include "ui_state.h"


GSManager::GSManager() {
    this->mainWindowUI = new Ui::GSMainWindow;
    this->mainWindowUI->setupUi(this);
    this->stateUI = new Ui::State;
    this->stateUI->setupUi(this);
    this->mainWindowUI->stateLayout->addWidget(this->stateUI->StateWidget);

    connect(mainWindowUI->openProcessFromFileAction, &QAction::triggered, this, [&]() {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Process File"), "/home/vedang/Desktop/");
        if (fileName != "") {
            this->createProcess(fileName.toStdString());
        }
    });
}

GSManager::~GSManager() {
    delete mainWindowUI;
}

void GSManager::createProcess(std::string filepath) {
    ProcessGateway pg(filepath);
    struct ProcessData pgdata = pg.parseProcessFile();

    this->cm = new ClocksModule();
    this->sm = new SensorsManager(pgdata.sensors);
    this->am = new ActuatorsManager(pgdata.actuators);
    // check for invalid start state here
    this->stm = new StatesManager(pgdata.states, *this->sm, *this->am);

    this->svg = new SensorValuesGateway(*this->sm);
    this->ac = new ActuatorsController(*this->am);
    this->stc = new StatesController(*this->stm);

    this->suih = new StateUIHandler(this->stateUI, *this->ac, *this->stc, *this->cm);
    this->sp = new SensorsPresenter({this->suih});
    this->stp = new StatesPresenter(*this->suih);

    this->sm->setOutputContract(this->sp);
    this->stm->setOutputContract(this->stp);


    this->daqp = new DAQPlaceholder(this->cm, this->svg);

    this->stm->startProcess();
    this->daqp->startAcquisition();
    this->cm->start();
}

void GSManager::closeProcess() {

}

void GSManager::startProcess() {

}