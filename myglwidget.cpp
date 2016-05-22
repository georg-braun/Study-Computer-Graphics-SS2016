#include "MyGLWidget.h"

MyGLWidget::MyGLWidget()
{

}

MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(parent)
{

    connect(timer, SIGNAL(timeout()),this,SLOT(updateGL()));
    //timer->start(40);


}


void MyGLWidget::initializeGL()
{
    loadTexture2("c:\image.jpg", backgroundimage);

//    glEnable(GL_DEPTH_TEST);  // Activate depth comparisons and update depth buffer

    glEnable(GL_CULL_FACE);   // Draw Front or Back?
/*
    glDepthFunc(GL_LEQUAL);   // Specify the depth buffer
    glShadeModel(GL_SMOOTH);  // GL_FLAT or GL_SMOOTH (interpolated)

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Are there Interpretations? Set hint!
*/


    // P1.3 - Black Background
  //  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Clear values used by glClear (Color-Buffer)
}

void MyGLWidget::resizeGL(int width, int height)
{
        // Compute aspect ratio
       height = (height == 0) ? 1 : height;  // ?

       // Set viewport to cover the whole window
       glViewport(0, 0, 640, 480);

       // Set projection matrix to a perspective projection
     //  glMatrixMode(GL_PROJECTION);  // Use projection matrix
     //  glLoadIdentity(); // Einheitsmatrix laden

       //glFrustum(-0.05, 0.05, -0.05, 0.05, 0, 10.0);



}




QImage MyGLWidget::loadTexture2(char *filename, GLuint &textureID){
    glEnable(GL_TEXTURE_2D); // Enable texturing
    glGenTextures(1, &textureID); // Obtain an id for the texture
    glBindTexture(GL_TEXTURE_2D, textureID); // Set as the current texture

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    QImage im(filename);
    tex = QGLWidget::convertToGLFormat(im);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    glDisable(GL_TEXTURE_2D);

    return tex;
}

void MyGLWidget::paintGL()
{

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D); // Enable texturing
    glGenTextures(1, &backgroundimage); // Obtain an id for the texture
    glBindTexture(GL_TEXTURE_2D, backgroundimage); // Set as the current texture

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  //  tex = QImage(640, 480, QImage::Format_RGB888);
    tex = QGLWidget::convertToGLFormat(tex);
    //qDebug() << tex.width() << " x " <<  tex.height() ;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_2D);
   // loadTexture2("c:\image.jpg", backgroundimage);
  //------------------------------------------

   glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, backgroundimage);

   glBegin(GL_QUADS);
       glTexCoord2f(0,0); glVertex3f(-1, -1, -1.0);
       glTexCoord2f(1,0); glVertex3f(1, -1,  -1.0);
       glTexCoord2f(1,1); glVertex3f(1, 1,   -1.0);
       glTexCoord2f(0,1); glVertex3f(-1, 1,  -1.0);

   glEnd();

   glDisable(GL_TEXTURE_2D);


/*
   glMatrixMode(GL_MODELVIEW);
   glTranslatef(-1.0f, -1.0f, 0.0f); // Initial
   //glLoadIdentity();
   GLfloat markerX, markerY ;

    for(unsigned int i = 0; i < v3fCircles.size(); i++) {
        markerX = 2.0 / 640 * v3fCircles[i][0] ;
        markerY = 2.0 / 480 * v3fCircles[i][1] ;
        glBegin(GL_QUADS) ;
               // Front
               glColor3f(0,0,1);
               glVertex3f( markerX-0.1, markerY-0.1, -1.0f);
               glVertex3f( markerX+0.1, markerY-0.1, -1.0f);
               glVertex3f( markerX+0.1, markerY+0.1, -1.0f);
               glVertex3f( markerX-0.1, markerY+0.1, -1.0f);
        glEnd() ;

    }
*/
    // X - Axis red
        glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glEnd();
        // Y - Axis green
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glEnd();
        // Z - Axis blue
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, -1.0f);
        glEnd();

/*
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(-0.0f, -0.0f, -7.0f); // Initial
*/
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


    updateGL();
}


void MyGLWidget::sendMakerPos(std::vector<cv::Vec3f> _v3fCircles) {
    v3fCircles = _v3fCircles ; // Vielleicht als Referenz machen?
}
