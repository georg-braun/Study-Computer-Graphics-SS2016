#include "planet.h"



Planet::Planet()
{



}



void Planet::setPlanetParameter(QOpenGLShaderProgram* _shaderProgram,
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
    unifNormalMatrix   = _unifNormalMatrix;


}



void Planet::addSubPlanet( Planet* _newSubPlanet )
{
    subplantes.push_back(_newSubPlanet);
}

void Planet::bindTextures()
{
    texture->bind(0);
    shaderProgram->setUniformValue("texture",0);
}


void Planet::render()
{

    QMatrix4x4 modelMatrix ;

    modelMatrix = modelStack->top() ;   // Worauf bezieht sich das aktuelle rendern

    if ( !*paused )
    {
        // Neue Positionen bestimmen
        angleCenter  += *elapsedTime * angleCenterFactor ;
        selfRotation += *elapsedTime * selfRotationFactor ;
    }

    // Transformationen für das aktuelle Objekt machen
    modelMatrix.rotate    (angleCenter, 0, 1, 0);         // Auf welchen Winkel im Bezug auf den Ursprung soll gedreht werden? (Regeln der Umlaufgeschwindigkeit)
    modelMatrix.translate (radius , 0, 0);                                // Planet vom Ursprung "auf" die Umlaufbahn schieben
    modelMatrix.rotate    (selfRotation, 0, 1, 0) ;       // Eigendrehung

    // Beim Skalieren muss ich aufpassen. Wenn ich hier mein Koordinatensystem "kleiner" mache,
    // bezieht sich dass auch auf die Subsysteme. Deshalb skaliere ich es in dem Punkt wo ich es
    // hinhaben will und mach die Skalierung danach wieder rückgängig.
    modelMatrix.scale(scale);

    shaderProgram->bind();
    shaderProgram->setUniformValue(*unifMatrixModel,modelMatrix);

    bindTextures();

    if ( mitBeleuchtung )
    {
        // Die Normalen der Objekte auf die Weltkoordinaten beziehen.
        // Translationen sind dabei egal.
        normalMatrix = modelMatrix.normalMatrix();
        shaderProgram->setUniformValue(*unifNormalMatrix,normalMatrix);
    }

    glDrawElements ( GL_TRIANGLES,
                     *iboLength,
                     GL_UNSIGNED_INT,
                     0);

    // Koordinatensystem wieder in den ursprünglichen Zustand bringen. Nur die Entfernung bleibt bestehen.
    modelMatrix.scale(1/scale) ;    // Skalierung wieder umkehren
    // Eigendrehung des Planeten wieder rückgängig machen
    modelMatrix.rotate(-selfRotation, 0, 1, 0) ;
    // Koordinatensystem wieder zurückdrehen, damit es in der initialen Position liegtt.
    modelMatrix.rotate(-angleCenter,0,1,0);
    // Das aktuelle Objekt ist gezeichnet. Jetzt betrachtet man noch die untergeordneten Systeme.
    // Damit die wissen worauf sie sich beziehen, legt man das aktuelle Model Koordinatensystem auf den Stack
    // welches die Subplanenten dann verwenden.
    modelStack->push(modelMatrix);

    for ( unsigned int i = 0 ; i < subplantes.size() ; i++ )
    {
        subplantes[i]->render();
    }

    // Der Planet ist gezeichnet und es gibt keine Subplaneten mehr.
    // Deshalb kann man die Model-Matrix wieder vom Stack runter nehmen.
    modelStack->pop();

}
