#ifndef SPEEDOMETER_H
#define SPEEDOMETER_H

//*************速度表***********
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPoint>
#include <QString>
#include <QRadialGradient>
#include <QFont>
#include <QWheelEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class Speedometer; }
QT_END_NAMESPACE

class Speedometer : public QWidget
{
    Q_OBJECT

public:
    Speedometer(QWidget *parent = nullptr);
    ~Speedometer();

public:
    void setSpeed(float speed); // 输入速度，转换成角度，改变指针转向
private:
    void initUI();
protected:
    void paintEvent(QPaintEvent*) override;
    void wheelEvent(QWheelEvent *event) override;   // 测试代码，可以滚动鼠标滑轮实现指针转向
private:
    Ui::Speedometer *ui;
    float m_angle;
    float m_speed;
};

#endif // SPEEDOMETER_H
