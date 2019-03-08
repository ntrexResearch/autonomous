#ifndef CANCOMMWIDGET_H
#define CANCOMMWIDGET_H

#include <QWidget>

namespace Ui {
class CanCommWidget;
}

class CanCommWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CanCommWidget(QWidget *parent = nullptr);
    ~CanCommWidget();

private:
    Ui::CanCommWidget *ui;
};

#endif // CANCOMMWIDGET_H
