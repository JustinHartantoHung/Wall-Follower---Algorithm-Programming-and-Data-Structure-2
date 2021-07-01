#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qfirmata.h>
#include <QMainWindow>
#include <analogpin.h>
#include <digitalpin.h>
#include <QDebug>
#include <QTimer>
#include <QThread>


//#define V1 255
//#define V2 0
//#define RIGHT 5
//#define LEFT 6
#define GAPLENGTH 10// cm
#define BOTSPEED 5// cm/s

MainWindow::MainWindow(QWidget *parent) : //#our mainwindow is derived from many class, Qmainwindow,_firmata, _firstdigita..
     QMainWindow(parent),
     _firmata (new QFirmata (this)),
     _firstDigitalPinIndex(2),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->_connectButton, SIGNAL(clicked()), this, SLOT(on__ConnectButton_Clicked()));
    connect(ui->_disconnectButton, SIGNAL(clicked()), this, SLOT(on__DisconnectButton_Clicked()));
//    connect(ui->startButton, SIGNAL (clicked()), this, SLOT(onTimer1()));
    connect(_firmata, SIGNAL(firmwareVersionReceived(int, int)), this, SLOT(onFirmwareVersionReceived(int,int)));
    connect(_firmata, SIGNAL(firmwareNameReceived(QString)), this, SLOT(onFirmwareNameReceived(QString)));
    connect(_firmata, SIGNAL(initialized(int,int,QString)), this, SLOT(onInitialized(int,int,QString)));
    connect(_firmata, SIGNAL(digitalPinChanged(int)), this, SLOT(onDigitalPinChanged(int)));
    connect(_firmata, SIGNAL(analogPinChanged(int)), this, SLOT(onAnalogPinChanged(int)));
    connect(_firmata, SIGNAL(sysExReceived(QVector<unsigned char>)), this, SLOT(onSysExReceived(QVector<unsigned char>)));
    timer = new QTimer(this); //#when the timer is timeout, it will connect button4

    connect(&checkGap, SIGNAL(timeout()), this, SLOT(gapStatus()));
    connect(timer, SIGNAL(timeout()), this, SLOT(SensorPing())); //#to use timer when pushbutton 2 and 3 is clicked

     //#initialy it is commented, because we need it to call the timer to start, so we uncomment it
//    timer2 = new QTimer(this);
//    connect(timer2, SIGNAL(timeout()), this, SLOT(on_connectButton_4_clicked()));

//    analogPinWidget->setDisabled(true);
//    _digitalPinWidget->setDisabled(true);


    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->_portComboBox->addItem(info.portName());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on__connectButton_clicked()
{
    if(_firmata-> open(ui->_portComboBox->currentText())) //function open, and others is inhereted in the _firmata object written in qfirmata.cpp
    {
        statusBar()->showMessage(tr("[Notice] Successfully opened connection"), 5000);

        _firmata->sendDigitalPinMode(0, ARD_INPUT);
        _firmata->sendDigitalPinReporting(0, ARD_INPUT);
        _firmata->sendDigitalPinMode(1, ARD_OUTPUT);
        _firmata->sendDigitalPinMode(2, ARD_OUTPUT);
        _firmata->sendDigitalPinMode(3, ARD_OUTPUT);
        _firmata->sendDigitalPinMode(4, ARD_OUTPUT);
        _firmata->sendDigitalPinMode(5, ARD_OUTPUT);
        _firmata->sendDigitalPinMode(6, ARD_INPUT); //check pins physically
        _firmata->sendDigitalPinMode(7, ARD_OUTPUT);
        _firmata->sendDigitalPinMode(8, ARD_INPUT);
        _firmata->sendDigitalPinMode(9, ARD_OUTPUT);
        _firmata->sendDigitalPinMode(10, ARD_PWM);
        _firmata->sendDigitalPinMode(11, ARD_PWM);
        _firmata->sendDigitalPinMode(12, ARD_OUTPUT);
        _firmata->sendDigitalPinMode(13, ARD_OUTPUT);
        _firmata->sendAnalogPinReporting(0, ARD_ANALOG);

    }
    else
    {
        statusBar()->showMessage(tr("[Error] Could not open connection"), 5000);
    }
}
void MainWindow::on__disconnectButton_clicked()
{
    _firmata->close();
    _firmwareLabel->setText("");
    statusBar()->showMessage(tr("[Notice] Successfully closed connection"), 5000);
}

