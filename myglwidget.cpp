#include "MyGLWidget.h"


//---------------------------------------------------------------------------------------------------------
MyGLWidget::MyGLWidget()
{

}

//---------------------------------------------------------------------------------------------------------

MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(parent)
    , vboMarker0(QOpenGLBuffer::VertexBuffer)
    , iboMarker0(QOpenGLBuffer::IndexBuffer)
    , vboMarker1(QOpenGLBuffer::VertexBuffer)
    , iboMarker1(QOpenGLBuffer::IndexBuffer)
    , vboMarker2(QOpenGLBuffer::VertexBuffer)
    , iboMarker2(QOpenGLBuffer::IndexBuffer)
{

    connect(&timer, SIGNAL(timeout()),this,SLOT(updateGL()));
    timer.start(40);
}

//---------------------------------------------------------------------------------------------------------

void MyGLWidget::fetchArData() {

    arDataPtr->mutex.lock();
    cameraMatrix = arDataPtr->cameraMatrix ;
    marker0ModelView = arDataPtr->marker0modelView_matrix ;
    marker1ModelView = arDataPtr->marker1modelView_matrix ;
    marker2ModelView = arDataPtr->marker2modelView_matrix ;

    marker0Detected = arDataPtr->marker0Detected ;
    marker1Detected = arDataPtr->marker1Detected ;
    marker2Detected = arDataPtr->marker2Detected ;
    drawAR = arDataPtr->drawAR ;
    tex = arDataPtr->tex ;
    detectorInitialized =  arDataPtr->detectorInitialized ;
    arDataPtr->mutex.unlock();
}

//---------------------------------------------------------------------------------------------------------

void MyGLWidget::loadModel()
{
      // Lade die Modelle aus den Dateien in Datenstrukturen
      ModelLoader * model ;
      model = new ModelLoader() ;

      // Frosch
      bool res = model->loadObjectFromFile("Models/baby.obj");
      // Wenn erfolgreich, generiere VBO und Index-Array
      if (res) {
          // Frage zu erwartende Array-Längen ab
          vboLength[0] = model->lengthOfVBO(0,false,true);
          iboLength[0] = model->lengthOfIndexArray();
          // Generiere VBO und Index-Array
          vboData[0] = new GLfloat[vboLength[0]];
          indexData[0] = new GLuint[iboLength[0]];
          //model->genSimpleVBO(vboData[0]);
          model->genVBO(vboData[0],0,false,true);
          model->genIndexArray(indexData[0]);
      }
      else {
          // Modell konnte nicht geladen werden
          assert(0) ;
      }
      delete model ;

      // Planet
      model = new ModelLoader() ;
      //res = model->loadObjectFromFile("Models/sombrero.obj");
      res = model->loadObjectFromFile("Models/sphere_low.obj");
      // Wenn erfolgreich, generiere VBO und Index-Array
      if (res) {
          // Frage zu erwartende Array-Längen ab
          vboLength[1] = model->lengthOfVBO(0,false,true);
          iboLength[1] = model->lengthOfIndexArray();
          // Generiere VBO und Index-Array
          vboData[1] = new GLfloat[vboLength[1]];
          indexData[1] = new GLuint[iboLength[1]];
          model->genVBO(vboData[1],0,false,true);
          model->genIndexArray(indexData[1]);
      }
      else {
          // Modell konnte nicht geladen werden
          assert(0) ;
      }
      delete model ;

      // Sombrero
      model = new ModelLoader() ;
      res = model->loadObjectFromFile("Models/sombrero.obj");
      // Wenn erfolgreich, generiere VBO und Index-Array
      if (res) {
          // Frage zu erwartende Array-Längen ab
          vboLength[2] = model->lengthOfVBO(0,false,true);
          iboLength[2] = model->lengthOfIndexArray();
          // Generiere VBO und Index-Array
          vboData[2] = new GLfloat[vboLength[1]];
          indexData[2] = new GLuint[iboLength[1]];
          model->genVBO(vboData[2],0,false,true);
          model->genIndexArray(indexData[2]);
      }
      else {
          // Modell konnte nicht geladen werden
          assert(0) ;
      }
      delete model ;
}

