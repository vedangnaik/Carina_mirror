#include "GSManager.h"


GSManager::GSManager() {
    this->renderUi();

    connect(this->GSMainWindowUI.openProcessFromFileAction, &QAction::triggered, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Process File"), "/home/vedang/Desktop/");
        if (fileName != "") {
            this->makeProcessFromFile(fileName.toStdString());
            this->GSMainWindowUI.openProcessFromFileAction->setEnabled(false);
            this->GSMainWindowUI.startProcessAction->setEnabled(true);
        }
    });

    connect(this->GSMainWindowUI.startProcessAction, &QAction::triggered, this, [=]() {
        this->startProcess();
        this->GSMainWindowUI.startProcessAction->setEnabled(false);
        this->GSMainWindowUI.closeProcessAction->setEnabled(true);
    });

    connect(this->GSMainWindowUI.closeProcessAction, &QAction::triggered, this, [=]() {
        this->closeProcess();
        this->GSMainWindowUI.closeProcessAction->setEnabled(false);
        this->GSMainWindowUI.openProcessFromFileAction->setEnabled(true);
    });

    connect(this->GSMainWindowUI.openSystemDiagramAction, &QAction::triggered, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open System Diagram Image"), "/home/vedang/Desktop/");
        if (fileName != "") {
            this->systemDiagramUI.systemDiagramFrame->setStyleSheet("#systemDiagramFrame{ border-image: url(" + fileName + ") 0 0 0 0 }");
        }
    });

    connect(this->GSMainWindowUI.clearSystemDiagramAction, &QAction::triggered, this, [=]() {
        this->systemDiagramUI.systemDiagramFrame->setStyleSheet("");
    });
}

void GSManager::makeProcessFromFile(std::string filepath) {
    // Only use for Process Gateway
    ProcessGateway pg(filepath);
    struct ProcessData pgdata = pg.parseProcessFile();
    // init L2 classes here
    this->cm = new ClocksModule();
    this->sm = new SensorsManager(pgdata.sensors);
    this->am = new ActuatorsManager(pgdata.actuators);
    // check for invalid start state here
    this->stm = new StatesManager(pgdata.states, *this->sm, *this->am);
    // init L3 classes here
    this->svg = new SensorValuesGateway(*this->sm);
    this->ac = new ActuatorsController(*this->am);
    this->stc = new StatesController(*this->stm);
    // init L4 and presenters here
    this->daqp = new DAQPlaceholder(this->cm, this->svg);
    this->suih = new StateUIHandler(this->stateUI, *this->ac, *this->stc, *this->cm);
    this->sduih = new SystemDiagramUIHandler(this->systemDiagramUI, *this->ac, *this->cm);
    this->sp = new SensorsPresenter({this->suih, this->sduih});
    this->stp = new StatesPresenter(*this->suih);
    // attach presenters to managers (kinda ugly, but idk another way to do it)
    this->sm->setOutputContract(this->sp);
    this->stm->setOutputContract(this->stp);
}

void GSManager::startProcess() {
    this->stm->startProcess();
    this->daqp->startAcquisition();
    this->cm->start();
}

void GSManager::stopProcess() {
    this->cm->stop();
    this->daqp->stopAcquisition();
    this->stm->stopProcess();
}

void GSManager::closeProcess() {
    this->stopProcess();

    delete this->daqp;
    delete this->stm;
}

void GSManager::renderUi() {
    this->GSMainWindowUI.setupUi(this);
    this->stateUI.setupUi(this);
    this->systemDiagramUI.setupUi(this);
    this->GSMainWindowUI.stateFrame->layout()->addWidget(this->stateUI.StateWidget);
    this->GSMainWindowUI.systemDiagramFrame->layout()->addWidget(this->systemDiagramUI.systemDiagramFrame);
}
