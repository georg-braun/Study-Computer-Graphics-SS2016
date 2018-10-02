#include "MyGLWidget.h"
#include <iostream>
#include <math.h>


MyGLWidget::MyGLWidget()
{

}



MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(parent)
    , vbo(QOpenGLBuffer::VertexBuffer)
    , ibo(QOpenGLBuffer::IndexBuffer)
{

     std::cout << glGetString (GL_VERSION ) ;
     setFocusPolicy(Qt::StrongFocus);               // To catch the KeyPressEvents.

     QTimer *timer = new QTimer(this) ;
     connect(timer, SIGNAL(timeout()),this,SLOT(updateGL()));
     timer->start(0);

}

MyGLWidget::~MyGLWidget()
{
    qTex->release();
    shaderProgram.release();

    vbo.release();
    ibo.release();

    // Deaktiviere die Verwendung der Attribute Arrays
    shaderProgram.disableAttributeArray(attrVertices);
    //shaderProgram.disableAttributeArray(attrColors);
    shaderProgram.disableAttributeArray(attrTexCoords);
}


void MyGLWidget::loadModel()
{
    // Lade Model aus Datei:
    ModelLoader model ;
    bool res = model.loadObjectFromFile("P3_models/sphere_low.obj");
    // Wenn erfolgreich, generiere VBO und Index-Array
    if (res) {
        // Frage zu erwartende Array-Längen ab
        //vboLength = model.lengthOfSimpleVBO();
        vboLength = model.lengthOfVBO(0,false,true);
        iboLength = model.lengthOfIndexArray();
        // Generiere VBO und Index-Array
        vboData = new GLfloat[vboLength];
        indexData = new GLuint[iboLength];
        //model.genSimpleVBO(vboData);
        model.genVBO(vboData,0,false,true);  // With textures
        model.genIndexArray(indexData);
    }
    else {
        // Modell konnte nicht geladen werden
        assert(0) ;
    }


}


 void MyGLWidget::createPlantes()
 {


 }




void MyGLWidget::initializeGL()
{

    glEnable(GL_DEPTH_TEST);                                // Activate depth comparisons and update depth buffer

    glEnable(GL_CULL_FACE);                                 // Draw Front or Back?

    glDepthFunc(GL_LEQUAL);                                 // Specify the depth buffer

    //glShadeModel(GL_SMOOTH);                                // !Deprecated GL_FLAT or GL_SMOOTH (interpolated)

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);      // Are there Interpretations? Set hint!

    glClearDepth(1.0f);                                     // Clear value for depth buffer (used by glClear)

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                   // Clear values used by glClear (Color-Buffer)();

    loadModel();
    initializeTextures();
    initalizeBuffer();
    initalizeShader();
    initializePlanets();

    attrVertices  = shaderProgram.attributeLocation("vert");     // #version 130
    attrTexCoords = shaderProgram.attributeLocation("texCoord"); // #version 130


    // Aktiviere die Verwendung der Attribute-Arrays
    shaderProgram.enableAttributeArray(attrVertices);
    shaderProgram.enableAttributeArray(attrTexCoords);


    // Laden der
    // Ein paar Hilfsvariablen - die 8 steht für
    // 4 Eckpunktkoordinaten + 4 Texturkoordinaten
    int offset = 0 ;
    size_t stride = 8 * sizeof(GLfloat);
    shaderProgram.setAttributeBuffer(attrVertices,GL_FLOAT,offset,4,stride);
    offset += 4*sizeof(GLfloat);
    shaderProgram.setAttributeBuffer(attrTexCoords,GL_FLOAT,offset,4,stride);


    // Lokalisiere bzw. definierte die Schnittstelle für die Matritzen
    // Die Matrix kann direkt übergeben werden, da setUniformValue für diesen Typ überladen ist.
    unifMatrixPerspective = shaderProgram.uniformLocation("perspectiveMatrix");
    Q_ASSERT(unifMatrixPerspective >= 0) ;
    unifMatrixModel = shaderProgram.uniformLocation("modelMatrix");
    Q_ASSERT(unifMatrixModel >= 0) ;
    unifMatrixView = shaderProgram.uniformLocation("viewlMatrix");
    Q_ASSERT(unifMatrixView >= 0) ;

    //qTex->bind();

}


void MyGLWidget::resizeGL(int width, int height)
{
   // Compute aspect ratio
   height = (height == 0) ? 1 : height;  // ?

   // Set viewport to cover the whole window
   glViewport(0, 0, width, height);


   // PROJECTION (neues OpenGL)
   projectionMatrix.setToIdentity();
   projectionMatrix.frustum(-0.05, 0.05, -0.05, 0.05, 0.1, 1000.0);
}


