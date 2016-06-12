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
    , vboMarker3(QOpenGLBuffer::VertexBuffer)
    , iboMarker3(QOpenGLBuffer::IndexBuffer)
    , vboMarker4(QOpenGLBuffer::VertexBuffer)
    , iboMarker4(QOpenGLBuffer::IndexBuffer)
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
    marker3ModelView = arDataPtr->marker3modelView_matrix ;
    marker4ModelView = arDataPtr->marker4modelView_matrix ;

    marker0Detected = arDataPtr->marker0Detected ;
    marker1Detected = arDataPtr->marker1Detected ;
    marker2Detected = arDataPtr->marker2Detected ;
    marker3Detected = arDataPtr->marker3Detected ;
    marker4Detected = arDataPtr->marker4Detected ;
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
          vboData[2] = new GLfloat[vboLength[2]];
          indexData[2] = new GLuint[iboLength[2]];
          model->genVBO(vboData[2],0,false,true);
          model->genIndexArray(indexData[2]);
      }
      else {
          // Modell konnte nicht geladen werden
          assert(0) ;
      }
      delete model ;

      // Sombrero
      model = new ModelLoader() ;
      res = model->loadObjectFromFile("Models/sword.obj");
      // Wenn erfolgreich, generiere VBO und Index-Array
      if (res) {
          // Frage zu erwartende Array-Längen ab
          vboLength[3] = model->lengthOfVBO(0,false,true);
          iboLength[3] = model->lengthOfIndexArray();
          // Generiere VBO und Index-Array
          vboData[3] = new GLfloat[vboLength[3]];
          indexData[3] = new GLuint[iboLength[3]];
          model->genVBO(vboData[3],0,false,true);
          model->genIndexArray(indexData[3]);
      }
      else {
          // Modell konnte nicht geladen werden
          assert(0) ;
      }
      delete model ;

      // Sombrero
      model = new ModelLoader() ;
     //res = model->loadObjectFromFile("Models/vikinghelmet.obj");
      res = model->loadObjectFromFile("Models/Porygon.obj");
      // Wenn erfolgreich, generiere VBO und Index-Array
      if (res) {
          // Frage zu erwartende Array-Längen ab
          vboLength[4] = model->lengthOfVBO(0,false,true);
          iboLength[4] = model->lengthOfIndexArray();
          // Generiere VBO und Index-Array
          vboData[4] = new GLfloat[vboLength[4]];
          indexData[4] = new GLuint[iboLength[4]];
          model->genVBO(vboData[4],0,false,true);
          model->genIndexArray(indexData[4]);
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

    texMarker3 = new QOpenGLTexture(QImage(":/sword.jpg").mirrored()) ;
    texMarker3->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texMarker3->setMagnificationFilter(QOpenGLTexture::Linear);
    //Q_ASSERT( texMarker0->textureId() == 0 ) ;

   // texMarker4 = new QOpenGLTexture(QImage(":/viking.jpg").mirrored()) ;
     texMarker4 = new QOpenGLTexture(QImage(":/porygon.jpg").mirrored()) ;
    texMarker4->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texMarker4->setMagnificationFilter(QOpenGLTexture::Linear);
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

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                   // Clear values used by glClear (Color-Buffer)

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
                //modelViewMatrix.rotate(90,1,0,0);
                if (marker0Scale < 0.005f) marker0Scale += 0.0001 ;
                modelViewMatrix.scale(marker0Scale);
                //modelViewMatrix.scale(0.005f);

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
        else
        {
            marker0Scale = 0 ;
        }

        // --- Marker 1 ---
        if ( marker1Detected && drawAR && projectionCalculated ) {

            // MODELVIEW TRANSFORMATION (Neues OpenGL)
            QMatrix4x4 modelViewMatrix = QMatrix4x4(  marker1ModelView.at<double>(0,0) , marker1ModelView.at<double>(0,1) , marker1ModelView.at<double>(0,2) , marker1ModelView.at<double>(0,3) ,
                                                      marker1ModelView.at<double>(1,0) , marker1ModelView.at<double>(1,1) , marker1ModelView.at<double>(1,2) , marker1ModelView.at<double>(1,3) ,
                                                      marker1ModelView.at<double>(2,0) , marker1ModelView.at<double>(2,1) , marker1ModelView.at<double>(2,2) , marker1ModelView.at<double>(2,3) ,
                                                      marker1ModelView.at<double>(3,0) , marker1ModelView.at<double>(3,1) , marker1ModelView.at<double>(3,2) , marker1ModelView.at<double>(3,3)
                                                      );
           // modelViewMatrix.rotate(90,1,0,0);
            if (marker1Scale < 0.1f) marker1Scale += 0.005 ;
            modelViewMatrix.scale(marker1Scale);
            // modelViewMatrix.scale(0.1f);

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
        else
        {
            marker1Scale = 0 ;
        }

        // --- Marker 2 ---
        if ( marker2Detected && drawAR && projectionCalculated ) {
            // MODELVIEW TRANSFORMATION (Neues OpenGL)
            QMatrix4x4 modelViewMatrix = QMatrix4x4(  marker2ModelView.at<double>(0,0) , marker2ModelView.at<double>(0,1) , marker2ModelView.at<double>(0,2) , marker2ModelView.at<double>(0,3) ,
                                                      marker2ModelView.at<double>(1,0) , marker2ModelView.at<double>(1,1) , marker2ModelView.at<double>(1,2) , marker2ModelView.at<double>(1,3) ,
                                                      marker2ModelView.at<double>(2,0) , marker2ModelView.at<double>(2,1) , marker2ModelView.at<double>(2,2) , marker2ModelView.at<double>(2,3) ,
                                                      marker2ModelView.at<double>(3,0) , marker2ModelView.at<double>(3,1) , marker2ModelView.at<double>(3,2) , marker2ModelView.at<double>(3,3)
                                                      );
         //   modelViewMatrix.rotate(90,1,0,0);
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

        // --- Marker 3 ---
        if ( marker3Detected && drawAR && projectionCalculated ) {
            // MODELVIEW TRANSFORMATION (Neues OpenGL)
            QMatrix4x4 modelViewMatrix = QMatrix4x4(  marker3ModelView.at<double>(0,0) , marker3ModelView.at<double>(0,1) , marker3ModelView.at<double>(0,2) , marker3ModelView.at<double>(0,3) ,
                                                      marker3ModelView.at<double>(1,0) , marker3ModelView.at<double>(1,1) , marker3ModelView.at<double>(1,2) , marker3ModelView.at<double>(1,3) ,
                                                      marker3ModelView.at<double>(2,0) , marker3ModelView.at<double>(2,1) , marker3ModelView.at<double>(2,2) , marker3ModelView.at<double>(2,3) ,
                                                      marker3ModelView.at<double>(3,0) , marker3ModelView.at<double>(3,1) , marker3ModelView.at<double>(3,2) , marker3ModelView.at<double>(3,3)
                                                      );
           // modelViewMatrix.rotate(90,1,0,0);
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

            vboMarker3.bind();
            iboMarker3.bind();

            shaderProgram.setUniformValue(unifMatrix,modelViewProjectionMatrix);

            texMarker3->bind();
            shaderProgram.setUniformValue("texture",0);

            // Fülle die Attribute-Buffer mit den konkreten Daten
            int offset = 0 ;
            int stride = 8 * sizeof(GLfloat) ;
            shaderProgram.setAttributeBuffer(attrVertices,GL_FLOAT,offset,4,stride);
            offset += 4*sizeof(GLfloat);
            shaderProgram.setAttributeBuffer(attrTexCoords,GL_FLOAT,offset,4,stride);

            glDrawElements ( GL_TRIANGLES,                      // Primitive
                             iboLength[3],                         // Wieviele Indizies
                             GL_UNSIGNED_INT,                   // Datentyp
                             0);                                // 0 = Nehme den Index Buffer
            // Deaktiviere die Verwendung der Attribute Arrays
            shaderProgram.disableAttributeArray(attrVertices);
            shaderProgram.disableAttributeArray(attrTexCoords);

            vboMarker3.release();
            iboMarker3.release();
            texMarker3->release();

        } // if ( marker2Detected && drawAR && projectionCalculated )

        // --- Marker 4 ---
        if ( marker4Detected && drawAR && projectionCalculated ) {
            // MODELVIEW TRANSFORMATION (Neues OpenGL)
            QMatrix4x4 modelViewMatrix = QMatrix4x4(  marker4ModelView.at<double>(0,0) , marker4ModelView.at<double>(0,1) , marker4ModelView.at<double>(0,2) , marker4ModelView.at<double>(0,3) ,
                                                      marker4ModelView.at<double>(1,0) , marker4ModelView.at<double>(1,1) , marker4ModelView.at<double>(1,2) , marker4ModelView.at<double>(1,3) ,
                                                      marker4ModelView.at<double>(2,0) , marker4ModelView.at<double>(2,1) , marker4ModelView.at<double>(2,2) , marker4ModelView.at<double>(2,3) ,
                                                      marker4ModelView.at<double>(3,0) , marker4ModelView.at<double>(3,1) , marker4ModelView.at<double>(3,2) , marker4ModelView.at<double>(3,3)
                                                      );
          //  modelViewMatrix.rotate(90,1,0,0);
            //modelViewMatrix.scale(0.1f);
            modelViewMatrix.scale(0.001f);

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

            vboMarker4.bind();
            iboMarker4.bind();

            shaderProgram.setUniformValue(unifMatrix,modelViewProjectionMatrix);

            texMarker4->bind();
            shaderProgram.setUniformValue("texture",0);

            // Fülle die Attribute-Buffer mit den konkreten Daten
            int offset = 0 ;
            int stride = 8 * sizeof(GLfloat) ;
            shaderProgram.setAttributeBuffer(attrVertices,GL_FLOAT,offset,4,stride);
            offset += 4*sizeof(GLfloat);
            shaderProgram.setAttributeBuffer(attrTexCoords,GL_FLOAT,offset,4,stride);

            glDrawElements ( GL_TRIANGLES,                      // Primitive
                             iboLength[4],                      // Wieviele Indizies
                             GL_UNSIGNED_INT,                   // Datentyp
                             0);                                // 0 = Nehme den Index Buffer
            // Deaktiviere die Verwendung der Attribute Arrays
            shaderProgram.disableAttributeArray(attrVertices);
            shaderProgram.disableAttributeArray(attrTexCoords);

            vboMarker4.release();
            iboMarker4.release();
            texMarker4->release();

        } // if ( marker2Detected && drawAR && projectionCalculated )

    } // detectorInitialized

    shaderProgram.release();

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


    // Erzeuge vbo
    vboMarker3.create();
    vboMarker3.bind();
    vboMarker3.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboMarker3.allocate(vboData[3],sizeof(GLfloat) * vboLength[3]);
    vboMarker3.release();
    //vboArray[1] = vbo ; // VBO in das Array kopieren

    // Erzeuge Index-Buffer
    iboMarker3.create();
    iboMarker3.bind();
    iboMarker3.setUsagePattern(QOpenGLBuffer::StaticDraw);
    iboMarker3.allocate(indexData[3],sizeof(GLuint) * iboLength[3]);
    iboMarker3.release();
    //iboArray[1] = vbo ;


    // Erzeuge vbo
    vboMarker4.create();
    vboMarker4.bind();
    vboMarker4.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboMarker4.allocate(vboData[4],sizeof(GLfloat) * vboLength[4]);
    vboMarker4.release();
    //vboArray[1] = vbo ; // VBO in das Array kopieren

    // Erzeuge Index-Buffer
    iboMarker4.create();
    iboMarker4.bind();
    iboMarker4.setUsagePattern(QOpenGLBuffer::StaticDraw);
    iboMarker4.allocate(indexData[4],sizeof(GLuint) * iboLength[4]);
    iboMarker4.release();
    //iboArray[1] = vbo ;
}

//---------------------------------------------------------------------------------------------------------

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
