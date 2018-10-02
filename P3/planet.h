#ifndef PLANET_H
#define PLANET_H

#include <vector>
#include <stack>
#include <QOpenGLShaderProgram>  // Shader
#include <QOpenGLTexture>
#include <strings.h>
#include <enums.h>
class Planet
{
    // Planetenparameter
    std::vector<Planet*> subplantes ;
    int    radius ;    // Abstand zum übergeordneten Planeten
    double    angleCenterFactor ;     // Faktor: Aktueller Rotationswinkel bezogen auf den übergeordneten Planeten (Umlauf)
    double    selfRotationFactor ;    // Faktor: Eigendrehung des Planeten
    double scale ;     // Skalierungsfaktor für den Planet
    unsigned int* iboLength ;
    QOpenGLShaderProgram* shaderProgram ;
    int* unifMatrixModel ;
    int* elapsedTime ;
    std::stack<QMatrix4x4>* modelStack ;

    // Aktuelle Werte
    double angleCenter  = 0 ;
    double selfRotation = 0 ;
    bool*  paused ;

    // Textures
    QOpenGLTexture *texture ;
    QImage textureImage ;



public:
    Planet();
    void loadTexture() ;
    void setPlanetParameter(QOpenGLShaderProgram* _shaderProgram,
                            int* _unifMatrixModel,
                            std::stack<QMatrix4x4>* _modelStack,
                            unsigned int* _iboLength,
                            QOpenGLTexture *_texture,
                            int* _elapsedTime,
                            bool* _paused,
                            int _radius,
                            double _angleCenterFactor ,
                            double _selfRotationFactor ,
                            double _scale );
    void addSubPlanet( Planet* _newSubPlanet ) ;
    void render() ;

};

#endif // PLANET_H