void MainWindow::onFirmwareVersionReceived(const int majorVersion, const int minorVersion)
{
    qDebug()<<"major"<<majorVersion<<"minor"<<minorVersion;
}

void MainWindow::onFirmwareNameReceived(QString firmwareName)
{
    qDebug()<<"firmware name"<<firmwareName;
}

void MainWindow::onInitialized(const int majorVersion, const int minorVersion, QString firmwareName)
{
    qDebug()<<"firmate initialized"<<majorVersion<<minorVersion<<firmwareName;
    statusBar()->showMessage(tr("initialized"), 5000);

    QString statusText = QString::number(majorVersion)+"."+QString::number(minorVersion);

//    _firmata->sendDigitalPinMode(11, ARD_INPUT);
//    _firmata->sendDigital(11, ARD_HIGH);
//    _firmata->sendDigitalPinMode(12, ARD_INPUT);
//    _firmata->sendDigital(12, ARD_LOW);
//    _firmata->sendDigitalPinMode(13, ARD_INPUT);
//    _firmata->sendDigital(13, ARD_LOW);
    _firmata->sendDigitalPinMode(0, ARD_INPUT);
    _firmata->sendDigitalPinReporting(0, ARD_INPUT);
    _firmata->sendDigitalPinMode(1, ARD_OUTPUT);
    _firmata->sendDigitalPinMode(2, ARD_OUTPUT);
    _firmata->sendDigitalPinMode(3, ARD_OUTPUT);
    _firmata->sendDigitalPinMode(4, ARD_OUTPUT);
    _firmata->sendDigitalPinMode(5, ARD_OUTPUT);
    _firmata->sendDigitalPinMode(6, ARD_INPUT); //check pins physically
    _firmata->sendDigitalPinMode(7, ARD_OUTPUT);
    _firmata->sendDigitalPinMode(8, ARD_INPUT);
    _firmata->sendDigitalPinMode(9, ARD_OUTPUT);
    _firmata->sendDigitalPinMode(10, ARD_PWM);
    _firmata->sendDigitalPinMode(11, ARD_PWM);
    _firmata->sendDigitalPinMode(12, ARD_OUTPUT);
    _firmata->sendDigitalPinMode(13, ARD_OUTPUT);
    // _firmata->sendDigitalPinMode(14, ARD_INPUT);
    // _firmata->sendDigitalPinReporting(14, ARD_INPUT);
    _firmata->sendAnalogPinReporting(0, ARD_ANALOG);
}


void MainWindow::onDigitalPinChanged(int pin)
{
    qDebug()<<"digital pin"<<pin<<"changed. new value"<<_firmata->getDigital(7)<<_firmata->getDigital(8);
//    _digitalPinVec[pin-2]->setValue(_firmata->getDigital(pin));
}

void MainWindow::onAnalogPinChanged(int pin)
{
//    if(pin < _analogPinVec.size())
//    {
//        _analogPinVec[pin]->setValue(_firmata->getAnalog(pin));
//    }

    if(pin == 0)
    {
//        ui->lcdNumber_3->display(_firmata->getAnalog(pin));
        int value = _firmata->getAnalog(pin);
        float voltvalue = (value/1023.0) * 5.0; // the new value for the new function
        qDebug()<<"analog pin"<<pin<<"changed. new value"<<_firmata->getAnalog(pin);
        //
        FrontSensor = 29.988 * qPow(voltvalue , -1.173);
        ui->lcdNumber_3->display(FrontSensor);
    }
}

