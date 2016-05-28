#include "markergenerator.h"

MarkerGenerator::MarkerGenerator()
{

}


///////////////////////////////////////////////////////////////////////////////////////////////////
void MarkerGenerator::createMarker() {
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
