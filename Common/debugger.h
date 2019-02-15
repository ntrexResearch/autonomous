#ifndef DEBUGGER_H
#define DEBUGGER_H
#include <QObject>

class Debugger : public QObject
{
    Q_OBJECT
public:
    Debugger();
    void PrintErrorMsg(QString);
signals:
    void SendErrorMsg(QString);

private:
    QString ErrorMsg;
};

#endif // DEBUGGER_H
