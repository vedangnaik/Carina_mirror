#include "AiDAQHandler.h"

AiDAQHandler::AiDAQHandler(DaqDeviceHandle handle, unsigned int numChannels, Range gain) : handle{handle}, numChannels{numChannels}, gain{gain} {
    // connect DAQ
    if (ulConnectDaqDevice(handle) != ERR_NO_ERROR) {
        // shit;
    } else {
        this->connected = true;
    }
    // allocate the temporary data buffer
    this->dataBuffer = (double*)malloc(this->numChannels * this->samplesPerChannel * sizeof(double));
    // prepare the map of channel values
    for (unsigned int i = 0; i < numChannels; i++) {
        this->calibratedValues.insert(std::make_pair(i, std::vector<double>{}));
    }

    // enable notification for available samples
    ulEnableEvent(this->handle, this->events, this->numSamplesToNotifyFor, &this->dataHandler, nullptr);
}

AiDAQHandler::~AiDAQHandler() {
    free(this->dataBuffer);
}
