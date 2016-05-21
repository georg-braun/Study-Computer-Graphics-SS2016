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
}

void MyGLWidget::resizeGL(int width, int height)
{
        // Compute aspect ratio
       height = (height == 0) ? 1 : height;  // ?

       // Set viewport to cover the whole window
       glViewport(0, 0, 640, 480);
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


    glEnable(GL_TEXTURE_2D); // Enable texturing
    glGenTextures(1, &backgroundimage); // Obtain an id for the texture
    glBindTexture(GL_TEXTURE_2D, backgroundimage); // Set as the current texture

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  //  tex = QImage(640, 480, QImage::Format_RGB888);
    tex = QGLWidget::convertToGLFormat(tex);
    qDebug() << tex.width() << " x " <<  tex.height() ;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_2D);
   // loadTexture2("c:\image.jpg", backgroundimage);
  //------------------------------------------

   glClearColor(0.4f, 0.1f, 0.1f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, backgroundimage);

   glBegin(GL_QUADS);
       glTexCoord2f(0,0); glVertex3f(-1, -1, -1);
       glTexCoord2f(1,0); glVertex3f(1, -1,  -1);
       glTexCoord2f(1,1); glVertex3f(1, 1,   -1);
       glTexCoord2f(0,1); glVertex3f(-1, 1,  -1);

   glEnd();

   glDisable(GL_TEXTURE_2D);


   //glMatrixMode(GL_MODELVIEW);  // Which matrix is active?
   //glLoadIdentity(); // Einheitsmatrix laden

  // Transformation
   //glTranslatef(0.0f, 0.0f, -7.0f); // Initial
   glBegin(GL_QUADS) ;
          // Front
          glColor3f(0,0,1);
          glVertex3f(  0.5f, -0.5f, 1.0f);
          glVertex3f(  1.0f,  1.0f, 1.0f);
          glVertex3f( -1.0f,  1.0f, 1.0f);
          glVertex3f( -1.0f, -1.0f, 1.0f);
   glEnd() ;
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
