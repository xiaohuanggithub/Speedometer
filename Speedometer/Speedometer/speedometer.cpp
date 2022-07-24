#include "speedometer.h"
#include "ui_speedometer.h"


Speedometer::Speedometer(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Speedometer),
      m_angle(-120),
      m_speed(0)
{
    ui->setupUi(this);
    initUI();
}

Speedometer::~Speedometer()
{
    delete ui;
}

void Speedometer::setSpeed(float speed)
{
    m_speed = speed;
    // 根据速度计算角度
    // 速度 - 角度 对应关系
    // 0 - -120
    // 20 - -100
    m_angle = speed - 120;
    // 将m_angle限制在[-120, 120]之间
    m_angle = qMax((int)m_angle, -120);
    m_angle = qMin((int)m_angle, 120);
}

void Speedometer::initUI()
{
    setAttribute(Qt::WA_QuitOnClose, true);
    //setFixedSize(640, 480);
    setWindowTitle("速度仪表盘");
    setStyleSheet("background-color:black;");
}

void Speedometer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    // 1、确定画布中心，仪表盘的半径
    int width = this->width();
    int height = this->height();
    int radius = qMin(width, height) / 2;
    // 确定仪表盘的中心位置，左右居中，偏下
    QPoint centerPoint(width >> 1, height * 0.6);
    painter.translate(centerPoint);    // 重新确定坐标原点，以（width/2, 0.6height）为原点
    // 2、画最外圈的标记
    // 启用反锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);
    // 设置画笔
    painter.setPen(Qt::NoPen);
    // 设置画刷颜色
    painter.setBrush(QColor(98, 246, 255));
    /*
    /// 测试代码
    /// 以坐标原点为三角形的最下面的顶点，画一个标记如图：
    /// 放大版（所有的坐标点扩大20倍）为了看清楚
    QPainterPath ptPath2;
    ptPath2.moveTo(0, 0);
    ptPath2.lineTo(20, -20);
    ptPath2.lineTo(20, -60);
    ptPath2.lineTo(-20, -60);
    ptPath2.lineTo(-20, -20);
    painter.drawPath(ptPath2);
    */

    // 标记的路径图
    QPainterPath ptPath;
    ptPath.moveTo(0, 0);
    ptPath.lineTo(1, -1);
    ptPath.lineTo(1, -3);
    ptPath.lineTo(-1, -3);
    ptPath.lineTo(-1, -1);

    /*
     * 对应关系
     * rotate角度 - 笛卡尔
     * 0 - 90
     * 20 - 70
     * 40 - 50
     * -20 - 110
     * -120 - 210
     * ==> 210 - i * 20
    */


    painter.setPen(QColor(98, 246, 255));
    for(int i = 0; i < 13; i++)
    {
        painter.save();                 // 先保存原始的坐标系
        // 将原点移到半径的圆弧上然后画标记，就可以围成一圈
        // 计算半径上面的点的坐标
        QPoint radiusCenterPoint;
        radiusCenterPoint.setX((radius + 20) * qCos((210 - i * 20) * M_PI / 180));
        radiusCenterPoint.setY(-((radius + 20)* qSin((210 - i * 20) * M_PI / 180)));
        // 移动坐标原点
        painter.translate(radiusCenterPoint);
        painter.rotate(-120 + i * 20);  // 旋转坐标系画标记，那标记点的路径图就不用改变了
        painter.drawPath(ptPath);       // 画标记
        // 3、写数字
        painter.drawText(-15, 0, 30, 20, Qt::AlignCenter, QString::number(i * 20));
        painter.restore();              // 恢复到原始坐标系
    }

    // 3、写数字
    // 对应关系
    /* rotate角度 - km/h
     * -120° - 0
     * -100° - 20
     * 120° - 240
    */

    // 4、画边框圆弧
    // 保存绘图对象
    painter.save();
    // 计算大小圆路径
    QPainterPath outRing;
    outRing.moveTo(0, 0);
    // 画外部椭圆
    outRing.arcTo(-radius, -radius, 2 * radius, 2 * radius, -31, 243);
    outRing.closeSubpath();
    // 画内部椭圆
    QPainterPath inRing;
    inRing.moveTo(0,0);
    inRing.addEllipse(-radius + 20, -radius + 20, 2 * (radius - 20), 2 * (radius - 20));
    // 设置渐变色
    QRadialGradient radialGradient(0, 0, radius, 0, 0);
    radialGradient.setColorAt(0.95, QColor(98, 246, 255));
    radialGradient.setColorAt(1, QColor(0, 0, 0));
    // 设置渐变画刷
    painter.setBrush(radialGradient);
    // 大圆减去小圆得到圆环
    painter.drawPath(outRing.subtracted(inRing));
    //恢复绘图对象
    painter.restore();

    // 5、画刻度
    painter.setPen(QColor(98, 246, 255));
    // 刻度的路径图
    QPainterPath graduationPath;
    graduationPath.moveTo(0, 0);
    graduationPath.lineTo(1, 0);
    graduationPath.lineTo(2, 2);
    graduationPath.lineTo(1, 10);
    graduationPath.lineTo(-1, 10);
    graduationPath.lineTo(-2, 2);
    graduationPath.lineTo(-1, 0);
    for(int i = 0; i < 121; i++)
    {
        painter.save();                 // 先保存原始的坐标系
        // 将原点移到半径的圆弧上然后画标记，就可以围成一圈
        // 计算半径上面的点的坐标
        QPoint radiusCenterPoint;
        radiusCenterPoint.setX((radius -21) * qCos((210 - i * 2) * M_PI / 180));
        radiusCenterPoint.setY(-((radius - 21) * qSin((210 - i * 2) * M_PI / 180)));
        // 移动坐标原点
        painter.translate(radiusCenterPoint);
        painter.rotate(-120 + i * 2);  // 旋转坐标系画标记，那标记点的路径图就不用改变了
        painter.drawPath(graduationPath);       // 画标记
        painter.restore();              // 恢复到原始坐标系
    }

    // 6、画内圈的大小刻度
    // 小刻度
    QPainterPath sGraduationPath = graduationPath;
    // 大刻度
    QPainterPath lGraduationPath;
    lGraduationPath.moveTo(0, 0);
    lGraduationPath.lineTo(1, 0);
    lGraduationPath.lineTo(2, 3);
    lGraduationPath.lineTo(1, 15);
    lGraduationPath.lineTo(-1, 15);
    lGraduationPath.lineTo(-2, 3);
    lGraduationPath.lineTo(-1, 0);
    for(int i = 0; i < 25; i++)
    {
        painter.save();                 // 先保存原始的坐标系
        // 将原点移到半径的圆弧上然后画标记，就可以围成一圈
        // 计算半径上面的点的坐标
        QPoint radiusCenterPoint;
        radiusCenterPoint.setX((radius - 35) * qCos((210 - i * 10) * M_PI / 180));
        radiusCenterPoint.setY(-((radius - 35) * qSin((210 - i * 10) * M_PI / 180)));
        // 移动坐标原点
        painter.translate(radiusCenterPoint);
        painter.rotate(-120 + i * 10);  // 旋转坐标系画标记，那标记点的路径图就不用改变了
        if(i%2)
        {
            painter.drawPath(sGraduationPath);
        }
        else
        {
            painter.drawPath(lGraduationPath);
        }
        painter.restore();              // 恢复到原始坐标系
    }

    // 7、画单位 km/h
    // 设置字体大小
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(25);
    painter.setFont(font);
    painter.drawText(-25, -(radius - 80), 50, 20, Qt::AlignCenter, "km/h");

    // 8、画指针
