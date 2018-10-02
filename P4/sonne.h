#ifndef SONNE_H
#define SONNE_H

#include "planet.h"


class Sonne : public Planet
{

protected:
    QOpenGLTexture *textureSunSphere ;
    void bindTextures() ;

public:
    Sonne();




    void setPlanetParameter(QOpenGLShaderProgram* _shaderProgram,
                            int* _unifMatrixModel,
                            std::stack<QMatrix4x4>* _modelStack,
                            unsigned int* _iboLength,
                            QOpenGLTexture *_texture,
                            QOpenGLTexture *_textureSunSphere,
                            int* _elapsedTime,
                            bool* _paused,
                            int _radius,
                            double _angleCenterFactor ,
                            double _selfRotationFactor ,
                            double _scale );

};

#endif // SONNE_H
