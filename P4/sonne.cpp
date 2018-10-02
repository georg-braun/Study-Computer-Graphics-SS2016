#include "sonne.h"

Sonne::Sonne()
{

}


void Sonne::bindTextures()
{
    /*
    int unifSun = 0 ;
    int uniSunSphere = 1 ;

    unifSun = shaderProgram->uniformLocation("texture") ;
    uniSunSphere = shaderProgram->uniformLocation("textureSphere") ;
*/

    texture->bind(0);
    shaderProgram->setUniformValue("texture",0);

    textureSunSphere->bind(1);
    shaderProgram->setUniformValue("textureSphere",1);




}


void Sonne::setPlanetParameter(QOpenGLShaderProgram* _shaderProgram,
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
                                double _scale )
{
    shaderProgram      = _shaderProgram ;
    unifMatrixModel    = _unifMatrixModel ;
    elapsedTime        = _elapsedTime ;
    modelStack         = _modelStack ;
    radius             = _radius ;
    angleCenterFactor  = _angleCenterFactor ;
    selfRotationFactor = _selfRotationFactor ;
    scale              = _scale ;
    iboLength          = _iboLength ;
    paused             = _paused ;
    texture            = _texture ;
    textureSunSphere   = _textureSunSphere ;
    mitBeleuchtung     = false ;

}