void MainWindow::onSysExReceived(QVector<unsigned char> message)
{
    // #whenever button ping is clicked, the message that sent to arduino will be accepted, take the right ping sensor, from the front, take the data, send back meessage to qt, qt check the respond 00 or 01
    // #the message is tranfered by byte, how to get the repond,from loop lin 257-258. *it is a pointer.
    QString str;
    QVector<unsigned char>::iterator it;
    unsigned char buffer, address;

    qDebug()<<"sysExReceived"<<message;
    it = message.begin();
    address = *it;
    it++; // skip the first byte, which is the string command
    while( it != message.end() ) {
            buffer = *it; //#getting character by character
            it++;
//            buffer += *it;// << 7;
//            it++;
            str+=buffer; //#adding character by character
    }
    if (address == SONAR_DATA+0x00){ // #the address got from aruidno where Serial.write(SONAR_DATA+SlaveAddress)
        qDebug()<<"sysExReceived"<<address;
        ui->lcdNumber->display(str);
        lastFrontLeft = FrontLeft;
        FrontLeft = str.toDouble();
        AFrontLeft.push_back(FrontLeft);
    }
    else if (address == SONAR_DATA+0x01){
        qDebug()<<"sysExReceived"<<address;
        ui->lcdNumber_2->display(str);
        lastRearLeft = RearLeft;
        RearLeft = str.toDouble();
        ARearLeft.push_back(RearLeft);
    }
}

void MainWindow::onDigitalPinWidgetValueChanged(unsigned int id, unsigned int value, bool pwm)
{
    qDebug()<<"digital pin"<<id<<value<<pwm;

    if(pwm)
    {
        _firmata->sendPWM(id, value);
    }
    else
    {
        _firmata->sendDigital(id, value);
    }
}

void MainWindow::onAnalogPinReportinChange(unsigned int pin, bool value)
{
    qDebug()<<"analog pin reporting"<<pin<<"changed. new value"<<value;

    if(value)
    {
        _firmata->sendAnalogPinReporting(pin, ARD_ANALOG);
    }
    else
    {
        _firmata->sendAnalogPinReporting(pin, ARD_OFF);
   }
}

void MainWindow::goStraight()
{
//    qDebug()<<"goStraight ";

//    _firmata->sendDigital(in1, ARD_HIGH);
//    _firmata->sendDigital(in2, ARD_LOW);
//    _firmata->sendDigital(in3, ARD_HIGH);
//    _firmata->sendDigital(in4, ARD_LOW);

//    _firmata->sendPWM(enA, 150);
//    _firmata->sendPWM(enB, 180);

    #ifdef DEBUG
    qDebug()<<"goStraight ";
    #endif
    _firmata->sendDigital(digL, ARD_LOW); //L
    _firmata->sendPWM(pwmL, TURNSLOW);
    _firmata->sendDigital(digR, ARD_LOW); //R
    _firmata->sendPWM(pwmR, TURNSLOW);

    ui->actlabel->setText("GO STRAIGHT");
}

void MainWindow::goRight()
{
//    qDebug()<<"goRight ";

//    _firmata->sendDigital(in1, ARD_HIGH);
//    _firmata->sendDigital(in2, ARD_LOW);
//    _firmata->sendDigital(in3, ARD_HIGH);
//    _firmata->sendDigital(in4, ARD_LOW);

//    _firmata->sendPWM(enA, 0);
//    _firmata->sendPWM(enB, 180);

    #ifdef DEBUG
    qDebug()<<"goRight ";
    #endif
    _firmata->sendDigital(digL, ARD_LOW); //L
    _firmata->sendPWM(pwmL, TURNFAST);
    _firmata->sendDigital(digR, ARD_LOW); //R
    _firmata->sendPWM(pwmR, TURNSLOW);

        ui->actlabel->setText("GO RIGHT");
}

