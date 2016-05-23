#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QtCore>
#include<QMessageBox>
#include<string.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dictonary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);

    /*
    // Marker erzeugen und abspeichern
    cv::Mat matMarkerImg ;
    std::string name = "";
    std::string jpg = ".jpg";
    std::string complete ;
    for (int i = 0 ; i < 50 ; i++) {
        complete = std::to_string(i)+jpg;
        cv::aruco::drawMarker(dictonary,i,200,matMarkerImg,1);
        cv::imwrite(complete,matMarkerImg);
    }
    */

    capWebcam.open(0);                  // associate the capture object to the default webcam

       if(capWebcam.isOpened() == false) {                 // if unsuccessful
           exitProgram();                                  // and exit program
           return;                                         //
       }


       qtimer = new QTimer(this);                          // instantiate timer
       connect(qtimer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGUI()));     // associate timer to processFrameAndUpdateGUI
       qtimer->start(40);

}




MainWindow::~MainWindow()
{
    delete ui;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::exitProgram() {
    if(qtimer->isActive()) qtimer->stop();          // if timer is running, stop timer
    QApplication::quit();                           // and exit program
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::processFrameAndUpdateGUI() {
    cv::Mat imgOriginal;                    // original image
    cv::Mat imgCopy;
    cv::Mat imgHSV;
    cv::Mat imgThreshLow;
    cv::Mat imgThreshHigh;
    cv::Mat imgThresh;

    bool blnFrameReadSuccessfully = capWebcam.read(imgOriginal);                    // get next frame from the webcam

    if (!blnFrameReadSuccessfully || imgOriginal.empty()) {                            // if we did not get a frame
        QMessageBox::information(this, "", "unable to read from webcam \n\n exiting program\n");        // show error via message box
        exitProgram();                                                              // and exit program
        return;                                                                     //
    }




    // ArUco - Marker ;)
    cv::Ptr<cv::aruco::DetectorParameters> detectorParams = cv::aruco::DetectorParameters::create();
    std::vector< int > ids;
    std::vector< std::vector< cv::Point2f > > corners, rejected;
    std::vector< cv::Vec3d > rvecs, tvecs;

    detectorParams->doCornerRefinement ;    // Feine Ecken (später für Pose Estimation)

    cv::aruco::detectMarkers(imgOriginal, dictonary, corners, ids, detectorParams, rejected) ;

    imgOriginal.copyTo(imgCopy);

    if ( ids.size() > 0 ) {     // Wurden Marker erkannt?
        cv::aruco::drawDetectedMarkers(imgCopy,corners,ids) ;
    }

    //cv::aruco::calibrateCameraAruco()





    // Weitergabe
    QImage qimgOriginal = convertOpenCVMatToQtQImage(imgOriginal);                         // convert from OpenCV Mat to Qt QImage
    QImage qimgCopy = convertOpenCVMatToQtQImage(imgCopy);                       //

    //ui->widget->sendMakerPos(v3fCircles);
    ui->widget->showImage(imgCopy);


    ui->lblOriginal->setPixmap(QPixmap::fromImage(qimgOriginal));           // show images on form labels

    ui->lblThresh->setPixmap(QPixmap::fromImage(qimgCopy));         //


}

///////////////////////////////////////////////////////////////////////////////////////////////////

QImage MainWindow::convertOpenCVMatToQtQImage(cv::Mat mat) {
    if(mat.channels() == 1) {                                   // if 1 channel (grayscale or black and white) image
        return QImage((uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);     // return QImage
    } else if(mat.channels() == 3) {                            // if 3 channel color image
        cv::cvtColor(mat, mat, CV_BGR2RGB);                     // flip colors
        return QImage((uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);       // return QImage
    } else {
        qDebug() << "in convertOpenCVMatToQtQImage, image was not 1 channel or 3 channel, should never get here";
    }
    return QImage();        // return a blank QImage if the above did not work
}
