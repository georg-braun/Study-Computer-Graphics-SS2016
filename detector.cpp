#include "detector.h"

Detector::Detector()
{
    dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);


    capWebcam.open(0);                  // associate the capture object to the default webcam

    if(capWebcam.isOpened() == false) {                 // if unsuccessful
       exitProgram();                                  // and exit program
       return;                                         //
    }


}


///////////////////////////////////////////////////////////////////////////////////////////////////
void Detector::exitProgram() {
    //if(qtimer->isActive()) qtimer->stop();          // if timer is running, stop timer
    //QApplication::quit();                           // and exit program
}

void Detector::attachArData() {

    /*
    drawArPtr = &(arDataPtr->drawAR) ;
    detectorInitializedPtr = &(arDataPtr->detectorInitialized);
    texPtr = &(arDataPtr->tex);
    modelView_matrixPtr = &(arDataPtr->modelView_matrix);
    cameraMatrixPtr = &(arDataPtr->cameraMatrix);
    */
}

void Detector::run()  {

    while ( runDetection ) {
        msleep(40);
        qDebug() << "Hello Detector Thread :)" ;
        processFrameAndUpdateGUI();
    }
}



///////////////////////////////////////////////////////////////////////////////////////////////////
bool Detector::readCameraParameters(std::string filename) {
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    if(!fs.isOpened())
        return false;
    fs["camera_matrix"] >> cameraMatrix;
    fs["distortion_coefficients"] >> distCoeffs;
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Detector::processFrameAndUpdateGUI() {

    bool drawAR = false ; // Erst mal davon ausgehen, dass kein Marker gefunden wurde

    bool blnFrameReadSuccessfully = capWebcam.read(imageOriginal);                    // get next frame from the webcam

    if (!blnFrameReadSuccessfully || imageOriginal.empty()) {                            // if we did not get a frame
        //QMessageBox::information(this, "", "unable to read from webcam \n\n exiting program\n");        // show error via message box
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
        drawAR = true ;

        //*drawArPtr = true ;

    }




    imageOriginal.copyTo(imageCopy);

    if ( ids.size() > 0 ) {     // Wurden Marker erkannt?
        cv::aruco::drawDetectedMarkers(imageCopy,corners,ids) ;

        for (unsigned int i=0 ; i < ids.size() ; i++) {
            cv::aruco::drawAxis(imageCopy, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], 0.055 * 0.5f ) ;
        }

    }


    // Weitergabe
    //QImage qimgOriginal = convertOpenCVMatToQtQImage(imageOriginal);                         // convert from OpenCV Mat to Qt QImage
 //   QImage qimgCopy = convertOpenCVMatToQtQImage(imageCopy);                       //



    convertImage(imageCopy);

    // Daten an das AR-Datenmodul weiterreichen
    arDataPtr->mutex.lock();
    cv::Mat(para.t()).copyTo( arDataPtr->modelView_matrix ); // transpose to col-major for OpenGL
    arDataPtr->tex = mRenderQtImg;
    arDataPtr->drawAR = drawAR ;  // Marker wurde gefunden, also was AR Zeug machen ;)
    // OpenGL Widget sagen, dass es nun ein Bild gibt und es anfangen kann dieses zu "zeichnen"
    arDataPtr->detectorInitialized = true ;
    arDataPtr->mutex.unlock();

    //ui->lblOriginal->setPixmap(QPixmap::fromImage(qimgOriginal));           // show images on form labels
    //ui->lblThresh->setPixmap(QPixmap::fromImage(qimgCopy));         //

    //lblWebcamOriginal->setPixmap(QPixmap::fromImage(qimgOriginal));
    //cv::imshow("Webcam",imageOriginal);
    cv::imshow("Marker",imageCopy);
    cv::moveWindow("Marker",0,0);

   rvecs.clear();
   tvecs.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Detector::initializeDetection() {

    // ToDo: Beim späteren Auslagern hier die relevanten Attribute initialiseren.
    detectorParams = cv::aruco::DetectorParameters::create();
    detectorParams->doCornerRefinement ;    // Feine Ecken (später für Pose Estimation)


    bool status = false ;
    if (estimatePos) status = readCameraParameters("CameraParams.txt") ;
        arDataPtr->cameraMatrix = cameraMatrix ;
    //*cameraMatrixPtr = cameraMatrix ;  // OpenGL die Camera Matrix geben
    //ui->widget->cameraMatrix = cameraMatrix ;  // OpenGL die Camera Matrix geben


    //ui->widget->readyToCalcProjection = true ;
    //ui->widget->loadProjectionMatrix();
    return status ;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void Detector::calcModelViewMatrixFirstId() {


    cv::Mat Rvec (rvecs[0],CV_32F) ;
    cv::Mat Tvec (tvecs[0],CV_32F) ;


    cv::Mat Rot(3,3,CV_32FC1);
    cv::Rodrigues(Rvec,Rot);

    // [R | t] matrix
    para = cv::Mat_<double>::eye(4,4);
    Rot.convertTo(para(cv::Rect(0,0,3,3)),CV_64F);
    Tvec.copyTo(para(cv::Rect(3,0,1,3)));

    cv::Mat cvToGl = cv::Mat::zeros(4, 4, CV_64F);
    cvToGl.at<double>(0, 0) = 1.0f;
    cvToGl.at<double>(1, 1) = -1.0f; // Invert the y axis
    cvToGl.at<double>(2, 2) = -1.0f; // invert the z axis
    cvToGl.at<double>(3, 3) = 1.0f;

    para = cvToGl * para;

    //cv::Mat(para.t()).copyTo( *modelView_matrixPtr ); // transpose to col-major for OpenGL



}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Detector::startDetection() {



    bool start = false ;
    start = initializeDetection() ;

    /*
    qtimer = new QTimer(this);                          // instantiate timer
    connect(qtimer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGUI()));     // associate timer to processFrameAndUpdateGUI

    if ( start ) {
        qtimer->start(40);
    }
    */

}


///////////////////////////////////////////////////////////////////////////////////////////////////
void Detector::stopDetection() {

    //qtimer->stop();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

QImage Detector::convertOpenCVMatToQtQImage(cv::Mat mat) {
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

void Detector::convertImage(cv::Mat image)
{

    image.copyTo(mOrigImage);

    //mImgRatio = (float)image.cols/(float)image.rows;

    if( mOrigImage.channels() == 3)
        mRenderQtImg = QImage((const unsigned char*)(mOrigImage.data),
                              mOrigImage.cols, mOrigImage.rows,
                              mOrigImage.step, QImage::Format_RGB888).rgbSwapped();
    else if( mOrigImage.channels() == 1)
        mRenderQtImg = QImage((const unsigned char*)(mOrigImage.data),
                              mOrigImage.cols, mOrigImage.rows,
                              mOrigImage.step, QImage::Format_Indexed8);

   // mRenderQtImg = mRenderQtImg.mirrored();



     //*texPtr = mRenderQtImg;

    // OpenGL Widget sagen, dass es nun ein Bild gibt und es anfangen kann dieses zu "zeichnen"
    //*detectorInitializedPtr = true ;

    // Berechne Model View für den ersten Marker

    //updateGL();
}