void MyGLWidget::paintGL()
{

    // Clear buffer to set color and alpha
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);







    shaderProgram.setUniformValue(unifMatrixPerspective,projectionMatrix);






    // VIEW TRANSFORMATION
    QMatrix4x4 viewMatrix ;
    viewMatrix.lookAt(cameraPos,cameraPos+cameraFront,cameraUp);
    shaderProgram.setUniformValue(unifMatrixView,viewMatrix);

    // MODEL TRANSFORMATION (Neues OpenGL)
    QMatrix4x4 modelMatrix ;
    // Initialisierung des Modells
    modelMatrix.setToIdentity();
    modelMatrixStack.push(modelMatrix);


    // Zeit zwischen den Render Bildern
    elapsedTime = tmrRender.elapsed();
    //qDebug() << elapsedTime ;
    tmrRender.start();


    // glBindTexture(GL_TEXTURE_2D,tList[sun]);
    //qTex->bind();
    textures[1]->bind();
    textures[1]->bind();


    // Übergebe die Textur an die Uniform Variable
    // Die 0 steht dabei für die verwendete Unit (0=Standard)
    //shaderProgram.setUniformValue("texture",0);



    // Triggern des Renderns
    sonne.render();

    // Stack wieder säubern.
    modelMatrixStack.pop();

    //qTex->release();



}

void MyGLWidget::initalizeBuffer()
{
    // Erzeuge vbo
    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.allocate(vboData,sizeof(GLfloat) * vboLength);
    vbo.release();

    // Erzeuge Index-Buffer
    ibo.create();
    ibo.bind();
    ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    ibo.allocate(indexData,sizeof(GLuint) * iboLength);
    ibo.release();

    vbo.bind();
    ibo.bind();

}

void MyGLWidget::initalizeShader()
{


    // Initialisierung Shader
    // Lade Shader-Source aus externen Dateien
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                          ":/default130.vert") ;

    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                          ":/default130.frag") ;
    // Kompiliere und linke die Shader-Programme
    shaderProgram.link() ;

    // Binde das Shader-Programm an den OpenGL-Kontext
    shaderProgram.bind();

}

void MyGLWidget::initializeTextures()
{

    // Initialization
    // Es scheint, dass man die QOpenQLTexture in dieser Klasse verwenden muss
    // und die Übergabe an den Shader autmoatisch beim Erzeugen erfolgt.
    // Aus anderen Klassen kann man dann die entsprechende QOpenGLTexture dann binden.

    qTex = new QOpenGLTexture(QImage(":/Maps/sunmap.jpg").mirrored()) ;
    qTex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    qTex->setMagnificationFilter(QOpenGLTexture::Linear);

    textures[texSonne] = new QOpenGLTexture(QImage(":/Maps/sunmap.jpg").mirrored()) ;
    textures[texMerkur] = new QOpenGLTexture(QImage(":/Maps/mercurymap.jpg").mirrored()) ;
    textures[texVenus] = new QOpenGLTexture(QImage(":/Maps/venusmap.jpg").mirrored()) ;
    textures[texErde] = new QOpenGLTexture(QImage(":/Maps/earthmap1k.jpg").mirrored()) ;
    textures[texErdeMond] = new QOpenGLTexture(QImage(":/Maps/mond.jpg").mirrored()) ;
    textures[texMars] = new QOpenGLTexture(QImage(":/Maps/marsmap1k.jpg").mirrored()) ;
    textures[texPhobos] = new QOpenGLTexture(QImage(":/Maps/phobos.jpg").mirrored()) ;
    textures[texDeimos] = new QOpenGLTexture(QImage(":/Maps/deimos.jpg").mirrored()) ;
    textures[texJupiter] = new QOpenGLTexture(QImage(":/Maps/jupitermap.jpg").mirrored()) ;
    textures[texSaturn] = new QOpenGLTexture(QImage(":/Maps/saturnmap.jpg").mirrored()) ;
    textures[texUranus] = new QOpenGLTexture(QImage(":/Maps/uranusmap.jpg").mirrored()) ;
    textures[texNeptun] = new QOpenGLTexture(QImage(":/Maps/neptunemap.jpg").mirrored()) ;


    for ( int i=0 ; i < 12 ; i++) {
        textures[i]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        textures[i]->setMagnificationFilter(QOpenGLTexture::Linear);
    }

  //Q_ASSERT( qTex->textureId() == 0 ) ;


}