//---------------------------------------------------------------------------------------------------------

void MyGLWidget::initializeTextures() {


    // Initialization
    glEnable(GL_TEXTURE_2D);

    texMarker0 = new QOpenGLTexture(QImage(":/frog.jpg").mirrored()) ;
    texMarker0->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texMarker0->setMagnificationFilter(QOpenGLTexture::Linear);
    //Q_ASSERT( texMarker0->textureId() == 0 ) ;

    texMarker1 = new QOpenGLTexture(QImage(":/mercurymap.jpg").mirrored()) ;
    texMarker1->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texMarker1->setMagnificationFilter(QOpenGLTexture::Linear);
    //Q_ASSERT( texMarker0->textureId() == 0 ) ;

    texMarker2 = new QOpenGLTexture(QImage(":/sombrero.jpg").mirrored()) ;
    texMarker2->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texMarker2->setMagnificationFilter(QOpenGLTexture::Linear);
    //Q_ASSERT( texMarker0->textureId() == 0 ) ;

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
    initializeTextures();
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


    // Aktuelle Daten von "OpenCV" holen
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


        // Binde das Shader-Programm an den OpenGL-Kontext
        shaderProgram.bind();

        if ( drawAR ) {
           /*
            // Lokalisiere bzw. definiere die Schnittstelle für die Eckpunkte
            attrVertices = 0;
            attrVertices = shaderProgram.attributeLocation("vert");  // #version 130

            // Aktiviere die Verwendung der Attribute-Arrays
            shaderProgram.enableAttributeArray(attrVertices);

            // Lokalisiere bzw. definierte die Schnittstelle für die Transformationsmatrix
            // Die Matrix kann direkt übergeben werden, da setUniformValue für diesen Typ überladen ist.
            unifMatrix = 0 ;
            unifMatrix = shaderProgram.uniformLocation("matrix");
            */
        }

        // --- Marker 0 ---
        if ( marker0Detected && drawAR && projectionCalculated ) {

                // MODELVIEW TRANSFORMATION (Neues OpenGL)
                QMatrix4x4 modelViewMatrix = QMatrix4x4(  marker0ModelView.at<double>(0,0) , marker0ModelView.at<double>(0,1) , marker0ModelView.at<double>(0,2) , marker0ModelView.at<double>(0,3) ,
                                                          marker0ModelView.at<double>(1,0) , marker0ModelView.at<double>(1,1) , marker0ModelView.at<double>(1,2) , marker0ModelView.at<double>(1,3) ,
                                                          marker0ModelView.at<double>(2,0) , marker0ModelView.at<double>(2,1) , marker0ModelView.at<double>(2,2) , marker0ModelView.at<double>(2,3) ,
                                                          marker0ModelView.at<double>(3,0) , marker0ModelView.at<double>(3,1) , marker0ModelView.at<double>(3,2) , marker0ModelView.at<double>(3,3)
                                                          );
                modelViewMatrix.rotate(90,1,0,0);
                modelViewMatrix.scale(0.005f);

                QMatrix4x4 modelViewProjectionMatrix  = QMatrix4x4(  persp.at<double>(0,0) , persp.at<double>(0,1) , persp.at<double>(0,2) , persp.at<double>(0,3) ,
                                                                     persp.at<double>(1,0) , persp.at<double>(1,1) , persp.at<double>(1,2) , persp.at<double>(1,3) ,
                                                                     persp.at<double>(2,0) , persp.at<double>(2,1) , persp.at<double>(2,2) , persp.at<double>(2,3) ,
                                                                     persp.at<double>(3,0) , persp.at<double>(3,1) , persp.at<double>(3,2) , persp.at<double>(3,3)
                                                                     );

                // Lokalisiere bzw. definiere die Schnittstelle für die Eckpunkte
                attrVertices = 0;
                attrVertices = shaderProgram.attributeLocation("vert");  // #version 130

                int attrTexCoords = 3 ;
                attrTexCoords = shaderProgram.attributeLocation("texCoord"); // #version 130

                // Aktiviere die Verwendung der Attribute-Arrays
                shaderProgram.enableAttributeArray(attrVertices);
                shaderProgram.enableAttributeArray(attrTexCoords);

                // Lokalisiere bzw. definierte die Schnittstelle für die Transformationsmatrix
                // Die Matrix kann direkt übergeben werden, da setUniformValue für diesen Typ überladen ist.
                unifMatrix = 0 ;
                unifMatrix = shaderProgram.uniformLocation("matrix");
                modelViewProjectionMatrix = modelViewProjectionMatrix * modelViewMatrix ;

                vboMarker0.bind();
                iboMarker0.bind();

                shaderProgram.setUniformValue(unifMatrix,modelViewProjectionMatrix);

                texMarker0->bind();
                shaderProgram.setUniformValue("texture",0);


                // Fülle die Attribute-Buffer mit den konkreten Daten
                int offset = 0 ;
                int stride = 8 * sizeof(GLfloat) ;
                shaderProgram.setAttributeBuffer(attrVertices,GL_FLOAT,offset,4,stride);
                offset += 4*sizeof(GLfloat);
                shaderProgram.setAttributeBuffer(attrTexCoords,GL_FLOAT,offset,4,stride);

                glDrawElements ( GL_TRIANGLES,                      // Primitive
                                 iboLength[0],                         // Wieviele Indizies
                                 GL_UNSIGNED_INT,                   // Datentyp
                                 0);                                // 0 = Nehme den Index Buffer
                // Deaktiviere die Verwendung der Attribute Arrays
                shaderProgram.disableAttributeArray(attrVertices);
                shaderProgram.disableAttributeArray(attrTexCoords);

                vboMarker0.release();
                iboMarker0.release();
                texMarker0->release();


        } // if ( marker0Detected && drawAR && projectionCalculated )


        // --- Marker 1 ---
        if ( marker1Detected && drawAR && projectionCalculated ) {

            // MODELVIEW TRANSFORMATION (Neues OpenGL)
            QMatrix4x4 modelViewMatrix = QMatrix4x4(  marker1ModelView.at<double>(0,0) , marker1ModelView.at<double>(0,1) , marker1ModelView.at<double>(0,2) , marker1ModelView.at<double>(0,3) ,
                                                      marker1ModelView.at<double>(1,0) , marker1ModelView.at<double>(1,1) , marker1ModelView.at<double>(1,2) , marker1ModelView.at<double>(1,3) ,
                                                      marker1ModelView.at<double>(2,0) , marker1ModelView.at<double>(2,1) , marker1ModelView.at<double>(2,2) , marker1ModelView.at<double>(2,3) ,
                                                      marker1ModelView.at<double>(3,0) , marker1ModelView.at<double>(3,1) , marker1ModelView.at<double>(3,2) , marker1ModelView.at<double>(3,3)
                                                      );
            modelViewMatrix.rotate(90,1,0,0);
            modelViewMatrix.scale(0.1f);

            QMatrix4x4 modelViewProjectionMatrix  = QMatrix4x4(  persp.at<double>(0,0) , persp.at<double>(0,1) , persp.at<double>(0,2) , persp.at<double>(0,3) ,
                                                                 persp.at<double>(1,0) , persp.at<double>(1,1) , persp.at<double>(1,2) , persp.at<double>(1,3) ,
                                                                 persp.at<double>(2,0) , persp.at<double>(2,1) , persp.at<double>(2,2) , persp.at<double>(2,3) ,
                                                                 persp.at<double>(3,0) , persp.at<double>(3,1) , persp.at<double>(3,2) , persp.at<double>(3,3)
                                                                 );

            // Lokalisiere bzw. definiere die Schnittstelle für die Eckpunkte
            attrVertices = 0;
            attrVertices = shaderProgram.attributeLocation("vert");  // #version 130

            int attrTexCoords = 3 ;
            attrTexCoords = shaderProgram.attributeLocation("texCoord"); // #version 130

            // Aktiviere die Verwendung der Attribute-Arrays
            shaderProgram.enableAttributeArray(attrVertices);
            shaderProgram.enableAttributeArray(attrTexCoords);

            // Lokalisiere bzw. definierte die Schnittstelle für die Transformationsmatrix
            // Die Matrix kann direkt übergeben werden, da setUniformValue für diesen Typ überladen ist.
            unifMatrix = 0 ;
            unifMatrix = shaderProgram.uniformLocation("matrix");
            modelViewProjectionMatrix = modelViewProjectionMatrix * modelViewMatrix ;

            vboMarker1.bind();
            iboMarker1.bind();

            shaderProgram.setUniformValue(unifMatrix,modelViewProjectionMatrix);

            texMarker1->bind();
            shaderProgram.setUniformValue("texture",0);

            // Fülle die Attribute-Buffer mit den konkreten Daten
            int offset = 0 ;
            int stride = 8 * sizeof(GLfloat) ;
            shaderProgram.setAttributeBuffer(attrVertices,GL_FLOAT,offset,4,stride);
            offset += 4*sizeof(GLfloat);
            shaderProgram.setAttributeBuffer(attrTexCoords,GL_FLOAT,offset,4,stride);

            glDrawElements ( GL_TRIANGLES,                      // Primitive
                             iboLength[1],                         // Wieviele Indizies
                             GL_UNSIGNED_INT,                   // Datentyp
                             0);                                // 0 = Nehme den Index Buffer
            // Deaktiviere die Verwendung der Attribute Arrays
            shaderProgram.disableAttributeArray(attrVertices);
            shaderProgram.disableAttributeArray(attrTexCoords);

            vboMarker1.release();
            iboMarker1.release();
            texMarker1->release();

        } // if ( marker1Detected && drawAR && projectionCalculated )


        // --- Marker 2 ---
        if ( marker2Detected && drawAR && projectionCalculated ) {
            qDebug() << marker1Detected ;
            // MODELVIEW TRANSFORMATION (Neues OpenGL)
            QMatrix4x4 modelViewMatrix = QMatrix4x4(  marker2ModelView.at<double>(0,0) , marker2ModelView.at<double>(0,1) , marker2ModelView.at<double>(0,2) , marker2ModelView.at<double>(0,3) ,
                                                      marker2ModelView.at<double>(1,0) , marker2ModelView.at<double>(1,1) , marker2ModelView.at<double>(1,2) , marker2ModelView.at<double>(1,3) ,
                                                      marker2ModelView.at<double>(2,0) , marker2ModelView.at<double>(2,1) , marker2ModelView.at<double>(2,2) , marker2ModelView.at<double>(2,3) ,
                                                      marker2ModelView.at<double>(3,0) , marker2ModelView.at<double>(3,1) , marker2ModelView.at<double>(3,2) , marker2ModelView.at<double>(3,3)
                                                      );
            modelViewMatrix.rotate(90,1,0,0);
            modelViewMatrix.scale(0.1f);

            QMatrix4x4 modelViewProjectionMatrix  = QMatrix4x4(  persp.at<double>(0,0) , persp.at<double>(0,1) , persp.at<double>(0,2) , persp.at<double>(0,3) ,
                                                                 persp.at<double>(1,0) , persp.at<double>(1,1) , persp.at<double>(1,2) , persp.at<double>(1,3) ,
                                                                 persp.at<double>(2,0) , persp.at<double>(2,1) , persp.at<double>(2,2) , persp.at<double>(2,3) ,
                                                                 persp.at<double>(3,0) , persp.at<double>(3,1) , persp.at<double>(3,2) , persp.at<double>(3,3)
                                                                 );

            // Lokalisiere bzw. definiere die Schnittstelle für die Eckpunkte
            attrVertices = 0;
            attrVertices = shaderProgram.attributeLocation("vert");  // #version 130

            int attrTexCoords = 3 ;
            attrTexCoords = shaderProgram.attributeLocation("texCoord"); // #version 130

            // Aktiviere die Verwendung der Attribute-Arrays
            shaderProgram.enableAttributeArray(attrVertices);
            shaderProgram.enableAttributeArray(attrTexCoords);

            // Lokalisiere bzw. definierte die Schnittstelle für die Transformationsmatrix
            // Die Matrix kann direkt übergeben werden, da setUniformValue für diesen Typ überladen ist.
            unifMatrix = 0 ;
            unifMatrix = shaderProgram.uniformLocation("matrix");
            modelViewProjectionMatrix = modelViewProjectionMatrix * modelViewMatrix ;

            vboMarker2.bind();
            iboMarker2.bind();

            shaderProgram.setUniformValue(unifMatrix,modelViewProjectionMatrix);

            texMarker2->bind();
            shaderProgram.setUniformValue("texture",0);

            // Fülle die Attribute-Buffer mit den konkreten Daten
            int offset = 0 ;
            int stride = 8 * sizeof(GLfloat) ;
            shaderProgram.setAttributeBuffer(attrVertices,GL_FLOAT,offset,4,stride);
            offset += 4*sizeof(GLfloat);
            shaderProgram.setAttributeBuffer(attrTexCoords,GL_FLOAT,offset,4,stride);

            glDrawElements ( GL_TRIANGLES,                      // Primitive
                             iboLength[2],                         // Wieviele Indizies
                             GL_UNSIGNED_INT,                   // Datentyp
                             0);                                // 0 = Nehme den Index Buffer
            // Deaktiviere die Verwendung der Attribute Arrays
            shaderProgram.disableAttributeArray(attrVertices);
            shaderProgram.disableAttributeArray(attrTexCoords);

            vboMarker2.release();
            iboMarker2.release();
            texMarker2->release();

        } // if ( marker2Detected && drawAR && projectionCalculated )



    } // detectorInitialized

    shaderProgram.release();

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
    vboMarker0.create();
    vboMarker0.bind();
    vboMarker0.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboMarker0.allocate(vboData[0],sizeof(GLfloat) * vboLength[0]);
    vboMarker0.release();
    //vboArray[0] = vbo ; // VBO in das Array kopieren

    // Erzeuge Index-Buffer
    iboMarker0.create();
    iboMarker0.bind();
    iboMarker0.setUsagePattern(QOpenGLBuffer::StaticDraw);
    iboMarker0.allocate(indexData[0],sizeof(GLuint) * iboLength[0]);
    iboMarker0.release();
    //iboArray[0] = vbo ;


    // Erzeuge vbo
    vboMarker1.create();
    vboMarker1.bind();
    vboMarker1.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboMarker1.allocate(vboData[1],sizeof(GLfloat) * vboLength[1]);
    vboMarker1.release();
    //vboArray[1] = vbo ; // VBO in das Array kopieren

    // Erzeuge Index-Buffer
    iboMarker1.create();
    iboMarker1.bind();
    iboMarker1.setUsagePattern(QOpenGLBuffer::StaticDraw);
    iboMarker1.allocate(indexData[1],sizeof(GLuint) * iboLength[1]);
    iboMarker1.release();
    //iboArray[1] = vbo ;

    // Erzeuge vbo
    vboMarker2.create();
    vboMarker2.bind();
    vboMarker2.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboMarker2.allocate(vboData[2],sizeof(GLfloat) * vboLength[2]);
    vboMarker2.release();
    //vboArray[1] = vbo ; // VBO in das Array kopieren

    // Erzeuge Index-Buffer
    iboMarker2.create();
    iboMarker2.bind();
    iboMarker2.setUsagePattern(QOpenGLBuffer::StaticDraw);
    iboMarker2.allocate(indexData[2],sizeof(GLuint) * iboLength[2]);
    iboMarker2.release();
    //iboArray[1] = vbo ;
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
