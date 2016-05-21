#include "MyGLWidget.h"

MyGLWidget::MyGLWidget()
{

}

MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(parent)
{

    connect(timer, SIGNAL(timeout()),this,SLOT(updateGL()));
    timer->start(4000);

}


void MyGLWidget::initializeGL()
{

    makeCurrent();


    float r = ((float)mBgColor.darker().red())/255.0f;
    float g = ((float)mBgColor.darker().green())/255.0f;
    float b = ((float)mBgColor.darker().blue())/255.0f;
    glClearColor(r,g,b,1.0f);


// Für den blauen Quader
    //glEnable(GL_DEPTH_TEST);  // Activate depth comparisons and update depth buffer

    glEnable(GL_CULL_FACE);   // Draw Front or Back?

    glDepthFunc(GL_LEQUAL);   // Specify the depth buffer
    glShadeModel(GL_SMOOTH);  // GL_FLAT or GL_SMOOTH (interpolated)

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Are there Interpretations? Set hint!

    glClearDepth(1.0f);  // Clear value for depth buffer (used by glClear)

    // P1.3 - Black Background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Clear values used by glClear (Color-Buffer)

}

void MyGLWidget::resizeGL(int width, int height)
{
    makeCurrent();
        glViewport(0, 0, (GLint)width, (GLint)height);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glOrtho(0, width, 0, height, 0, 1);	// To Draw image in the center of the area

        glMatrixMode(GL_MODELVIEW);

        // ---> Scaled Image Sizes
        mOutH = width/mImgRatio;
        mOutW = width;

        if(mOutH>height)
        {
            mOutW = height*mImgRatio;
            mOutH = height;
        }

        //emit imageSizeChanged( mOutW, mOutH );
        // <--- Scaled Image Sizes

        mPosX = (width-mOutW)/2;
        mPosY = (height-mOutH)/2;




// Für blauen Quader
   // Compute aspect ratio
   height = (height == 0) ? 1 : height;  // ?

   // Set viewport to cover the whole window
   glViewport(0, 0, width, height);

   // Set projection matrix to a perspective projection
   glMatrixMode(GL_PROJECTION);  // Use projection matrix
   glLoadIdentity(); // Einheitsmatrix laden

   glFrustum(-0.05, 0.05, -0.05, 0.05, 0.1, 100.0);

}


void MyGLWidget::paintGL()
{

/*
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     makeCurrent();

        glClear(GL_COLOR_BUFFER_BIT);

        if (!mRenderQtImg.isNull())
        {
            glLoadIdentity();

            QImage image; // the image rendered

            glPushMatrix();
            {
                int imW = mRenderQtImg.width();
                int imH = mRenderQtImg.height();

                // The image is to be resized to fit the widget?
                if( imW != this->size().width() &&
                        imH != this->size().height() )
                {

                    image = mRenderQtImg.scaled( //this->size(),
                                                 QSize(mOutW,mOutH),
                                                 Qt::IgnoreAspectRatio,
                                                 Qt::SmoothTransformation
                                                 );

                    //qDebug( QString( "Image size: (%1x%2)").arg(imW).arg(imH).toAscii() );
                }
                else
                    image = mRenderQtImg;

                // ---> Centering image in draw area

                glRasterPos2i( mPosX, mPosY );
                // <--- Centering image in draw area

                imW = image.width();
                imH = image.height();

                glDrawPixels( imW, imH, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
            }
            glPopMatrix();

            // end
            glFlush();
        }
*/






    // Clear buffer to set color and alpha
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);  // Which matrix is active?
  //  glLoadIdentity(); // Einheitsmatrix laden
    // Apply model view transformations
    glMatrixMode(GL_MODELVIEW);  // Which matrix is active?
    glLoadIdentity(); // Einheitsmatrix laden

    // Transformation
    glTranslatef(0.0f, 0.0f, -7.0f); // Initial


    // Set color for drawing
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

    // Würfel - Rechte Hand Regel: Daumen zeigt nach außen.
/*    glBegin(GL_QUADS) ;

        // Back
        glColor3f(1,0,0);
        glVertex3f( -1.0f, -1.0f, -1.0f);
        glVertex3f( -1.0f,  1.0f, -1.0f);
        glVertex3f(  1.0f,  1.0f, -1.0f);
        glVertex3f(  1.0f, -1.0f, -1.0f);
        // Front
        glColor3f(0,0,1);
        glVertex3f(  1.0f, -1.0f, 1.0f);
        glVertex3f(  1.0f,  1.0f, 1.0f);
        glVertex3f( -1.0f,  1.0f, 1.0f);
        glVertex3f( -1.0f, -1.0f, 1.0f);

        // Right
        glColor3f(0,1,0);
        glVertex3f(  1.0f,  1.0f,  1.0f);
        glVertex3f(  1.0f, -1.0f,  1.0f);
        glVertex3f(  1.0f, -1.0f, -1.0f);
        glVertex3f(  1.0f,  1.0f, -1.0f);
        // Left
        glColor3f(0,1,0);
        glVertex3f( -1.0f, -1.0f,  1.0f);
        glVertex3f( -1.0f,  1.0f,  1.0f);
        glVertex3f( -1.0f,  1.0f, -1.0f);
        glVertex3f( -1.0f, -1.0f, -1.0f);

        // Top
        glColor3f(1,1,0);
        glVertex3f(  1.0f,  1.0f, -1.0f);
        glVertex3f( -1.0f,  1.0f, -1.0f);
        glVertex3f( -1.0f,  1.0f,  1.0f);
        glVertex3f(  1.0f,  1.0f,  1.0f);

        // Bottom
        glColor3f(1,1,0);
        glVertex3f( -1.0f,  -1.0f,  1.0f);
        glVertex3f( -1.0f,  -1.0f, -1.0f);
        glVertex3f(  1.0f,  -1.0f, -1.0f);
        glVertex3f(  1.0f,  -1.0f,  1.0f);

    glEnd() ;
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
                              mOrigImage.step, QImage::Format_RGB888).rgbSwapped();
    else if( mOrigImage.channels() == 1)
        mRenderQtImg = QImage((const unsigned char*)(mOrigImage.data),
                              mOrigImage.cols, mOrigImage.rows,
                              mOrigImage.step, QImage::Format_Indexed8);

    mRenderQtImg = mRenderQtImg.mirrored();
}
