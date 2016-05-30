#include "MyGLWidget.h"


//---------------------------------------------------------------------------------------------------------
MyGLWidget::MyGLWidget()
{

}

//---------------------------------------------------------------------------------------------------------

MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(parent)
    , vbo(QOpenGLBuffer::VertexBuffer)
    , ibo(QOpenGLBuffer::IndexBuffer)
{

    connect(&timer, SIGNAL(timeout()),this,SLOT(updateGL()));
    timer.start(40);
}

//---------------------------------------------------------------------------------------------------------

void MyGLWidget::fetchArData() {

    arDataPtr->mutex.lock();
    cameraMatrix = arDataPtr->cameraMatrix ;
    modelView_matrix = arDataPtr->modelView_matrix ;
    drawAR = arDataPtr->drawAR ;
    tex = arDataPtr->tex ;
    detectorInitialized =  arDataPtr->detectorInitialized ;
    arDataPtr->mutex.unlock();
}

//---------------------------------------------------------------------------------------------------------

void MyGLWidget::loadModel()
{
    // Lade Model aus Datei:
      ModelLoader model ;
      bool res = model.loadObjectFromFile("Models/baby.obj");
      // Wenn erfolgreich, generiere VBO und Index-Array
      if (res) {
          // Frage zu erwartende Array-Längen ab
          vboLength = model.lengthOfSimpleVBO();
          iboLength = model.lengthOfIndexArray();
          // Generiere VBO und Index-Array
          vboData = new GLfloat[vboLength];
          indexData = new GLuint[iboLength];
          model.genSimpleVBO(vboData);
          model.genIndexArray(indexData);
      }
      else {
          // Modell konnte nicht geladen werden
          assert(0) ;
      }
}


//---------------------------------------------------------------------------------------------------------

void MyGLWidget::initializeGL()
{
    // Enable GL textures
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    //glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    setFixedHeight(480);
    setFixedWidth(640);

    //glEnable(GL_CULL_FACE);   // Draw Front or Back?

    glClearColor(1.0, 0.0, 0.0, 0.0) ;
    glClearDepth(1.0) ;
    glDepthFunc(GL_LESS) ;
    glEnable(GL_DEPTH_TEST) ;
    glShadeModel(GL_SMOOTH) ;

    glEnable(GL_TEXTURE_2D); // Enable texturing
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glDisable(GL_TEXTURE_2D);


//    glEnable(GL_DEPTH_TEST);                                // Activate depth comparisons and update depth buffer

//    glEnable(GL_CULL_FACE);                                 // Draw Front or Back?

//    glDepthFunc(GL_LEQUAL);                                 // Specify the depth buffer

    //glShadeModel(GL_SMOOTH);                                // !Deprecated GL_FLAT or GL_SMOOTH (interpolated)

//    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);      // Are there Interpretations? Set hint!

//    glClearDepth(1.0f);                                     // Clear value for depth buffer (used by glClear)

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                   // Clear values used by glClear (Color-Buffer)



    //fillBuffer();
    loadModel();
    initalizeBuffer();
    initalizeShader();
}


//---------------------------------------------------------------------------------------------------------

void MyGLWidget::resizeGL(int width, int height)
{
       // Set viewport to cover the whole window
       glViewport(0, 0, 640, 480);
}



//---------------------------------------------------------------------------------------------------------

