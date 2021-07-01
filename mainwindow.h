#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTableWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QSpinBox>
#include <QSignalMapper>
#include <QModelIndex>
#include <QTimer>
#include <thread>
#include "qcustomplot.h"
#include <QFile>

#include "analogpin.h"
#include "digitalpin.h"

#include "qfirmata.h"
#define SAFEDISTANCE 20 // distance of robot to wall in front
#define TARGET 20 //target distance to the wall
#define BANDWIDTH 6 //tolerance or band of the allowed distance to the wall

#define GAPDISTANCE 75 //distance where it is considered as a gap

#define HEADINGBAND 1 //allowed tolerance of the heading
#define HEADINGTOP HEADINGBAND  //top band of the heading
#define HEADINGBOTTOM 0-HEADINGBAND // bottom band of the heading

#define digL 4
#define pwmL 10
#define digR 2
#define pwmR 11 //verify all of these with the actual pins

#define TURNSLOW 90 //PWM between 255-0, 0 is the slowest, 255 fastest
#define TURNFAST 150
#define TURNMED 120
#define STOP 0
#define DEBUG 1

#define UPDATETIME 100

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setupRealtimeDataDemo(QCustomPlot *customPlot);

    QFile myfile;//declare file


private:
    Ui::MainWindow *ui;
    QFirmata *_firmata; //#declared as pointer


    QTableWidget *_configTableWidget;
    QComboBox *_portComboBox;
    QVector<QSpinBox*> _ioAnalogInputSpinBoxVec;
    QVector<QComboBox*> _ioDigitalPinModeComboBoxVec;
    QVector<QCheckBox*> _ioDigitalCheckBoxVec;
    QVector<QSlider*> _ioDigitalPinSliderVec;
    QVector<QSpinBox*> _ioDigitalPinSpinBoxVec;

    QPushButton *_connectButton;
    QPushButton *_disconnectButton;
    QLabel *_firmwareLabel;

    QScrollArea *_ioDigitalContainerWidget;
    QScrollArea *_ioAnalogInputContainerWidget;
    QWidget *_analogPinWidget;
    QWidget *_digitalPinWidget;

    QVector<AnalogPin*> _analogPinVec;
    QVector<DigitalPin*> _digitalPinVec;

//    bool LSense;
//    bool RSense;
//    bool CSense;
    int _firstDigitalPinIndex;
    //-----------------------------
    float Distance;
    float FrontSensor, FrontLeft, RearLeft, heading;
    float lastFrontLeft, lastRearLeft;
    QVector<float> ADistance, AFrontSensor, AFrontLeft, ARearLeft;

    float TARGETclose = TARGET-(BANDWIDTH/2);
    float TARGETfar = TARGET+(BANDWIDTH/2);

//    int enA = 12;
//    int in1 = 11;
//    int in2 = 10;

//    int enB = 7;
//    int in3 = 9;
//    int in4 = 8;
    void RobotForward(int delay);

    bool manual = false;
    bool startState = false;
    bool gap = false;
    bool pause = true;
    bool crash = false;
    int robotStuck = 0;
    int frontWall = false; //0 = wall, 1 = twist right, 2 = twist left


    bool gapPassed = false;
    int gapTime;
    QTimer checkGap;


    QString address;
    bool Flag;
    int Task;
    QString demoName;
    QTimer dataTimer;

    QTimer *timer;

    QVector<QString> actionName;


private slots:
    void on__disconnectButton_clicked();
    void on__connectButton_clicked();
    void onFirmwareVersionReceived(const int majorVersion, const int minorVersion);
    void onFirmwareNameReceived(QString firmwareName);
    void onInitialized(const int majorVersion, const int minorVersion, QString firmwareName);
    void onDigitalPinChanged(int pin);
    void onAnalogPinChanged(int pin);
    void onSysExReceived(const QVector<unsigned char> message);
    void onDigitalPinWidgetValueChanged(unsigned int id, unsigned int value, bool pwm);
    void onAnalogPinReportinChange(unsigned int pin, bool value);
    void realtimeDataSlot();
//    void on_horizontalSlider_valueChanged(int valueA);
//    void on_horizontalSlider_2_valueChanged(int valueB);
//    void demoOne();
//    void demoTwo();
//    void loop();
    void goStraight();
    void goRight();
    void goRightLow();
    void goLeft();
    void goLeftLow();
    void Park();
    void Neutral();
    void Reverse();
    void TwistLeft();
    void TwistRight();
    void Test();
    void RobotController();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_StartButton_clicked();
    void on_SaveButton_clicked();
    void ExecuteAction();
    void SensorPing();
    void on_pushButton_8_clicked();
    void gapStatus();
    void falsePause();
    void truePause();
    void falseCrash();
};

#endif // MAINWINDOW_H
