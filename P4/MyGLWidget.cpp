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

     setFocusPolicy(Qt::StrongFocus);               // To catch the KeyPressEvents.

     QTimer *timer = new QTimer(this) ;
     connect(timer, SIGNAL(timeout()),this,SLOT(updateGL()));
     timer->start(0);

}

MyGLWidget::~MyGLWidget()
{

    shaderProgram[0].release();
    shaderProgram[1].release();

    vbo.release();
    ibo.release();

    // Deaktiviere die Verwendung der Attribute Arrays
    shaderProgram[0].disableAttributeArray(attrVerticesPlanet);
    shaderProgram[0].disableAttributeArray(attrNormalsPlanet);
    shaderProgram[0].disableAttributeArray(attrTexCoordsPlanet);
    shaderProgram[1].disableAttributeArray(attrVerticesSun);
    shaderProgram[1].disableAttributeArray(attrNormalsSun);
    shaderProgram[1].disableAttributeArray(attrTexCoordsSun);
}


void MyGLWidget::loadModel()
{
    // Lade Model aus Datei:
    ModelLoader model ;
    bool res = model.loadObjectFromFile("P3_models/sphere_low.obj");
    // Wenn erfolgreich, generiere VBO und Index-Array
    if (res) {
        hasTexureCoord = model.hasTextureCoordinates() ;
        // Frage zu erwartende Array-Längen ab
        if ( hasTexureCoord )
        {
            vboLength = model.lengthOfVBO(0,true,true);
        }
        else
        {
            vboLength = model.lengthOfVBO(0,true,false);
        }

        iboLength = model.lengthOfIndexArray();
        // Generiere VBO und Index-Array
        vboData = new GLfloat[vboLength];
        indexData = new GLuint[iboLength];
        //model.genSimpleVBO(vboData);
        if ( hasTexureCoord )
        {
            model.genVBO(vboData,0,true,true);  // With textures
        }
        else
        {
            model.genVBO(vboData,0,true,false);  // Without textures
        }

        model.genIndexArray(indexData);

    }
    else {
        // Modell konnte nicht geladen werden
        assert(0) ;
    }

}



void MyGLWidget::initializeGL()
{

    glEnable(GL_DEPTH_TEST);                                // Activate depth comparisons and update depth buffer

    glEnable(GL_CULL_FACE);                                 // Draw Front or Back?

    glDepthFunc(GL_LEQUAL);                                 // Specify the depth buffer

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);      // Are there Interpretations? Set hint!

    glClearDepth(1.0f);                                     // Clear value for depth buffer (used by glClear)

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                   // Clear values used by glClear (Color-Buffer)();

    loadModel();
    initializeTextures();
    initalizeBuffer();
    initalizeShader();
    initializePlanets();

    initializeShaderProgramPlanet() ;
    initializeShaderProgramSun();

    // VIEW TRANSFORMATION
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(cameraPos,cameraPos+cameraFront,cameraUp);

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

    time = time + 0.001 ;   // Timer für das Hitzeflimmern


    // Übergabe von Werten an die Shader-Programme
    shaderProgram[0].bind();
    shaderProgram[0].setUniformValue(unifMatrixViewPlanet,viewMatrix);
    shaderProgram[0].setUniformValue(unifMatrixPerspectivePlanet,projectionMatrix);
    shaderProgram[0].setUniformValue(unifViewPositionPlanet,cameraPos);

    shaderProgram[1].bind();
    shaderProgram[1].setUniformValue(unifMatrixViewSun,viewMatrix);
    shaderProgram[1].setUniformValue(unifMatrixPerspectiveSun,projectionMatrix);
    shaderProgram[1].setUniformValue(unifTimerSun,time);

    // Initialisierung des Modells
    modelMatrix.setToIdentity();
    modelMatrixStack.push(modelMatrix);

    // Zeit zwischen den Render Bildern
    elapsedTime = tmrRender.elapsed();
    tmrRender.start();

    // Triggern des Renderns
    sonne.render();

    // Stack wieder säubern.
    modelMatrixStack.pop();

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


    // Initialisierung Shader - Mit Texturen
    // Lade Shader-Source aus externen Dateien
    shaderProgram[0].addShaderFromSourceFile(QOpenGLShader::Vertex,
                                          ":/default130.vert") ;

    shaderProgram[0].addShaderFromSourceFile(QOpenGLShader::Fragment,
                                          ":/default130.frag") ;
    // Kompiliere und linke die Shader-Programme
    shaderProgram[0].link() ;

    // Binde das Shader-Programm an den OpenGL-Kontext
    shaderProgram[0].bind();


    // Initialisierung Shader - Sonnenflimmern
    // Lade Shader-Source aus externen Dateien
    shaderProgram[1].addShaderFromSourceFile(QOpenGLShader::Vertex,
                                          ":/sun130.vert") ;

    shaderProgram[1].addShaderFromSourceFile(QOpenGLShader::Fragment,
                                          ":/sun130.frag") ;
    shaderProgram[1].link() ;
    shaderProgram[1].bind();

}