#if 0
    /// ************版本1************
    //设置画刷
    painter.setPen(QColor(255, 0, 0, 200));
    painter.setBrush(QColor(255, 0, 0, 200));
    QPainterPath indicatorPath;
    indicatorPath.moveTo(0, 0);
    indicatorPath.lineTo(8, 0);
    indicatorPath.lineTo(0, -(radius - 30));
    indicatorPath.lineTo(-8, 0);
    indicatorPath.addEllipse(-8, -8, 16, 16);
    painter.save();
    //计算并选择绘图对象坐标
    painter.rotate(m_angle);    // 根据速度设置角度
    //绘制路径
    painter.drawPath(indicatorPath);
    painter.restore();
#endif

#if 1
    /// ************版本2************
    painter.setPen(QColor(255, 0, 0, 200));
    painter.setBrush(Qt::NoBrush);
    // 画圆形框
    painter.setPen(QPen(QColor(255, 0, 0, 200), 5));
    painter.drawEllipse(-radius / 6, -radius / 6, radius / 3, radius / 3);

    painter.setPen(QColor(255, 0, 0, 200));
    QPainterPath indicatorPath;
    indicatorPath.moveTo(0, 0);
    indicatorPath.lineTo(8, 0);
    indicatorPath.lineTo(0, -(radius - 60));
    indicatorPath.lineTo(-8, 0);
    indicatorPath.addEllipse(-8, -8, 16, 16);
    painter.save();
    // 计算圆形框的边框坐标作为坐标系原点，然后画指针
    QPoint point;
    point.setX(radius / 6 * qCos((270 + m_angle) * M_PI / 180));
    point.setY(radius / 6 * qSin((270 + m_angle) * M_PI / 180));
    painter.translate(point);
    painter.rotate(m_angle);
    painter.setBrush(QColor(255, 0, 0, 200));
    painter.drawPath(indicatorPath);
    painter.restore();
    // 显示速度
    font.setPointSize(25);
    painter.setFont(font);
    painter.drawText(-25, -10, 50, 20, Qt::AlignCenter, QString::number(m_speed));
#endif
}

// 测试代码
void Speedometer::wheelEvent(QWheelEvent *event)
{
    static int speed = 0;
    if(event->angleDelta().y() > 0)
    {
        speed += 10;
    }
    else
    {
        speed -= 10;
    }

    if(speed > 240)
    {
        speed = 240;
    }
    else if(speed < 0)
    {
        speed = 0;
    }

    setSpeed(speed);
    update();
}


