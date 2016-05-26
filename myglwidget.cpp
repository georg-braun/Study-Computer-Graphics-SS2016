#include "MyGLWidget.h"


//---------------------------------------------------------------------------------------------------------
MyGLWidget::MyGLWidget()
{

}

//---------------------------------------------------------------------------------------------------------

MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(parent)
{

    connect(timer, SIGNAL(timeout()),this,SLOT(updateGL()));
    //timer->start(40);


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

    loadTexture2("c:\image.jpg", backgroundimage);

    //glEnable(GL_CULL_FACE);   // Draw Front or Back?

    // Python Bsp
    glClearColor(1.0, 0.0, 0.0, 0.0) ;
    glClearDepth(1.0) ;
    glDepthFunc(GL_LESS) ;
    glEnable(GL_DEPTH_TEST) ;
    glShadeModel(GL_SMOOTH) ;
    //( ;
    //glLoadIdentity() ;
    //perspectiveGL(33.7, 1.3, 0.1, 100.0) ; // Später noch mit der CameraMatrix verbessern
   // glOrtho(-4, 4, -3, 3, 0.1, 100);



   // glMatrixMode(GL_MODELVIEW) ;


}

void MyGLWidget::perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar )
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;

    //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fH = tan( fovY / 360 * pi ) * zNear;
    fW = fH * aspect;

    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}

//---------------------------------------------------------------------------------------------------------

void MyGLWidget::resizeGL(int width, int height)
{
        // Compute aspect ratio
       //height = (height == 0) ? 1 : height;  // ?

       // Set viewport to cover the whole window
       glViewport(0, 0, 640, 480);


       //if ( projectionCalculated ) loadProjectionMatrix();
}



//---------------------------------------------------------------------------------------------------------

void MyGLWidget::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity() ;
    glOrtho(-4, 4, -3, 3, 0.1, 100);




  // -- Verarbeitung des OpenCV-Bildes
    glEnable(GL_TEXTURE_2D); // Enable texturing
    glGenTextures(1, &backgroundimage); // Obtain an id for the texture
    glBindTexture(GL_TEXTURE_2D, backgroundimage); // Set as the current texture

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    tex = QGLWidget::convertToGLFormat(tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    glDisable(GL_TEXTURE_2D);

  //------------------------------------------
/*

*/
  // -- OpenCV-Bild (Background) zeichnen

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, backgroundimage);

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







    // Vllt Fehler wenn nil zugriff!!
    if ( drawAR ) {



        glMatrixMode(GL_PROJECTION);
        glLoadIdentity() ;
        glLoadMatrixd((double*)persp.data);


        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadMatrixd((double*)modelView_matrix.data);

        glTranslatef(0.005,0.008,0);

/*        glBegin(GL_QUADS) ;

        glColor3f(0,1,0);
        glVertex3f( -0.05,  -0.05,  0.0);
        glVertex3f( 0.05,  -0.05,  0.0);
        glVertex3f( 0.05,  0.05,  0.0);
        glVertex3f( -0.05,  0.05,  0.0);
*/
        /*
        glBegin(GL_QUADS) ;
           glColor3f(0,1,0);
            glVertex3f( 0.0,  0.0,  0.0);
            glVertex3f( 0.05,  0.0,  0.0);
            glVertex3f( 0.05,  0.05,  0.0);
            glVertex3f( 0.0,  0.05,  0.0);

            glColor3f(0,1,1);
            glVertex3f( 0.0,  0.0, -0.05);
            glVertex3f( 0.0,  0.05, -0.05);
            glVertex3f( 0.05,  0.05, -0.05);
            glVertex3f( 0.05,  0.0, -0.05);

            glColor3f(1,1,0);
            glVertex3f( 0.0,  0.05, -0.05);
            glVertex3f( 0.0,  0.05,  0.0);
            glVertex3f( 0.05,  0.05,  0.0);
            glVertex3f( 0.05,  0.05, -0.05);

            glColor3f(0,0,1);
            glVertex3f( 0.0,  0.0, -0.05);
            glVertex3f( 0.05,  0.0, -0.05);
            glVertex3f( 0.05,  0.0,  0.0);
            glVertex3f( 0.0,  0.0,  0.0);

            glColor3f(1,0,1);
            glVertex3f( 0.05,  0.0, -0.05);
            glVertex3f( 0.05,  0.05, -0.05);
            glVertex3f( 0.05,  0.05,  0.0);
            glVertex3f( 0.05,  0.0,  0.0);

            glColor3f(1,1,1);
            glVertex3f( 0.0,  0.0, -0.05);
            glVertex3f( 0.0,  0.0,  0.0);
            glVertex3f( 0.0,  0.05,  0.0);
            glVertex3f( 0.0,  0.05, -0.05);
        glEnd() ;
*/


        // Würfel - Rechte Hand Regel: Daumen zeigt nach außen.
        glBegin(GL_QUADS) ;

            // Back
            glColor3f(1,0,0);
            glVertex3f( -0.01f, -0.01f, -0.01f);
            glVertex3f( -0.01f,  0.01f, -0.01f);
            glVertex3f(  0.01f,  0.01f, -0.01f);
            glVertex3f(  0.01f, -0.01f, -0.01f);
            // Front
            glColor3f(0,0,1);
            glVertex3f(  0.01f, -0.01f, 0.01f);
            glVertex3f(  0.01f,  0.01f, 0.01f);
            glVertex3f( -0.01f,  0.01f, 0.01f);
            glVertex3f( -0.01f, -0.01f, 0.01f);

            // Right
            glColor3f(0,1,0);
            glVertex3f(  0.01f,  0.01f,  0.01f);
            glVertex3f(  0.01f, -0.01f,  0.01f);
            glVertex3f(  0.01f, -0.01f, -0.01f);
            glVertex3f(  0.01f,  0.01f, -0.01f);
            // Left
            glColor3f(0,1,1);
            glVertex3f( -0.01f, -0.01f,  0.01f);
            glVertex3f( -0.01f,  0.01f,  0.01f);
            glVertex3f( -0.01f,  0.01f, -0.01f);
            glVertex3f( -0.01f, -0.01f, -0.01f);

            // Top
            glColor3f(1,1,0);
            glVertex3f(  0.01f,  0.01f, -0.01f);
            glVertex3f( -0.01f,  0.01f, -0.01f);
            glVertex3f( -0.01f,  0.01f,  0.01f);
            glVertex3f(  0.01f,  0.01f,  0.01f);

            // Bottom
            glColor3f(1,1,0);
            glVertex3f( -0.01f,  -0.01f,  0.01f);
            glVertex3f( -0.01f,  -0.01f, -0.01f);
            glVertex3f(  0.01f,  -0.01f, -0.01f);
            glVertex3f(  0.01f,  -0.01f,  0.01f);


        glEnd() ;

        // drawAxis();

        glPopMatrix();

        drawAR = false ;
    }




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


