#include "detector.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////////////

void Detector::run()  {

    while ( runDetection ) {
        msleep(40);
//        qDebug() << "Hello Detector Thread :)" ;
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
    marker0Detected = false ;
    marker1Detected = false ;
    marker2Detected = false ;
    marker3Detected = false ;
    marker4Detected = false ;


    bool blnFrameReadSuccessfully = capWebcam.read(imageOriginal);                    // get next frame from the webcam

    if (!blnFrameReadSuccessfully || imageOriginal.empty()) {                            // if we did not get a frame
        //QMessageBox::information(this, "", "unable to read from webcam \n\n exiting program\n");        // show error via message box
        exitProgram();                                                              // and exit program
        return;                                                                     //
    }

    std::vector< int > ids;
    std::vector< std::vector< cv::Point2f > > corners, rejected;

    cv::aruco::detectMarkers(imageOriginal, dictionary, corners, ids, detectorParams, rejected) ;

    if ( ids.size() > 0 ) {
        cv::aruco::estimatePoseSingleMarkers(corners, 0.055f, cameraMatrix, distCoeffs, rvecs, tvecs) ;

        for (unsigned int i=0 ; i < ids.size() ; i++) {
            if ( ids.at(i) == 0 ) {
                marker0Detected = true ;
                marker0ArrayPos = i ;
            }
            if ( ids.at(i) == 1 ) {
                marker1Detected = true ;
                marker1ArrayPos = i ;
            }
            if ( ids.at(i) == 2 ) {
                marker2Detected = true ;
                marker2ArrayPos = i ;
            }
            if ( ids.at(i) == 3 ) {
                marker3Detected = true ;
                marker3ArrayPos = i ;
            }
            if ( ids.at(i) == 4 ) {
                marker4Detected = true ;
                marker4ArrayPos = i ;
            }
        }

        // Berechnungen der ModelView Matritzen
        calcModelViewMatrices();
        // Es wurden Marker gefunden, also darf OpenGL später auch loslegen ;)
        drawAR = true ;
    }


    // Einzeichnen der Koordinatensystem auf den Markern
    imageOriginal.copyTo(imageCopy);
    if ( ids.size() > 0 ) {     // Wurden Marker erkannt?
        cv::aruco::drawDetectedMarkers(imageCopy,corners,ids) ;

        for (unsigned int i=0 ; i < ids.size() ; i++) {
            cv::aruco::drawAxis(imageCopy, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], 0.055f * 0.5f ) ;
        }

    }



    // Weitergabe
    //QImage qimgOriginal = convertOpenCVMatToQtQImage(imageOriginal);                         // convert from OpenCV Mat to Qt QImage
    cv::Mat imageDisplayCoord ;
    imageCopy.copyTo(imageDisplayCoord);
    QImage qimgCoord = convertOpenCVMatToQtQImage(imageDisplayCoord);                       //


    convertImage(imageCopy);    // Bild für OpenGL aufbereiten

    // Daten an das AR-Datenmodul weiterreichen
    arDataPtr->mutex.lock();


    marker0ModelView.copyTo( arDataPtr->marker0modelView_matrix );
    marker1ModelView.copyTo( arDataPtr->marker1modelView_matrix );
    marker2ModelView.copyTo( arDataPtr->marker2modelView_matrix );
    marker3ModelView.copyTo( arDataPtr->marker3modelView_matrix );
    marker4ModelView.copyTo( arDataPtr->marker4modelView_matrix );

    arDataPtr->marker0Detected = marker0Detected ;
    arDataPtr->marker1Detected = marker1Detected ;
    arDataPtr->marker2Detected = marker2Detected ;
    arDataPtr->marker3Detected = marker3Detected ;
    arDataPtr->marker4Detected = marker4Detected ;

    arDataPtr->tex = mRenderQtImg;
    arDataPtr->drawAR = drawAR ;  // Marker wurde gefunden, also was AR Zeug machen ;)
    // OpenGL Widget sagen, dass es nun ein Bild gibt und es anfangen kann dieses zu "zeichnen"
    arDataPtr->detectorInitialized = true ;


    arDataPtr->mutex.unlock();


    lblOriginal->setPixmap(QPixmap::fromImage(qimgCoord));
    //ui->lblOriginal->setPixmap(QPixmap::fromImage(qimgOriginal));           // show images on form labels
    //ui->lblThresh->setPixmap(QPixmap::fromImage(qimgCopy));         //

    //cv::imshow("Marker",imageCopy);
    //cv::moveWindow("Marker",0,0);

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
    if (status) arDataPtr->cameraMatrix = cameraMatrix ;
    return status ;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Detector::calcModelViewMatrices() {


    cv::Mat cvToGl = cv::Mat::zeros(4, 4, CV_64F);
    cvToGl.at<double>(0, 0) = 1.0f;
    cvToGl.at<double>(1, 1) = -1.0f; // Invert the y axis
    cvToGl.at<double>(2, 2) = -1.0f; // invert the z axis
    cvToGl.at<double>(3, 3) = 1.0f;


    if ( marker0Detected ) {
        cv::Mat marker0Rvec (rvecs[marker0ArrayPos],CV_32F) ;
        cv::Mat marker0Tvec (tvecs[marker0ArrayPos],CV_32F) ;

        cv::Mat Rot(3,3,CV_32FC1);
        cv::Rodrigues(marker0Rvec,Rot);

        // [R | t] matrix
        marker0ModelView = cv::Mat_<double>::eye(4,4);
        Rot.convertTo(marker0ModelView(cv::Rect(0,0,3,3)),CV_64F);
        marker0Tvec.copyTo(marker0ModelView(cv::Rect(3,0,1,3)));

        marker0ModelView = cvToGl * marker0ModelView;
    }

    if ( marker1Detected ) {
        cv::Mat marker1Rvec (rvecs[marker1ArrayPos],CV_32F) ;
        cv::Mat marker1Tvec (tvecs[marker1ArrayPos],CV_32F) ;

        cv::Mat Rot(3,3,CV_32FC1);
        cv::Rodrigues(marker1Rvec,Rot);

        // [R | t] matrix
        marker1ModelView = cv::Mat_<double>::eye(4,4);
        Rot.convertTo(marker1ModelView(cv::Rect(0,0,3,3)),CV_64F);
        marker1Tvec.copyTo(marker1ModelView(cv::Rect(3,0,1,3)));

        marker1ModelView = cvToGl * marker1ModelView;
    }

    if ( marker2Detected ) {
        cv::Mat marker2Rvec (rvecs[marker2ArrayPos],CV_32F) ;
        cv::Mat marker2Tvec (tvecs[marker2ArrayPos],CV_32F) ;

        cv::Mat Rot(3,3,CV_32FC1);
        cv::Rodrigues(marker2Rvec,Rot);

        // [R | t] matrix
        marker2ModelView = cv::Mat_<double>::eye(4,4);
        Rot.convertTo(marker2ModelView(cv::Rect(0,0,3,3)),CV_64F);
        marker2Tvec.copyTo(marker2ModelView(cv::Rect(3,0,1,3)));

        marker2ModelView = cvToGl * marker2ModelView;
    }

    if ( marker3Detected ) {
        cv::Mat marker3Rvec (rvecs[marker3ArrayPos],CV_32F) ;
        cv::Mat marker3Tvec (tvecs[marker3ArrayPos],CV_32F) ;

        cv::Mat Rot(3,3,CV_32FC1);
        cv::Rodrigues(marker3Rvec,Rot);

        // [R | t] matrix
        marker3ModelView = cv::Mat_<double>::eye(4,4);
        Rot.convertTo(marker3ModelView(cv::Rect(0,0,3,3)),CV_64F);
        marker3Tvec.copyTo(marker3ModelView(cv::Rect(3,0,1,3)));

        marker3ModelView = cvToGl * marker3ModelView;
    }

    if ( marker4Detected ) {
        cv::Mat marker4Rvec (rvecs[marker4ArrayPos],CV_32F) ;
        cv::Mat marker4Tvec (tvecs[marker4ArrayPos],CV_32F) ;

        cv::Mat Rot(3,3,CV_32FC1);
        cv::Rodrigues(marker4Rvec,Rot);

        // [R | t] matrix
        marker4ModelView = cv::Mat_<double>::eye(4,4);
        Rot.convertTo(marker4ModelView(cv::Rect(0,0,3,3)),CV_64F);
        marker4Tvec.copyTo(marker4ModelView(cv::Rect(3,0,1,3)));

        marker4ModelView = cvToGl * marker4ModelView;
    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Detector::startDetection() {

    bool start = false ;
    start = initializeDetection() ;
    // ToDo: Hier noch Wert mit zurückgeben, falls die Initialisierung nicht geklappt hat!
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

///////////////////////////////////////////////////////////////////////////////////////////////////

void Detector::convertImage(cv::Mat image)
{

    image.copyTo(mOrigImage);

    if( mOrigImage.channels() == 3)
        mRenderQtImg = QImage((const unsigned char*)(mOrigImage.data),
                              mOrigImage.cols, mOrigImage.rows,
                              mOrigImage.step, QImage::Format_RGB888).rgbSwapped();
    else if( mOrigImage.channels() == 1)
        mRenderQtImg = QImage((const unsigned char*)(mOrigImage.data),
                              mOrigImage.cols, mOrigImage.rows,
                              mOrigImage.step, QImage::Format_Indexed8);

}
