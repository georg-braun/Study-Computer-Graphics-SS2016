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

protected:

    // Planetenparameter
    std::vector<Planet*> subplantes ;
    int       radius ;                // Abstand zum übergeordneten Planeten
    double    angleCenterFactor ;     // Faktor: Aktueller Rotationswinkel bezogen auf den übergeordneten Planeten (Umlauf)
    double    selfRotationFactor ;    // Faktor: Eigendrehung des Planeten
    double    scale ;                 // Skalierungsfaktor für den Planet
    double    angleCenter  = 0 ;
    double    selfRotation = 0 ;

    // Refrenzen zum Rednern
    unsigned int*           iboLength ;
    QOpenGLShaderProgram*   shaderProgram ;
    int*                    unifMatrixModel ;
    int*                    unifNormalMatrix;
    int*                    elapsedTime ;
    std::stack<QMatrix4x4>* modelStack ;
    QOpenGLTexture*         texture ;
    bool*                   paused ;
    bool                    mitBeleuchtung = true ;
    QMatrix3x3              normalMatrix ;


    virtual void bindTextures() ;


public:
    Planet();
    virtual ~Planet() {};

    void setPlanetParameter(QOpenGLShaderProgram* _shaderProgram,
                            int* _unifMatrixModel,
                            int* _unifNormalMatrix,
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