void MainWindow::goRightLow()
{
//    _firmata->sendDigital(in1, ARD_HIGH);
//    _firmata->sendDigital(in2, ARD_LOW);
//    _firmata->sendDigital(in3, ARD_HIGH);
//    _firmata->sendDigital(in4, ARD_LOW);

//    _firmata->sendPWM(enA, 0);
//    _firmata->sendPWM(enB, 180);

    #ifdef DEBUG
    qDebug()<<"goRightLow ";
    #endif
    _firmata->sendDigital(digL, ARD_LOW); //L
    _firmata->sendPWM(pwmL, TURNMED);
    _firmata->sendDigital(digR, ARD_LOW); //R
    _firmata->sendPWM(pwmR, TURNSLOW);

     ui->actlabel->setText("GO RIGHT SLOW");
}

void MainWindow::goLeft()
{
//    qDebug()<<"goLeft ";
//    _firmata->sendDigital(in1, ARD_HIGH);
//    _firmata->sendDigital(in2, ARD_LOW);
//    _firmata->sendDigital(in3, ARD_HIGH);
//    _firmata->sendDigital(in4, ARD_LOW);

//    _firmata->sendPWM(enA, 150);
//    _firmata->sendPWM(enB, 0);

    #ifdef DEBUG
    qDebug()<<"goLeft ";
    #endif
    _firmata->sendDigital(digL, ARD_LOW); //L
    _firmata->sendPWM(pwmL, TURNSLOW);
    _firmata->sendDigital(digR, ARD_LOW); //R
    _firmata->sendPWM(pwmR, TURNFAST);

    ui->actlabel->setText("GO LEFT");
}

void MainWindow::goLeftLow()
{
//    _firmata->sendDigital(in1, ARD_HIGH);
//    _firmata->sendDigital(in2, ARD_LOW);
//    _firmata->sendDigital(in3, ARD_HIGH);
//    _firmata->sendDigital(in4, ARD_LOW);

//    _firmata->sendPWM(enA, 150);
//    _firmata->sendPWM(enB, 0);

    #ifdef DEBUG
    qDebug()<<"goLeftLow ";
    #endif
    _firmata->sendDigital(digL, ARD_LOW); //L
    _firmata->sendPWM(pwmL, TURNSLOW);
    _firmata->sendDigital(digR, ARD_LOW); //R
    _firmata->sendPWM(pwmR, TURNMED);

     ui->actlabel->setText("GO LEFT SLOW");
}

void MainWindow::Park()
{
//    _firmata->sendDigital(in1, ARD_HIGH);
//    _firmata->sendDigital(in2, ARD_HIGH);
//    _firmata->sendDigital(in3, ARD_LOW);
//    _firmata->sendDigital(in4, ARD_LOW);

//    _firmata->sendPWM(enA, 0);
//    _firmata->sendPWM(enB, 0);

    #ifdef DEBUG
    qDebug()<<"Park ";
    #endif
    _firmata->sendDigital(digL, ARD_LOW); //L
    _firmata->sendPWM(pwmL, STOP);
    _firmata->sendDigital(digR, ARD_LOW); //R
    _firmata->sendPWM(pwmR, STOP);
    ui->actlabel->setText("PARK");
}

void MainWindow::Neutral()
{
//    qDebug()<<"Neutral ";
//    _firmata->sendDigital(in1, ARD_HIGH);
//    _firmata->sendDigital(in2, ARD_HIGH);
//    _firmata->sendDigital(in3, ARD_HIGH);
//    _firmata->sendDigital(in4, ARD_HIGH);

//    _firmata->sendPWM(enA, 0);
//    _firmata->sendPWM(enB, 0);

    #ifdef DEBUG
    qDebug()<<"Neutral ";
    #endif
    _firmata->sendDigital(digL, ARD_LOW); //L
    _firmata->sendPWM(pwmL, STOP);
    _firmata->sendDigital(digR, ARD_LOW); //R
    _firmata->sendPWM(pwmR, STOP);
    ui->actlabel->setText("NEUTRAL");
}

