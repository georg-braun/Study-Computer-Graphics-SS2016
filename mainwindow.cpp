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

    dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);


    capWebcam.open(0);                  // associate the capture object to the default webcam

    if(capWebcam.isOpened() == false) {                 // if unsuccessful
       exitProgram();                                  // and exit program
       return;                                         //
    }



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
void MainWindow::calibrateCamera() {

    initializeCalibration();

    // Start Calibration Loop: Paar Beispielbilder holen mit denen kalibriert wird
    // Wenn genug Bilder gesammelt sind, dann werden diese ausgewertet.
    qCalibrationTimer = new QTimer(this);                          // instantiate timer
    connect(qCalibrationTimer, SIGNAL(timeout()), this, SLOT(calibration()));     // associate timer to processFrameAndUpdateGUI
    qCalibrationTimer->start(40);

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::checkCalbrationImages() {
    // Auswertung der Calibration Bilder
    if(allIds.size() < 1) {
        std::cerr << "Not enough captures for calibration" << endl;

    }


    markerCounterPerFrame.reserve(allCorners.size());
    for(unsigned int i = 0; i < allCorners.size(); i++) {
        markerCounterPerFrame.push_back((int)allCorners[i].size());
        for(unsigned int j = 0; j < allCorners[i].size(); j++) {
            allCornersConcatenated.push_back(allCorners[i][j]);
            allIdsConcatenated.push_back(allIds[i][j]);
        }
    }

    // calibrate camera
   repError = cv::aruco::calibrateCameraAruco(allCornersConcatenated, allIdsConcatenated,
                                              markerCounterPerFrame, board, imgSize, cameraMatrix,
                                              distCoeffs, rvecsCalib, tvecsCalib, calibrationFlags);

   bool saveOk = saveCameraParams("CameraParams.txt", imgSize, 0, calibrationFlags, cameraMatrix,
                                      distCoeffs, repError);

   if(!saveOk) {
       qDebug() << "Cannot save output file" << endl;
   }

   qDebug() << "Rep Error: " << repError << endl;
   qDebug() << "Calibration saved to " << "CameraParams.txt" << endl;

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::createMarker() {
    // Marker erzeugen und abspeichern
    cv::Mat matMarkerImg ;
    std::string name = "";
    std::string jpg = ".jpg";
    std::string complete ;
    for (int i = 0 ; i < 50 ; i++) {
        complete = std::to_string(i)+jpg;
        cv::aruco::drawMarker(dictionary,i,200,matMarkerImg,1);
        cv::imwrite(complete,matMarkerImg);
    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::initializeCalibration() {

    // Detector
    detectorParams = cv::aruco::DetectorParameters::create();
    detectorParams->doCornerRefinement ;

    // create board object
    gridboard = cv::aruco::GridBoard::create(4,4,0.05f,0.01f,dictionary,1) ;
    board = gridboard.staticCast<cv::aruco::Board>();

    // Initialize
    cv::Mat markerBoard ;
    cv::Size boardSize = cv::Size(400,400);
    cv::aruco::drawPlanarBoard(board,boardSize,markerBoard);
    cv::imwrite("Board.jpg",markerBoard);

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::calibration() {
    // Bilder sammeln, die dann für die Kalibration genutzt werden

    int waitTime = 10 ;

    bool blnFrameReadSuccessfully = capWebcam.read(imageOriginal);
    if (!blnFrameReadSuccessfully || imageOriginal.empty()) {                            // if we did not get a frame
        QMessageBox::information(this, "", "unable to read from webcam \n\n exiting program\n");        // show error via message box
        exitProgram();                                                              // and exit program
        return;                                                                     //
    }
    else {

        // detect markers
        cv::aruco::detectMarkers(imageOriginal, dictionary, corners, ids, detectorParams, rejected);

        cv::aruco::refineDetectedMarkers(imageOriginal,board, corners, ids, rejected);

        // Draw results
        imageOriginal.copyTo(imageCopy) ;
        if(ids.size() > 0) cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
        putText(imageCopy, "Press 'c' to add current frame. 'ESC' to finish and calibrate",
                cv::Point(10, 20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 2);

        // Bild anzeigen
        cv::imshow("Calibration",imageCopy);
        qDebug() << "IDs: " << ids.size() << "  AllIDs: " << allIds.size();

        // Auf Tastendruck ein Bild zur Kalibirerung auswählen
        char key = (char)cv::waitKey(waitTime);
        if(key == 'c' && ids.size() > 0) {
            allCorners.push_back(corners);
            allIds.push_back(ids);
            imgSize = imageOriginal.size();
        }

    }


    if(allIds.size() >= 2) {
        // Genug bilder gesammelt.
        qCalibrationTimer->stop();
        checkCalbrationImages();
    }


}

///////////////////////////////////////////////////////////////////////////////////////////////////

const bool MainWindow::saveCameraParams(const std::string &filename, cv::Size imageSize, float aspectRatio, int flags,
                             const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs, double totalAvgErr) {
    cv::FileStorage fs(filename, cv::FileStorage::WRITE);
    if(!fs.isOpened())
        return false;

    time_t tt;
    std::time(&tt);
    struct tm *t2 = localtime(&tt);
    char buf[1024];
    strftime(buf, sizeof(buf) - 1, "%c", t2);

    fs << "calibration_time" << buf;

    fs << "image_width" << imageSize.width;
    fs << "image_height" << imageSize.height;

    if(flags & cv::CALIB_FIX_ASPECT_RATIO) fs << "aspectRatio" << aspectRatio;

    if(flags != 0) {
        sprintf(buf, "flags: %s%s%s%s",
                flags & cv::CALIB_USE_INTRINSIC_GUESS ? "+use_intrinsic_guess" : "",
                flags & cv::CALIB_FIX_ASPECT_RATIO ? "+fix_aspectRatio" : "",
                flags & cv::CALIB_FIX_PRINCIPAL_POINT ? "+fix_principal_point" : "",
                flags & cv::CALIB_ZERO_TANGENT_DIST ? "+zero_tangent_dist" : "");
    }

    fs << "flags" << flags;

    fs << "camera_matrix" << cameraMatrix;
    fs << "distortion_coefficients" << distCoeffs;
    fs << "avg_reprojection_error" << totalAvgErr;

    /*
    cv::Mat testMat ;
    cameraMatrix.convertTo(testMat,CV_32F);
    cv::write(fs,"testMat12",testMat.at<float>(1,2));
    */
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool MainWindow::readCameraParameters(std::string filename) {
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    if(!fs.isOpened())
        return false;
    fs["camera_matrix"] >> cameraMatrix;
    fs["distortion_coefficients"] >> distCoeffs;
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::processFrameAndUpdateGUI() {

    bool blnFrameReadSuccessfully = capWebcam.read(imageOriginal);                    // get next frame from the webcam

    if (!blnFrameReadSuccessfully || imageOriginal.empty()) {                            // if we did not get a frame
        QMessageBox::information(this, "", "unable to read from webcam \n\n exiting program\n");        // show error via message box
        exitProgram();                                                              // and exit program
        return;                                                                     //
    }


    std::vector< int > ids;
    std::vector< std::vector< cv::Point2f > > corners, rejected;
    //std::vector< cv::Vec3d > rvecs, tvecs;


    cv::aruco::detectMarkers(imageOriginal, dictionary, corners, ids, detectorParams, rejected) ;

    if ( ids.size() > 0 ) {
        cv::aruco::estimatePoseSingleMarkers(corners, 0.055, cameraMatrix, distCoeffs, rvecs, tvecs) ;
        calcModelViewMatrixFirstId();
        ui->widget->drawAR = true ;

    }




    imageOriginal.copyTo(imageCopy);

    if ( ids.size() > 0 ) {     // Wurden Marker erkannt?
        cv::aruco::drawDetectedMarkers(imageCopy,corners,ids) ;

        for (unsigned int i=0 ; i < ids.size() ; i++) {
            cv::aruco::drawAxis(imageCopy, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], 0.055 * 0.5f ) ;
        }

    }


    // Weitergabe
    QImage qimgOriginal = convertOpenCVMatToQtQImage(imageOriginal);                         // convert from OpenCV Mat to Qt QImage
    QImage qimgCopy = convertOpenCVMatToQtQImage(imageCopy);                       //


    ui->widget->showImage(imageCopy);

    ui->lblOriginal->setPixmap(QPixmap::fromImage(qimgOriginal));           // show images on form labels
    ui->lblThresh->setPixmap(QPixmap::fromImage(qimgCopy));         //


   rvecs.clear();
   tvecs.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool MainWindow::initializeDetection() {

    // ToDo: Beim späteren Auslagern hier die relevanten Attribute initialiseren.
    detectorParams = cv::aruco::DetectorParameters::create();
    detectorParams->doCornerRefinement ;    // Feine Ecken (später für Pose Estimation)


    bool status = false ;
    if (estimatePos) status = readCameraParameters("CameraParams.txt") ;
    ui->widget->cameraMatrix = cameraMatrix ;  // OpenGL die Camera Matrix geben

    ui->widget->readyToCalcProjection = true ;
    ui->widget->loadProjectionMatrix();
    return status ;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::calcModelViewMatrixFirstId() {
/*

    cv::Mat Rvec (rvecs[0],CV_32F) ;
    cv::Mat Tvec (tvecs[0],CV_32F) ;


    cv::Mat Rot(3,3,CV_32FC1);
    cv::Rodrigues(Rvec,Rot);


     //Rot = rmtx
    cv::Mat view_matrix = cv::Mat::zeros(4, 4, CV_64F);

    view_matrix.at<double>(0,0) = Rot.at<double>(0, 0) ;
    view_matrix.at<double>(0,1) = Rot.at<double>(0, 1) ;
    view_matrix.at<double>(0,2) = Rot.at<double>(0, 2) ;
    view_matrix.at<double>(0,3) = tvecs[0][0] ;
    view_matrix.at<double>(1,0) = Rot.at<double>(1, 0) ;
    view_matrix.at<double>(1,1) = Rot.at<double>(1, 1) ;
    view_matrix.at<double>(1,2) = Rot.at<double>(1, 2) ;
    view_matrix.at<double>(1,3) = tvecs[0][1] ;
    view_matrix.at<double>(2,0) = Rot.at<double>(2, 0) ;
    view_matrix.at<double>(2,1) = Rot.at<double>(2, 1) ;
    view_matrix.at<double>(2,2) = Rot.at<double>(2, 2) ;
    view_matrix.at<double>(2,3) = tvecs[0][2];
    view_matrix.at<double>(3,0) = 0.0 ;
    view_matrix.at<double>(3,1) = 0.0 ;
    view_matrix.at<double>(3,2) = 0.0 ;
    view_matrix.at<double>(3,3) = 1.0 ;



    cv::Mat inverse_matrix(4,4,CV_64F);
    inverse_matrix.at<double>(0,0) = 1.0 ;
    inverse_matrix.at<double>(0,1) = 1.0 ;
    inverse_matrix.at<double>(0,2) = 1.0 ;
    inverse_matrix.at<double>(0,3) = 1.0 ;
    inverse_matrix.at<double>(1,0) = -1.0 ;
    inverse_matrix.at<double>(1,1) = -1.0 ;
    inverse_matrix.at<double>(1,2) = -1.0 ;
    inverse_matrix.at<double>(1,3) = -1.0 ;
    inverse_matrix.at<double>(2,0) = -1.0 ;
    inverse_matrix.at<double>(2,1) = -1.0 ;
    inverse_matrix.at<double>(2,2) = -1.0 ;
    inverse_matrix.at<double>(2,3) = -1.0 ;
    inverse_matrix.at<double>(3,0) = 1.0 ;
    inverse_matrix.at<double>(3,1) = 1.0 ;
    inverse_matrix.at<double>(3,2) = 1.0 ;
    inverse_matrix.at<double>(3,3) = 1.0 ;

    view_matrix = view_matrix * inverse_matrix ;

    cv::Mat view_matrix_t(4,4,CV_64F);
    cv::transpose(view_matrix,view_matrix_t) ;

    view_matrix_t.copyTo( ui->widget->modelView_matrix  );
*/




    cv::Mat Rvec (rvecs[0],CV_32F) ;
    cv::Mat Tvec (tvecs[0],CV_32F) ;


    cv::Mat Rot(3,3,CV_32FC1);
    cv::Rodrigues(Rvec,Rot);

    // [R | t] matrix
    cv::Mat_<double> para = cv::Mat_<double>::eye(4,4);
    Rot.convertTo(para(cv::Rect(0,0,3,3)),CV_64F);
    Tvec.copyTo(para(cv::Rect(3,0,1,3)));

    cv::Mat cvToGl = cv::Mat::zeros(4, 4, CV_64F);
    cvToGl.at<double>(0, 0) = 1.0f;
    cvToGl.at<double>(1, 1) = -1.0f; // Invert the y axis
    cvToGl.at<double>(2, 2) = -1.0f; // invert the z axis
    cvToGl.at<double>(3, 3) = 1.0f;

    para = cvToGl * para;

    cv::Mat(para.t()).copyTo( ui->widget->modelView_matrix ); // transpose to col-major for OpenGL


}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::startDetection() {

    bool start = false ;
    start = initializeDetection() ;

    qtimer = new QTimer(this);                          // instantiate timer
    connect(qtimer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGUI()));     // associate timer to processFrameAndUpdateGUI

    if ( start ) {
        qtimer->start(40);
    }

}


///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::stopDetection() {

    qtimer->stop();
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