void MyGLWidget::initializePlanets()
{
    // Logische Anordnung der Planeten
    sonne   .setPlanetParameter(&shaderProgram, &unifMatrixModel, &modelMatrixStack, &iboLength, textures[texSonne]    , &elapsedTime, &paused, 0    , 0   , 0.01 , 1) ;

    merkur  .setPlanetParameter(&shaderProgram, &unifMatrixModel, &modelMatrixStack, &iboLength, textures[texMerkur]   , &elapsedTime, &paused, 10   , 0.05   , 0.005  , 0.07) ;
    venus   .setPlanetParameter(&shaderProgram, &unifMatrixModel, &modelMatrixStack, &iboLength, textures[texVenus]    , &elapsedTime, &paused, 14   , 0.04   , 0.001  , 0.1) ;

    erde    .setPlanetParameter(&shaderProgram, &unifMatrixModel, &modelMatrixStack, &iboLength, textures[texErde]     , &elapsedTime, &paused, 18   , 0.03    , 0.1  , 0.1) ;
    erdemond.setPlanetParameter(&shaderProgram, &unifMatrixModel, &modelMatrixStack, &iboLength, textures[texErdeMond] , &elapsedTime, &paused, 1    , 0.08    , 0.01  , 0.06) ;

    mars    .setPlanetParameter(&shaderProgram, &unifMatrixModel, &modelMatrixStack, &iboLength, textures[texMars]     , &elapsedTime, &paused, 25   , 0.01    , 0.1  , 0.08) ;
    phobos  .setPlanetParameter(&shaderProgram, &unifMatrixModel, &modelMatrixStack, &iboLength, textures[texPhobos]   , &elapsedTime, &paused, 1    , 0.15    , 0.2  , 0.03) ;
    deimos  .setPlanetParameter(&shaderProgram, &unifMatrixModel, &modelMatrixStack, &iboLength, textures[texDeimos]   , &elapsedTime, &paused, 2    , 0.1    , 0.09  , 0.03) ;

    jupiter .setPlanetParameter(&shaderProgram, &unifMatrixModel, &modelMatrixStack, &iboLength, textures[texJupiter]  , &elapsedTime, &paused, 40   , 0.001     , 0.15  , 0.3) ;
    saturn  .setPlanetParameter(&shaderProgram, &unifMatrixModel, &modelMatrixStack, &iboLength, textures[texSaturn]   , &elapsedTime, &paused, 50   , 0.0007     , 0.15  , 0.25) ;
    uranus  .setPlanetParameter(&shaderProgram, &unifMatrixModel, &modelMatrixStack, &iboLength, textures[texUranus]   , &elapsedTime, &paused, 60   , 0.0002     , 0.12  , 0.15) ;
    neptun  .setPlanetParameter(&shaderProgram, &unifMatrixModel, &modelMatrixStack, &iboLength, textures[texNeptun]   , &elapsedTime, &paused, 70   , 0.00008     , 0.12  , 0.15) ;

    sonne.addSubPlanet(&merkur);
    sonne.addSubPlanet(&venus);
    sonne.addSubPlanet(&erde);
    erde.addSubPlanet(&erdemond);

    sonne.addSubPlanet(&mars);
    mars.addSubPlanet(&phobos);
    mars.addSubPlanet(&deimos);

    sonne.addSubPlanet(&jupiter);
    sonne.addSubPlanet(&saturn);
    sonne.addSubPlanet(&uranus);
    sonne.addSubPlanet(&neptun);



}



void MyGLWidget::wheelEvent ( QWheelEvent * event )
{
    zoom += event->delta() / 30 ;
    emit zoomFactorChanged(zoom);
    glDraw() ;                      // !Deprecated
}


void MyGLWidget::keyPressEvent(QKeyEvent *event)
{

    GLfloat   cameraSpeed = 1.0f ;
    QVector3D cross ;

    bool      changedFront = false ;
    switch ( event->key()) {
        case Qt::Key_W     : cameraPos += cameraSpeed * cameraFront ;
             break ;
        case Qt:: Key_S    : cameraPos -= cameraSpeed * cameraFront ;
             break ;
        case Qt::Key_A     : cross = cross.crossProduct(cameraFront , cameraUp) ;
                             cross.normalize();
                             cameraPos -= cross * cameraSpeed ;
             break ;
        case Qt::Key_D     : cross = cross.crossProduct(cameraFront , cameraUp) ;
                             cross.normalize();
                             cameraPos += cross * cameraSpeed ;
             break ;
        case Qt::Key_Up    : pitch += 1 ;
                             changedFront = true ;
             break ;
        case Qt::Key_Down  : pitch -= 1 ;
                             changedFront = true ;
             break ;
        case Qt::Key_Left  : yaw -= 1 ;
                             changedFront = true ;
             break ;
        case Qt::Key_Right : yaw += 1 ;
                             changedFront = true ;
             break ;
        case Qt::Key_P     : paused = !paused ;
             break ;
        case Qt::Key_R     : cameraPos = QVector3D(0.0f, 0.0f, 3.0f);
                             cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
                             cameraUp = QVector3D(0.0f, 1.0f, 0.0f);
             break ;
        default : QGLWidget::keyPressEvent(event) ;
    }

    if (changedFront)
    {
    QVector3D front;
        front.setX ( cos(pitch*(M_PI/180)) * cos(yaw*(M_PI/180)) );
        front.setY ( sin(pitch*(M_PI/180) )  );
        front.setZ ( cos(pitch*(M_PI/180)) * sin(yaw*(M_PI/180)) );
        front.normalize();
        cameraFront = front ;
    }

}


void MyGLWidget::receiveRotationZ( int degrees )
{
    zRotation = degrees ;

}


