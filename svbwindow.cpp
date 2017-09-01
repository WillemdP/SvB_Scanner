#include "svbwindow.h"
#include "ui_svbwindow.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <zbar.h>
#include <iostream>

using namespace cv;
using namespace std;
using namespace zbar;

SvbWindow::SvbWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SvbWindow)
{
    ui->setupUi(this);
    setWindowTitle("Stapper Databasis");

    qDebug() << "Labels for milestones, e.g. smiley or encouragement on 1-10, mild praise at 11-20, higher and higher rewards...";
    qDebug() << "No SQLite capabilities necessary. Just forward the number (do not send more than one number per second)";

    qDebug() << "Create a scanner object for each camera detected. Connect them to the TcpMessenger::send(QString result) slot.";

    qDebug() << "The big screen is divided into four squares: One for top 15 men, One for top 15 women, One for top 15 congregations, One for top 15 teams.";

    qDebug() << "Broadcast changes to all connected devices.";
    qDebug() << "Or request relevant info from server and don't display anything else.";

    qDebug() << "Set window icons.";

    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    connectToHost();

    getQRcode();
}

SvbWindow::~SvbWindow()
{
    delete ui;
}

void SvbWindow::connected()
{
    qDebug() << "Connected.";
}

void SvbWindow::disconnected()
{
    qDebug() << "Disconnected.";
}

void SvbWindow::byteswritten(qint64 bytes)
{
    qDebug() << "Packet sent.";
}

void SvbWindow::readyRead()
{
    qDebug() << "Message received:" << QString(socket->readAll());
}

void SvbWindow::getQRcode()
{
    VideoCapture cap(1); // open the video camera no. 0
    // cap.set(CV_CAP_PROP_FRAME_WIDTH,800);
    // cap.set(CV_CAP_PROP_FRAME_HEIGHT,640);
    if (!cap.isOpened()) // if not success, exit program
    {
      cout << "Cannot open the video cam" << endl;
      return;
    }
    ImageScanner scanner;
     scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
    double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
    cout << "Frame size : " << dWidth << " x " << dHeight << endl;
    namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
    while (1)
    {
      Mat frame;
      bool bSuccess = cap.read(frame); // read a new frame from video
       if (!bSuccess) //if not success, break loop
      {
         cout << "Cannot read a frame from video stream" << endl;
         break;
      }
      Mat grey;
      cvtColor(frame,grey,CV_BGR2GRAY);
      int width = frame.cols;
      int height = frame.rows;
      uchar *raw = (uchar *)grey.data;
      // wrap image data
      Image image(width, height, "Y800", raw, width * height);
      // scan the image for barcodes
      int n = scanner.scan(image);
      // extract results
      for(Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {
        vector<Point> vp;
        // do something useful with results
        cout << "decoded " << symbol->get_type_name() << " symbol \"" << symbol->get_data() << '"' <<" "<< endl;
        int n = symbol->get_location_size();
        for(int i=0;i<n;i++){
          vp.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i)));
        }
        RotatedRect r = minAreaRect(vp);
        Point2f pts[4];
        r.points(pts);
        for(int i=0;i<4;i++){
          line(frame,pts[i],pts[(i+1)%4],Scalar(255,0,0),3);
        }
        //cout<<"Angle: "<<r.angle<<endl;
      }
      imshow("MyVideo", frame); //show the frame in "MyVideo" window
      if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
      {
        cout << "esc key is pressed by user" << endl;
        break;
      }
    }
}

void SvbWindow::connectToHost()
{
    QString dialogTitle = tr("Locate host");
    QDialog dialog(this);
    dialog.setWindowTitle(dialogTitle);

    QFormLayout layout(&dialog);

    QLineEdit ipAddressInput(&dialog);

    QDialogButtonBox buttonBox(Qt::Horizontal, &dialog);
    buttonBox.addButton(QDialogButtonBox::Ok);
    buttonBox.addButton(QDialogButtonBox::Cancel);
    connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    buttonBox.setEnabled(true);

    layout.addRow(tr("Enter server IP address:"), &ipAddressInput);
    layout.addWidget(&buttonBox);


    if (dialog.exec() != QDialog::Accepted) {
        qDebug() << "Dialog rejected.";
        exit(0);
    }
    if(ipAddressInput.text()=="")
        exit(0);

    QString devIP = ipAddressInput.text();

    socket->connectToHost(devIP, 8000);
    if(!socket->waitForConnected(300))
        connectToHost();
    return;
}
/*


#include <opencv2/highgui/highgui.hpp>
 #include <opencv2/imgproc/imgproc.hpp>
 #include <zbar.h>
 #include <iostream>
 using namespace cv;
 using namespace std;
 using namespace zbar;
 //g++ main.cpp /usr/local/include/ /usr/local/lib/ -lopencv_highgui.2.4.8 -lopencv_core.2.4.8
 int main(int argc, char* argv[])
 {
   VideoCapture cap(0); // open the video camera no. 0
   // cap.set(CV_CAP_PROP_FRAME_WIDTH,800);
   // cap.set(CV_CAP_PROP_FRAME_HEIGHT,640);
   if (!cap.isOpened()) // if not success, exit program
   {
     cout << "Cannot open the video cam" << endl;
     return -1;
   }
   ImageScanner scanner;
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
   double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
   double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
   cout << "Frame size : " << dWidth << " x " << dHeight << endl;
   namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
   while (1)
   {
     Mat frame;
     bool bSuccess = cap.read(frame); // read a new frame from video
      if (!bSuccess) //if not success, break loop
     {
        cout << "Cannot read a frame from video stream" << endl;
        break;
     }
     Mat grey;
     cvtColor(frame,grey,CV_BGR2GRAY);
     int width = frame.cols;
     int height = frame.rows;
     uchar *raw = (uchar *)grey.data;
     // wrap image data
     Image image(width, height, "Y800", raw, width * height);
     // scan the image for barcodes
     int n = scanner.scan(image);
     // extract results
     for(Image::SymbolIterator symbol = image.symbol_begin();
     symbol != image.symbol_end();
     ++symbol) {
         vector<Point> vp;
     // do something useful with results
     cout << "decoded " << symbol->get_type_name() << " symbol "" << symbol->get_data() << '"' <<" "<< endl;
       int n = symbol->get_location_size();
       for(int i=0;i<n;i++){
         vp.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i)));
       }
       RotatedRect r = minAreaRect(vp);
       Point2f pts[4];
       r.points(pts);
       for(int i=0;i<4;i++){
         line(frame,pts[i],pts[(i+1)%4],Scalar(255,0,0),3);
       }
       //cout<<"Angle: "<<r.angle<<endl;
     }
     imshow("MyVideo", frame); //show the frame in "MyVideo" window
     if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
     {
       cout << "esc key is pressed by user" << endl;
       break;
     }
   }
   return 0;
 }




*/