void MyGLWidget::paintGL()
{

    // Clear buffer to set color and alpha
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);










    fetchArData();

    if ( detectorInitialized ) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity() ;
        glOrtho(-4, 4, -3, 3, 0.1, 100);

      // -- Verarbeitung des OpenCV-Bildes
        glEnable(GL_TEXTURE_2D); // Enable texturing
        tex = QGLWidget::convertToGLFormat(tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
        glDisable(GL_TEXTURE_2D);

      // -- OpenCV-Bild (Background) zeichnen

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glEnable(GL_TEXTURE_2D);

        glPushMatrix();
        glTranslatef(0.0,0.0,-99) ;

        glBegin(GL_QUADS);
            glTexCoord2f(0,0); glVertex3f(-4, -3, -0.0);
            glTexCoord2f(1,0); glVertex3f( 4, -3, -0.0);
            glTexCoord2f(1,1); glVertex3f( 4,  3, -0.0);
            glTexCoord2f(0,1); glVertex3f(-4,  3, -0.0);
        glEnd();

        glPopMatrix();

        glDisable(GL_TEXTURE_2D);



        // Vllt Fehler wenn nil zugriff!! Deshalb Status Variablen
        if ( drawAR && projectionCalculated ) {


                // MODELVIEW TRANSFORMATION (Neues OpenGL)
                QMatrix4x4 modelViewMatrix = QMatrix4x4(  modelView_matrix.at<double>(0,0) , modelView_matrix.at<double>(0,1) , modelView_matrix.at<double>(0,2) , modelView_matrix.at<double>(0,3) ,
                                                          modelView_matrix.at<double>(1,0) , modelView_matrix.at<double>(1,1) , modelView_matrix.at<double>(1,2) , modelView_matrix.at<double>(1,3) ,
                                                          modelView_matrix.at<double>(2,0) , modelView_matrix.at<double>(2,1) , modelView_matrix.at<double>(2,2) , modelView_matrix.at<double>(2,3) ,
                                                          modelView_matrix.at<double>(3,0) , modelView_matrix.at<double>(3,1) , modelView_matrix.at<double>(3,2) , modelView_matrix.at<double>(3,3)
                                                          );
                modelViewMatrix.rotate(90,1,0,0);
                modelViewMatrix.scale(0.005f);


                QMatrix4x4 modelViewProjectionMatrix  = QMatrix4x4(  persp.at<double>(0,0) , persp.at<double>(0,1) , persp.at<double>(0,2) , persp.at<double>(0,3) ,
                                                                     persp.at<double>(1,0) , persp.at<double>(1,1) , persp.at<double>(1,2) , persp.at<double>(1,3) ,
                                                                     persp.at<double>(2,0) , persp.at<double>(2,1) , persp.at<double>(2,2) , persp.at<double>(2,3) ,
                                                                     persp.at<double>(3,0) , persp.at<double>(3,1) , persp.at<double>(3,2) , persp.at<double>(3,3)
                                                                     );

                modelViewProjectionMatrix = modelViewProjectionMatrix * modelViewMatrix ;


                // Binde das Shader-Programm an den OpenGL-Kontext
                shaderProgram.bind();
                vbo.bind();
                ibo.bind();

                // Lokalisiere bzw. definiere die Schnittstelle für die Eckpunkte
                int attrVertices = 0;
                attrVertices = shaderProgram.attributeLocation("vert");  // #version 130

                // Lokalisiere bzw. definiere die Schnittstelle für die Farben
                //int attrColors = 1;
                //attrColors = shaderProgram.attributeLocation("color");  // #version 130

                // Aktiviere die Verwendung der Attribute-Arrays
                shaderProgram.enableAttributeArray(attrVertices);
                //shaderProgram.enableAttributeArray(attrColors);

                // Lokalisiere bzw. definierte die Schnittstelle für die Transformationsmatrix
                // Die Matrix kann direkt übergeben werden, da setUniformValue für diesen Typ überladen ist.
                int unifMatrix = 0 ;
                unifMatrix = shaderProgram.uniformLocation("matrix");
                Q_ASSERT(unifMatrix >= 0) ;
                shaderProgram.setUniformValue(unifMatrix,modelViewProjectionMatrix);

                // Fülle die Attribute-Buffer mit den konkreten Daten
                int offset = 0 ;
                //int stride = 8 * sizeof(GLfloat) ;
                int stride = 4 * sizeof(GLfloat) ;
                shaderProgram.setAttributeBuffer(attrVertices,GL_FLOAT,offset,4,stride);
                //offset += 4 * sizeof(GLfloat);
                //shaderProgram.setAttributeBuffer(attrColors,GL_FLOAT,offset,4,stride);

                glDrawElements ( GL_TRIANGLES,                      // Primitive
                                 iboLength,                         // Wieviele Indizies
                                 GL_UNSIGNED_INT,                   // Datentyp
                                 0);                                // 0 = Nehme den Index Buffer
                // Deaktiviere die Verwendung der Attribute Arrays
                shaderProgram.disableAttributeArray(attrVertices);
                //shaderProgram.disableAttributeArray(attrColors);


                vbo.release();
                ibo.release();
                shaderProgram.release();

            drawAR = false ;
        }

    } // if detectorInitialized


}

//---------------------------------------------------------------------------------------------------------

void MyGLWidget::drawAxis() {

      // X - Axis red
      glBegin(GL_LINES);
      glColor3f(1.0f, 0.0f, 0.0f);
      glVertex3f(-0.05f, 0.0f, 0.0f);
      glVertex3f(0.05f, 0.0f, 0.0f);
      glEnd();
      // Y - Axis green
      glBegin(GL_LINES);
      glColor3f(0.0f, 1.0f, 0.0f);
      glVertex3f(0.0f, -0.05f, 0.0f);
      glVertex3f(0.0f, 0.05f, 0.0f);
      glEnd();
      // Z - Axis blue
      glBegin(GL_LINES);
      glColor3f(0.0f, 0.0f, 1.0f);
      glVertex3f(0.0f, 0.0f, 0.05f);
      glVertex3f(0.0f, 0.0f, -0.05f);
      glEnd();

}

//---------------------------------------------------------------------------------------------------------