void MyGLWidget::initializeShaderProgramPlanet()
{
    shaderProgram[0].bind();
    attrVerticesPlanet = shaderProgram[0].attributeLocation("vert");
    attrNormalsPlanet  = shaderProgram[0].attributeLocation("vertexNormal") ;
    if ( hasTexureCoord )
    {
        attrTexCoordsPlanet = shaderProgram[0].attributeLocation("texCoord");
    }

    // Aktiviere die Verwendung der Attribute-Arrays
    shaderProgram[0].enableAttributeArray(attrVerticesPlanet);
    shaderProgram[0].enableAttributeArray(attrNormalsPlanet);
    if ( hasTexureCoord )
    {
    shaderProgram[0].enableAttributeArray(attrTexCoordsPlanet);
    }

    // Laden der
    // Ein paar Hilfsvariablen - die 8 steht für
    // 4 Eckpunktkoordinaten + 4 Texturkoordinaten
    int offset = 0 ;
    size_t stride = 8 * sizeof(GLfloat);    // Annahme ohne Texturen
    if (hasTexureCoord)
    {
        stride = 12 * sizeof(GLfloat);       // .. Doch mit Texturen ;)
    }
    shaderProgram[0].setAttributeBuffer(attrVerticesPlanet,GL_FLOAT,offset,4,stride);        // Vertices

    offset += 4*sizeof(GLfloat);
    shaderProgram[0].setAttributeBuffer(attrNormalsPlanet,GL_FLOAT,offset,4,stride);         // Normals

    if (hasTexureCoord)
    {
        offset += 4*sizeof(GLfloat);
        shaderProgram[0].setAttributeBuffer(attrTexCoordsPlanet,GL_FLOAT,offset,4,stride);   // Texture
    }


    // Lokalisiere bzw. definierte die Schnittstelle für die Matritzen
    // Die Matrix kann direkt übergeben werden, da setUniformValue für diesen Typ überladen ist.
    unifMatrixPerspectivePlanet = shaderProgram[0].uniformLocation("perspectiveMatrix");
    unifMatrixModelDefault      = shaderProgram[0].uniformLocation("modelMatrix");
    unifMatrixViewPlanet        = shaderProgram[0].uniformLocation("viewlMatrix");
    unifkaPlanet                = shaderProgram[0].uniformLocation("ka");
    unifkdPlanet                = shaderProgram[0].uniformLocation("kd");
    unifksPlanet                = shaderProgram[0].uniformLocation("ks");
    unifNormalMatrixPlanet      = shaderProgram[0].uniformLocation("normalMatrix");
    unifLightPositionPlanet     = shaderProgram[0].uniformLocation("lightPosition");
    unifViewPositionPlanet      = shaderProgram[0].uniformLocation("viewPosition");

    QVector4D lightPos = QVector4D(0.0, 0.0, 0.0, 0.0);
    shaderProgram[0].setUniformValue(unifLightPositionPlanet,lightPos);

    shaderProgram[0].setUniformValue(unifkaPlanet,ka);
    shaderProgram[0].setUniformValue(unifkdPlanet,kd);
    shaderProgram[0].setUniformValue(unifksPlanet,ks);
}


