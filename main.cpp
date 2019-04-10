#include "mainwindow.h"
#include <QApplication>
#include "Controller/pidcontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    PIDController pid = PIDController(100, -100, 0.1, 0.01, 0.5);

//       double val = 20;
//       for (int i = 0; i < 100; i++) {
//           double inc = pid.calculate(0, val, 0.2);
//           printf("val:% 7.3f inc:% 7.3f\n", val, inc);
//           val += inc;
//   }
    int ret = a.exec();
    //delete a;
    return ret;
}