void MyGLWidget::loadProjectionMatrix() {

    fetchArData();  // Get the Data: Camera Matrix
    double nearVal = 0.1;
    double farVal = 100.0 ;

    // Beim Einlesen der cameraMatrix aus dem Config-File gehen Kodierungsinfos verloren.
    // Deshalb hier auf Nummer sicher gehen.
    cv::Mat cameraMatrixConverted ;
    cameraMatrix.convertTo(cameraMatrixConverted,CV_32F);



    if(persp.empty()) {
                persp.create(4,4);
                persp.setTo(0);

                // http://kgeorge.github.io/2014/03/08/calculating-opengl-perspective-matrix-from-opencv-intrinsic-matrix/
                qDebug() << cameraMatrixConverted.at<float>(0,0);
                qDebug() << cameraMatrixConverted.at<float>(1,1);
                qDebug() << cameraMatrixConverted.at<float>(0,2);
                qDebug() << cameraMatrixConverted.at<float>(1,2);

                double fx = cameraMatrixConverted.at<float>(0,0);
                double fy = cameraMatrixConverted.at<float>(1,1);
                double cx = cameraMatrixConverted.at<float>(0,2);
                double cy = cameraMatrixConverted.at<float>(1,2);
                persp(0,0) = fx/cx;
                persp(1,1) = fy/cy;
                persp(2,2) = -(farVal+nearVal)/(farVal-nearVal);
                persp(2,3) = -2.0*farVal*nearVal / (farVal-nearVal);
                persp(3,2) = -1.0;

                // Transponieren entfernt, da in QMatrix gespeichert wird und diese dann für OpenGL transponiert.
                //persp = persp.t(); //to col-major

                glMatrixMode(GL_PROJECTION);;
                glLoadMatrixd((double*)persp.data);
                projectionCalculated = true ;

            }

}

//---------------------------------------------------------------------------------------------------------
/*
void MyGLWidget::addVertice(int     verticeNo ,
                            GLfloat x ,
                            GLfloat y ,
                            GLfloat z ,
                            GLfloat r ,
                            GLfloat g ,
                            GLfloat b )
{
    vertices[verticeNo].x = x ;
        vertices[verticeNo].y = y ;
        vertices[verticeNo].z = z ;
        vertices[verticeNo].h = 1 ;
        vertices[verticeNo].r = r ;
        vertices[verticeNo].g = g ;
        vertices[verticeNo].b = b ;
        vertices[verticeNo].t = 1 ;
}
*/
//---------------------------------------------------------------------------------------------------------

void MyGLWidget::initalizeBuffer()
{
    // Erzeuge vbo
    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
//    vbo.allocate( vertices,                                 // Vertex-Array
//                  sizeof(GLfloat) * 8 * verticesCount );    // Speicherbedarf pro Vertex
    vbo.allocate(vboData,sizeof(GLfloat) * vboLength);
    vbo.release();

    // Erzeuge Index-Buffer
    ibo.create();
    ibo.bind();
    ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);
//    ibo.allocate( indicies,                                 // Index-Array
//                  sizeof(GLubyte) * 24 );                   // Speicherbedarf Indizies
    ibo.allocate(indexData,sizeof(GLuint) * iboLength);
    ibo.release();
}

//---------------------------------------------------------------------------------------------------------

void MyGLWidget::fillBuffer()
{
    /*
    // Vertices
    // Back
    addVertice(0 , -0.1f , -0.1f , -0.1f , 1 , 0 , 0 );
    addVertice(1 ,  0.1f , -0.1f , -0.1f , 0 , 1 , 0 );
    addVertice(2 ,  0.1f ,  0.1f , -0.1f , 1 , 0 , 1 );
    addVertice(3 , -0.1f ,  0.1f , -0.1f , 0 , 0 , 1 );
    // Front        0.1     0.1     0.1
    addVertice(4 , -0.1f , -0.1f ,  0.1f , 1 , 0 , 1 );
    addVertice(5 ,  0.1f , -0.1f ,  0.1f , 1 , 1 , 0 );
    addVertice(6 ,  0.1f ,  0.1f ,  0.1f , 0 , 1 , 1 );
    addVertice(7 , -0.1f ,  0.1f ,  0.1f , 1 , 0 , 0 );

    // Indicies - Cube
    // Front
    indicies[0] = 4 ;
    indicies[1] = 5 ;
    indicies[2] = 6 ;
    indicies[3] = 7 ;
    // Back
    indicies[4] = 3 ;
    indicies[5] = 2 ;
    indicies[6] = 1 ;
    indicies[7] = 0 ;
    // Left
    indicies[8] = 7 ;
    indicies[9] = 3 ;
    indicies[10] = 0 ;
    indicies[11] = 4 ;
    // Right
    indicies[12] = 6 ;
    indicies[13] = 5 ;
    indicies[14] = 1 ;
    indicies[15] = 2 ;
    // Top
    indicies[16] = 6 ;
    indicies[17] = 2 ;
    indicies[18] = 3 ;
    indicies[19] = 7 ;
    // Bottom
    indicies[20] = 4 ;
    indicies[21] = 0 ;
    indicies[22] = 1 ;
    indicies[23] = 5 ;
    */
}

//----------------------------------------------------------------------


void MyGLWidget::initalizeShader()
{


    // Initialisierung Shader
    // Lade Shader-Source aus externen Dateien
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                          ":/default330.vert") ;

    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                          ":/default330.frag") ;
    // Kompiliere und linke die Shader-Programme
    shaderProgram.link() ;


}