void MyGLWidget::initializeShaderProgramSun()
{
    shaderProgram[1].bind();
    attrVerticesSun = shaderProgram[1].attributeLocation("vert");     // #version 130
    attrNormalsSun  = shaderProgram[1].attributeLocation("normale") ;
    if ( hasTexureCoord )
    {
        attrTexCoordsSun = shaderProgram[1].attributeLocation("texCoord"); // #version 130
    }

    // Aktiviere die Verwendung der Attribute-Arrays
    shaderProgram[1].enableAttributeArray(attrVerticesSun);
    shaderProgram[1].enableAttributeArray(attrNormalsSun);
    if ( hasTexureCoord )
    {
        shaderProgram[1].enableAttributeArray(attrTexCoordsSun);
    }


    int offset = 0 ;
    size_t stride = 8 * sizeof(GLfloat);    // Annahme ohne Texturen
    if (hasTexureCoord)
    {
        stride = 12 * sizeof(GLfloat);       // .. Doch mit Texturen ;)
    }

    // Vertices
    shaderProgram[1].setAttributeBuffer(attrVerticesSun,GL_FLOAT,offset,4,stride);

    // Normals
    offset += 4*sizeof(GLfloat);
    shaderProgram[1].setAttributeBuffer(attrNormalsSun,GL_FLOAT,offset,4,stride);

    // Texture
    if (hasTexureCoord)
    {
        offset += 4*sizeof(GLfloat);
        shaderProgram[1].setAttributeBuffer(attrTexCoordsSun,GL_FLOAT,offset,4,stride);
    }


    unifMatrixPerspectiveSun = shaderProgram[1].uniformLocation("perspectiveMatrix");
    unifMatrixModelNormalen  = shaderProgram[1].uniformLocation("modelMatrix");
    unifMatrixViewSun        = shaderProgram[1].uniformLocation("viewlMatrix");
    unifTimerSun             = shaderProgram[1].uniformLocation("t");

}


void MyGLWidget::initializeTextures()
{

    // Initialization
    // Es scheint, dass man die QOpenQLTexture in dieser Klasse verwenden muss
    // und die Übergabe an den Shader automoatisch beim Erzeugen erfolgt.
    // Aus anderen Klassen kann man dann die entsprechende QOpenGLTexture dann binden.

    textures[texSonne]     = new QOpenGLTexture(QImage(":/Maps/sunmap.jpg").mirrored()) ;
    textures[texMerkur]    = new QOpenGLTexture(QImage(":/Maps/mercurymap.jpg").mirrored()) ;
    textures[texVenus]     = new QOpenGLTexture(QImage(":/Maps/venusmap.jpg").mirrored()) ;
    textures[texErde]      = new QOpenGLTexture(QImage(":/Maps/earthmap1k.jpg").mirrored()) ;
    textures[texErdeMond]  = new QOpenGLTexture(QImage(":/Maps/mond.jpg").mirrored()) ;
    textures[texMars]      = new QOpenGLTexture(QImage(":/Maps/marsmap1k.jpg").mirrored()) ;
    textures[texPhobos]    = new QOpenGLTexture(QImage(":/Maps/phobos.jpg").mirrored()) ;
    textures[texDeimos]    = new QOpenGLTexture(QImage(":/Maps/deimos.jpg").mirrored()) ;
    textures[texJupiter]   = new QOpenGLTexture(QImage(":/Maps/jupitermap.jpg").mirrored()) ;
    textures[texSaturn]    = new QOpenGLTexture(QImage(":/Maps/saturnmap.jpg").mirrored()) ;
    textures[texUranus]    = new QOpenGLTexture(QImage(":/Maps/uranusmap.jpg").mirrored()) ;
    textures[texNeptun]    = new QOpenGLTexture(QImage(":/Maps/neptunemap.jpg").mirrored()) ;
    textures[texSunSphere] = new QOpenGLTexture(QImage(":/Maps/sunSphere.jpg").mirrored()) ;

    for ( int i=0 ; i < 13 ; i++) {
        textures[i]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        textures[i]->setMagnificationFilter(QOpenGLTexture::Linear);
    }
    textures[texSunSphere]->setWrapMode(QOpenGLTexture::Repeat);
}