// Mat zu QImage
void MyGLWidget::showImage(cv::Mat image)
{

    image.copyTo(mOrigImage);

    mImgRatio = (float)image.cols/(float)image.rows;

    if( mOrigImage.channels() == 3)
        mRenderQtImg = QImage((const unsigned char*)(mOrigImage.data),
                              mOrigImage.cols, mOrigImage.rows,
                              mOrigImage.step, QImage::Format_RGB888);//.rgbSwapped();
    else if( mOrigImage.channels() == 1)
        mRenderQtImg = QImage((const unsigned char*)(mOrigImage.data),
                              mOrigImage.cols, mOrigImage.rows,
                              mOrigImage.step, QImage::Format_Indexed8);

   // mRenderQtImg = mRenderQtImg.mirrored();
    tex = mRenderQtImg;// = mRenderQtImg.mirrored();


    // Berechne Model View für den ersten Marker

    updateGL();
}

void MyGLWidget::loadProjectionMatrix() {
    //static cv::Mat_<double> persp ;
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


            //    cout << "perspective m \n" << persp << endl;

                persp = persp.t(); //to col-major

                glMatrixMode(GL_PROJECTION);
                //glLoadIdentity() ;
                glLoadMatrixd((double*)persp.data);

            }



}

//---------------------------------------------------------------------------------------------------------

QImage MyGLWidget::loadTexture2(char *filename, GLuint &textureID){
    glEnable(GL_TEXTURE_2D); // Enable texturing
    glGenTextures(1, &textureID); // Obtain an id for the texture
    glBindTexture(GL_TEXTURE_2D, textureID); // Set as the current texture

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    QImage im(filename);
    tex = QGLWidget::convertToGLFormat(im);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());


    glDisable(GL_TEXTURE_2D);

    return tex;
}
//---------------------------------------------------------------------------------------------------------

void MyGLWidget::sendMakerPos(std::vector<cv::Vec3f> _v3fCircles) {
    v3fCircles = _v3fCircles ; // Vielleicht als Referenz machen?
}