void MainWindow::Reverse()
{
//    qDebug()<<"Reverse ";
//    _firmata->sendDigital(in1, ARD_LOW);
//    _firmata->sendDigital(in2, ARD_HIGH);
//    _firmata->sendDigital(in3, ARD_LOW);
//    _firmata->sendDigital(in4, ARD_HIGH);

//    _firmata->sendPWM(enA, 150);
//    _firmata->sendPWM(enB, 150);

    #ifdef DEBUG
    qDebug()<<"Reverse";
    #endif
    _firmata->sendDigital(digL, ARD_HIGH); //L, ARD_LOW is the opposite direction to ARD_HIGH
    _firmata->sendPWM(pwmL, 255-TURNSLOW);
    _firmata->sendDigital(digR, ARD_HIGH); //R
    _firmata->sendPWM(pwmR, 255-TURNSLOW);

    ui->actlabel->setText("REVERSE");
}

void MainWindow::TwistLeft()
{
    #ifdef DEBUG
    qDebug()<<"Twist Left";
    #endif
    _firmata->sendDigital(digL, ARD_HIGH); //L
    _firmata->sendPWM(pwmL, 255-TURNMED);
    _firmata->sendDigital(digR, ARD_LOW); //R
    _firmata->sendPWM(pwmR, TURNMED);

    ui->actlabel->setText("TWIST LEFT");
}

void MainWindow::TwistRight()
{
    #ifdef DEBUG
    qDebug()<<"Twist Right ";
    #endif
    _firmata->sendDigital(digL, ARD_LOW); //L
    _firmata->sendPWM(pwmL, TURNMED);
    _firmata->sendDigital(digR, ARD_HIGH); //R
    _firmata->sendPWM(pwmR, 255-TURNMED);

    ui->actlabel->setText("TWIST RIGHT");
}

void MainWindow::Test()
{
//    _firmata->sendDigital(in1, ARD_HIGH);
//    _firmata->sendDigital(in2, ARD_LOW);
//    _firmata->sendDigital(in3, ARD_HIGH);
//    _firmata->sendDigital(in4, ARD_LOW);

//    _firmata->sendPWM(enA, 200);
//    _firmata->sendPWM(enB, 200);
}

void MainWindow::RobotForward(int delay)
{
//    _firmata->sendDigital(in1, ARD_HIGH);
//    _firmata->sendDigital(in2, ARD_LOW);
//    _firmata->sendDigital(in3, ARD_HIGH);
//    _firmata->sendDigital(in4, ARD_LOW);

//    _firmata->sendPWM(enA, 150);
//    _firmata->sendPWM(enB, 180);
//     QThread::sleep(delay);
}
// call this using timer periodically or onAnalogPinChanged(int pin)

void MainWindow::RobotController()
{
  //create your robot algorithm / rules here
  //over simplified example
    if (Distance > TARGET)
        goRight();
    else if (Distance < TARGET)
        goLeft();
    else
        goStraight();
}


void MainWindow::on_pushButton_clicked()
{
    goStraight();
}
void MainWindow::on_pushButton_2_clicked()
{
    goRight();
}

void MainWindow::on_pushButton_3_clicked()
{
    Reverse();
}

void MainWindow::on_pushButton_4_clicked()
{
    goLeft();
}

void MainWindow::on_pushButton_5_clicked()
{
    Neutral();
}

void MainWindow::on_pushButton_6_clicked()
{
    TwistLeft();
}

void MainWindow::on_pushButton_7_clicked()
{
    TwistRight();
}

