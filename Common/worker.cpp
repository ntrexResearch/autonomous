#include "worker.h"

Worker::Worker()
{

}

Worker::~Worker()
{

}

void Worker::onTimeout() {

    if (portList.size() != info.availablePorts().size()) {
        foreach(QSerialPortInfo item, info.availablePorts()) {
            portStringList << item.portName();
        }
        portList = info.availablePorts();
        emit portListChanged(portStringList);
    }
    portStringList.clear();

}
