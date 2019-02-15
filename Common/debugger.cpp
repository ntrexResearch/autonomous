#include "debugger.h"

Debugger::Debugger()
{

}

void Debugger::PrintErrorMsg(QString msg) {
    emit SendErrorMsg(msg);
}