//Algorithm: Decision Table
// Heading = FrontLeft - RearLeft
// Distance = (FrontLeft + RearLeft)/2
/*|   FrontSensor  | Distance       | Heading      |   Action   |
 *| > SafeDistance | Within Target  |   = 0 (in)   | goStraight |
 *|                | Within Target  |   > 0 (out)  |   goLeft   |
 *|                | Within Target  |   < 0 (out)  |   goRight  |
 *|                | > Target       |   = 0 (in)   |   goLeft   |
 *|                | > Target       |   > 0 (out)  |   goLeft   |
 *|                | > Target       |   < 0 (out)  | goStraight |
 *|                | < Target       |   = 0 (in)   |   goRight  |
 *|                | < Target       |   > 0 (out)  | goStraight |
 *|                | < Target       |   < 0 (out)  |   goRight  |
 *| < SafeDistance | > Target       |    x         |  TwistLeft |
 *|                | <=Target       |    x         | TwistRight |
 */

void MainWindow::ExecuteAction()
{
    heading = FrontLeft-RearLeft;
    Distance = (FrontLeft+RearLeft)/2;

    //if frontleft sensor jumps to large value, then there is gap
    //if rearleft sensor jumps to small value && frontleft no longer in gap, gap is passed
    //if both sensor senses values larger than gap distance, then it is passing the gap

    if((FrontLeft - lastFrontLeft) >= GAPDISTANCE)
    {
        gap = true;
        qDebug() << "Gap start";
    }
    if(((lastRearLeft - RearLeft) > GAPDISTANCE) && FrontLeft < GAPDISTANCE)
    {
        gap = false;
        qDebug() << "Gap end";
    }
    if(FrontLeft >= GAPDISTANCE && RearLeft >= GAPDISTANCE)
    {
        gap = true;
    }
    else
    {
        gap = false;

    }


    if(FrontSensor < SAFEDISTANCE)
    {
        if(pause) // if the robot meets a wall,
                  //it stops for a second before continuing the algorithm
        {
            Neutral();
            QTimer::singleShot(1000, this, SLOT(falsePause()));
            actionName.append("Pause");
            frontWall = 0;
        }
        else
        {
            // The robot will twist right if it meets a wall and there is a wall on the left

            //frontWall makes it so that the robot picks to twist in 1 direction and continue
            //twisting until there is no wall infront of it

            if(((RearLeft <= (TARGETfar *3/2) || FrontLeft <= (TARGETfar *3/2)) || gap)
                    && frontWall != 2)
            {
                TwistRight();
            //    Sleep(500);
                actionName.append("TWIST RIGHT");
                frontWall = 1;
            }

            else if((FrontLeft > (TARGETfar * 3/2) && RearLeft > (TARGETfar * 3/2))
                    && frontWall != 1 && !gap)
            {
                TwistLeft();
                actionName.append("TWIST LEFT");
                frontWall = 2;
            }
            else //DUMMY case for if there are cases that doesnt meet both the if and elseif
                 //Twist right here because the sensor is on the left side, so twist right is preferable over twist left
            {
                TwistRight();
                actionName.append("DUMMY TWIST RIGHT");
                frontWall = 1;
            }
        }
    }

    else if(FrontSensor > SAFEDISTANCE)
    {
//        frontWall = 0;
//        truePause();

        if(FrontSensor > 1000) //if it crashes and sticks to a wall
        {
            crash = true;
            Reverse();
            QTimer::singleShot(1000, this, SLOT(falseCrash()));
            //flips "pause" to false after 1000ms
            actionName.append("REVERSE");
        }
        else if(!crash)
        {
            if(gap) //if there is gap, force it to go straight
            {
                goStraight();
                qDebug("Gap Detected");
                actionName.append("GAP, GO STRAIGHT");
                truePause(); //flips the "pause" bool to true
            }

            else if(Distance >= TARGETclose && Distance <= TARGETfar) //Distance > (TARGET-Band) && Distance < (TARGET+Band)
                                                                      // distance within band
            {
                if(heading <= HEADINGTOP && heading >= HEADINGBOTTOM)
                {
                    goStraight();
                    actionName.append("GO STRAIGHT");
                    truePause();
                    frontWall = 0;
                    robotStuck = 0;
                }

                else if(heading >= HEADINGTOP)
                {
                        goLeft();
                        actionName.append("GO LEFT");
                        truePause();
                        frontWall = 0;
                        robotStuck = 0;
                }

                else if(heading <= HEADINGBOTTOM)
                {
                    goRight();
                    actionName.append("GO RIGHT");
                    truePause();
                    frontWall = 0;
                    robotStuck = 0;
                }
            }
            else if(Distance > TARGETfar)
            {
                if(RearLeft >= (SAFEDISTANCE *3/2) && FrontLeft >= (3/2 * SAFEDISTANCE))
                //if both the sensors sense a wall far away, twist and move closer to the wall
                {
                    //pauses the robot for a second before continuing algorithm
                    if(pause)
                    {
                        Neutral();
                        QTimer::singleShot(1000, this, SLOT(falsePause()));
                        //flips "pause" to false after 1000ms
                        actionName.append("Pause");
                        frontWall = 0;
                        robotStuck = 0;
                    }
                    else if(frontWall != 1)
                    {
                        TwistLeft();
                        actionName.append("TWIST LEFT CLOSE WALL");
                        frontWall = 2;
                        robotStuck = 0;
                    }
                    else
                    {
                        Neutral();
                        actionName.append("DUMMY");
                    }
                }
                else if(heading <= HEADINGTOP && heading >= HEADINGBOTTOM)
                {
                    goLeft();
                    actionName.append("GO LEFT");
                    truePause();
                    frontWall = 0;
                    robotStuck = 0;
                }

                else if(heading >= HEADINGTOP)
                {
                    goLeft();
                    actionName.append("GO LEFT");
                    truePause();
                    frontWall = 0;
                    robotStuck = 0;
                }

                else if(heading <= HEADINGBOTTOM)
                {
                    goStraight();
                    actionName.append("GO STRAIGHT");
                    truePause();
                    frontWall = 0;
                    robotStuck = 0;
                }
            }
            else if(Distance < TARGETclose)
            {
                if(heading <= HEADINGTOP && heading >= HEADINGBOTTOM)
                {
                    goRight();
                    actionName.append("GO RIGHT");
                    truePause();
                    frontWall = 0;
                    robotStuck = 0;
                }

                else if(heading >= HEADINGTOP)
                {
                    goStraight();
                    actionName.append("GO STRAIGHT");
                    truePause();
                    frontWall = 0;
                    robotStuck = 0;
                }

                else if(heading < HEADINGBOTTOM)
                {
                    goRight();
                    actionName.append("GO RIGHT");
                    truePause();
                    frontWall = 0;
                    robotStuck = 0;
                }
            }
        }
        else
        {
            Neutral();
            actionName.append("DUMMY NEUTRAL");
        }
    }
}



void MainWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
 {
   demoName = "Real Time Data Demo";

   // include this section to fully disable antialiasing for higher performance:
   /*
   customPlot->setNotAntialiasedElements(QCP::aeAll);
   QFont font;
   font.setStyleStrategy(QFont::NoAntialias);
   customPlot->xAxis->setTickLabelFont(font);
   customPlot->yAxis->setTickLabelFont(font);
   customPlot->legend->setFont(font);
   */
   customPlot->addGraph(); // blue line
   customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
   customPlot->addGraph(); // red line
   customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));
   //add more graph for band 2 and 3
   customPlot->addGraph(); // blue line
   customPlot->graph(2)->setPen(QPen(QColor(0 , 255, 255)));//change color
   customPlot->addGraph(); // red line
   customPlot->graph(3)->setPen(QPen(QColor(0, 255, 255)));//change color

   QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
   timeTicker->setTimeFormat("%h:%m:%s");
   customPlot->xAxis->setTicker(timeTicker);
   customPlot->axisRect()->setupFullAxesBox();
   customPlot->yAxis->setRange(0, 100); // ganti maksimum dan min valuenya biar dpt grafiknya

   // make left and bottom axes transfer their ranges to right and top axes:
   connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
   connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

   // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
   connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
   dataTimer.start(0); // Interval 0 means to refresh as fast as possible
 }

