#include "GSManager.h"


GSManager::GSManager() {
    this->GSMainWindowUI.setupUi(this);
    this->stateUI.setupUi(this);
    this->systemDiagramUI.setupUi(this);

    this->GSMainWindowUI.stateFrame->layout()->addWidget(this->stateUI.StateWidget);
    this->GSMainWindowUI.systemDiagramFrame->layout()->addWidget(this->systemDiagramUI.systemDiagramFrame);

    connect(GSMainWindowUI.openProcessFromFileAction, &QAction::triggered, this, [&]() {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Process File"), "/home/vedang/Desktop/");
        if (fileName != "") {
            this->createProcess(fileName.toStdString());
        }
    });
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
    this->sduih = new SystemDiagramUIHandler(this->systemDiagramUI, *this->ac, *this->cm);
    this->sp = new SensorsPresenter({this->suih, this->sduih});
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
