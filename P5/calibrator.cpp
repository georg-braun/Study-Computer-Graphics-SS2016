#include "calibrator.h"



Calibrator::Calibrator()
{

    dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);

    capWebcam.open(0);                  // associate the capture object to the default webcam

    if(capWebcam.isOpened() == false) {                 // if unsuccessful
       return;                                         //
    }

}



///////////////////////////////////////////////////////////////////////////////////////////////////
void Calibrator::calibrateCamera() {

    initializeCalibration();

    // Start Calibration Loop: Paar Beispielbilder holen mit denen kalibriert wird
    // Wenn genug Bilder gesammelt sind, dann werden diese ausgewertet.
    //qCalibrationTimer = new QTimer();                          // instantiate timer
    //QObject::connect(qCalibrationTimer, SIGNAL(timeout()), this, SLOT(calibration()));     // associate timer to processFrameAndUpdateGUI
    //qCalibrationTimer->start(40);
    calibration();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Calibrator::checkCalbrationImages() {
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

   qDebug() << "Calibration saved to " << "CameraParams.txt" << endl;

}



///////////////////////////////////////////////////////////////////////////////////////////////////
void Calibrator::initializeCalibration() {

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
void Calibrator::calibration() {
    // Bilder sammeln, die dann für die Kalibration genutzt werden

    while (allIds.size() < 2) {

        int waitTime = 10 ;

        bool blnFrameReadSuccessfully = capWebcam.read(imageOriginal);
        if (!blnFrameReadSuccessfully || imageOriginal.empty()) {                            // if we did not get a frame
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
    //        qCalibrationTimer->stop();
            checkCalbrationImages();
        }
    }


}

///////////////////////////////////////////////////////////////////////////////////////////////////

const bool Calibrator::saveCameraParams(const std::string &filename, cv::Size imageSize, float aspectRatio, int flags,
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