void MainWindow::realtimeDataSlot()
{
  static QTime time(QTime::currentTime());
  // calculate two new data points:
  double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
  static double lastPointKey = 0;


  if (key-lastPointKey > 0.002) // at most add point every 2 ms
  {
    // add data to lines:
    //Mofify these lines
    ui->customPlot->graph(0)->addData(key, (FrontLeft + RearLeft)/2); //untuk memberikan plot x axis
    ui->customPlot->graph(1)->addData(key, FrontSensor);
    ui->customPlot->graph(2)->addData(key, TARGETclose); //show the band (tolerance)
    ui->customPlot->graph(3)->addData(key, TARGETfar);
    lastPointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
  ui->customPlot->replot();


  // calculate frames per second:
  static double lastFpsKey;
  static int frameCount;
  ++frameCount;
  if (key-lastFpsKey > 2) // average fps over 2 seconds
  {
    lastFpsKey = key;
    frameCount = 0;
  }
}

void MainWindow::on_StartButton_clicked()
{
    setupRealtimeDataDemo(ui->customPlot);

    if(ui->StartButton->text() == "START")
    {
        timer->start(UPDATETIME);
        startState = true;
        ui->StartButton->setText("STOP");
        connect(timer, SIGNAL(timeout()), this, SLOT(ExecuteAction()));


        //Clear vectors
        AFrontLeft.clear();
        ARearLeft.clear();
        actionName.clear();
        AFrontSensor.clear();

    }
    else
    {
        timer->stop();
        startState = false;
        ui->StartButton->setText("START");
        Neutral();
    }
}

void MainWindow::on_SaveButton_clicked()
{
    qDebug() << AFrontSensor.size() << "......." << actionName.size();

    myfile.setFileName("out.txt");
    if (!myfile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    if (myfile.isOpen())
    {
        QTextStream out(&myfile);
        out << "Safe Distance: " << SAFEDISTANCE << "\nTarget: " << TARGETclose << " - " << TARGETfar ;
        out << "\nFront Sensor,    Frontleft,     Rearleft,      Distance,      ACTION\n";
        for (long i = 0; i < AFrontSensor.size(); i++) {
            out << AFrontSensor[i]  << ",          "
                << AFrontLeft[i] << ",             "
                << ARearLeft[i] << "         "
                << (ARearLeft[i]+AFrontLeft[i])/2 << "         "
                << actionName[i] << "\n";
        }
        myfile.close();
    }
}

void MainWindow::SensorPing()
{
    _firmata->sendSysEx(SONAR_CONFIG,{0});
    _firmata->sendSysEx(SONAR_CONFIG,{1});
    AFrontSensor.push_back(FrontSensor);
}

void MainWindow::on_pushButton_8_clicked()
{
    if (manual == false)
    {
        timer->stop();
        ui->pushButton_8->setText("Turn Off Manual");
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_5->setEnabled(true);
        ui->pushButton_6->setEnabled(true);
        ui->pushButton_7->setEnabled(true);
        manual = true;
    }
    else if (manual == true)
    {
        if (startState == true)
        {
            timer->start();
        }
        ui->pushButton_8->setText("Turn On Manual");
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
        ui->pushButton_6->setEnabled(false);
        ui->pushButton_7->setEnabled(false);
        manual = false;
    }
}


void MainWindow::gapStatus()
{
    gapPassed = true;
    checkGap.stop();
}

void MainWindow::falsePause()
{
    pause = false;
}

void MainWindow::truePause()
{
    pause = true;
}
 void MainWindow::falseCrash()
 {
     crash = true;
     robotStuck = 0;
 }
