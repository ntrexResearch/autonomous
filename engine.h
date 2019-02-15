#ifndef ENGINE_H
#define ENGINE_H
#include <QObject>
#include <QTimer>
#include <QDebug>

class Engine : public QObject
{
    Q_OBJECT
public:
    static Engine* Instance(QObject *_parent);

public slots:
    void run();

protected:
    Engine(QObject *_parent);

private:
    QTimer *timer;
    static Engine* _instance;

    void initialize();
};

#endif // ENGINE_H