void MyGLWidget::initializePlanets()
{
    // Logische Anordnung der Planeten
    sonne   .setPlanetParameter(&shaderProgram[1], &unifMatrixModelNormalen, &modelMatrixStack, &iboLength, textures[texSonne] , textures[texSunSphere]  , &elapsedTime, &paused, 0    , 0    , 0.01 , 1) ;

    merkur  .setPlanetParameter(&shaderProgram[0], &unifMatrixModelDefault, &unifNormalMatrixPlanet, &modelMatrixStack, &iboLength, textures[texMerkur]   , &elapsedTime, &paused, 10   , 0.05    , 0.005 , 0.07) ;
    venus   .setPlanetParameter(&shaderProgram[0], &unifMatrixModelDefault, &unifNormalMatrixPlanet, &modelMatrixStack, &iboLength, textures[texVenus]    , &elapsedTime, &paused, 14   , 0.04    , 0.001 , 0.1) ;

    erde    .setPlanetParameter(&shaderProgram[0], &unifMatrixModelDefault, &unifNormalMatrixPlanet, &modelMatrixStack, &iboLength, textures[texErde]     , &elapsedTime, &paused, 18   , 0.03    , 0.1   , 0.1) ;
    erdemond.setPlanetParameter(&shaderProgram[0], &unifMatrixModelDefault, &unifNormalMatrixPlanet, &modelMatrixStack, &iboLength, textures[texErdeMond] , &elapsedTime, &paused, 1    , 0.08    , 0.01  , 0.06) ;

    mars    .setPlanetParameter(&shaderProgram[0], &unifMatrixModelDefault, &unifNormalMatrixPlanet, &modelMatrixStack, &iboLength, textures[texMars]     , &elapsedTime, &paused, 25   , 0.01    , 0.1   , 0.08) ;
    phobos  .setPlanetParameter(&shaderProgram[0], &unifMatrixModelDefault, &unifNormalMatrixPlanet, &modelMatrixStack, &iboLength, textures[texPhobos]   , &elapsedTime, &paused, 1    , 0.15    , 0.2   , 0.03) ;
    deimos  .setPlanetParameter(&shaderProgram[0], &unifMatrixModelDefault, &unifNormalMatrixPlanet, &modelMatrixStack, &iboLength, textures[texDeimos]   , &elapsedTime, &paused, 2    , 0.1     , 0.09  , 0.03) ;

    jupiter .setPlanetParameter(&shaderProgram[0], &unifMatrixModelDefault, &unifNormalMatrixPlanet, &modelMatrixStack, &iboLength, textures[texJupiter]  , &elapsedTime, &paused, 40   , 0.001   , 0.15  , 0.3) ;
    saturn  .setPlanetParameter(&shaderProgram[0], &unifMatrixModelDefault, &unifNormalMatrixPlanet, &modelMatrixStack, &iboLength, textures[texSaturn]   , &elapsedTime, &paused, 50   , 0.0007  , 0.15  , 0.25) ;
    uranus  .setPlanetParameter(&shaderProgram[0], &unifMatrixModelDefault, &unifNormalMatrixPlanet, &modelMatrixStack, &iboLength, textures[texUranus]   , &elapsedTime, &paused, 60   , 0.0002  , 0.12  , 0.15) ;
    neptun  .setPlanetParameter(&shaderProgram[0], &unifMatrixModelDefault, &unifNormalMatrixPlanet, &modelMatrixStack, &iboLength, textures[texNeptun]   , &elapsedTime, &paused, 70   , 0.00008 , 0.12  , 0.15) ;

    sonne.addSubPlanet(&merkur);
    sonne.addSubPlanet(&venus);
    sonne.addSubPlanet(&erde);
    erde .addSubPlanet(&erdemond);
    sonne.addSubPlanet(&mars);
    mars .addSubPlanet(&phobos);
    mars .addSubPlanet(&deimos);
    sonne.addSubPlanet(&jupiter);
    sonne.addSubPlanet(&saturn);
    sonne.addSubPlanet(&uranus);
    sonne.addSubPlanet(&neptun);

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

    viewMatrix.setToIdentity();
    viewMatrix.lookAt(cameraPos,cameraPos+cameraFront,cameraUp);


}


