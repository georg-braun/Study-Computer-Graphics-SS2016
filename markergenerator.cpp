#include "markergenerator.h"

MarkerGenerator::MarkerGenerator()
{

}


///////////////////////////////////////////////////////////////////////////////////////////////////
void MarkerGenerator::createMarker() {
    // Marker erzeugen und abspeichern
    dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
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
