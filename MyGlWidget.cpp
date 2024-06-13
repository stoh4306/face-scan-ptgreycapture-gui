#include "MyGlWidget.h"
#include<Qt3DInput\QMouseEvent>
#include"CameraManager.h"

MyGlWidget::MyGlWidget(QWidget *parent) :QOpenGLWidget(parent)
{
	imageWidth = 0;
	imageHeight = 0;
	bufferSize = 0;
	setUpdatesEnabled(true);
	isUpdating = false;
	camX = 0;
	camY = 0;
	zoom = 100;
	//buffer = nullptr;
}

MyGlWidget::~MyGlWidget()
{
}
void MyGlWidget::unProject(int xCursor, int yCursor, GLdouble & wx, GLdouble & wy, GLdouble & wz)
{
	
	GLdouble projection[16];
	GLdouble modelView[16];
	GLint viewPort[4];
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	//glGetIntegerv(GL_VIEWPORT, viewPort);
	viewPort[0] = 0;
	viewPort[1] = 0;
	viewPort[2] = width();
	viewPort[3] = height();
	GLfloat zCursor, winX, winY;
	winX = (float)xCursor;
	winY = (float)viewPort[3] - (float)yCursor;
	auto rat = iRatio / ratio;
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &zCursor);

	//promat
	//glFrustum(-0.01, 0.01, -0.01 / (iRatio / ratio), 0.01 / (iRatio / ratio), 0.01, 5);
	//
	if (rat <= 1)
		projection[5] = (iRatio / ratio);
	else
		projection[0] = 1 / (iRatio / ratio);
	if (gluUnProject(winX, winY, zCursor, modelView, projection, viewPort, &wx, &wy, &wz) == GLU_FALSE) {
		//printf("½ÇÆÐ\n");
	}
	wx *= zoom*0.01f;
	wy *= zoom *0.01f;
	wx += camX;
	wy += camY;
	//printf("viewPort : %d, %d, %d, %d, unProjected Pos : %lf, %lf, %lf\n", viewPort[0], viewPort[1], viewPort[2], viewPort[3], wx, wy, wz);
}
void MyGlWidget::setBuffer(unsigned char * data, int size, int width, int height)
{
	
	if (data != nullptr) {
		if (this->bufferSize < size) {
			this->bufferSize = size;
			this->imageWidth = width;
			this->imageHeight = height;
			this->buffer.resize(size);
			//if (this->buffer!=nullptr) {
			//	delete[] this->buffer;
			//	this->buffer = nullptr;
			//}
		
			//this->buffer = new char[size];
		}
		CopyMemory(&this->buffer[0], data, size);
		update();

	}
	CameraManager::GetInstance()->mtx.lock();
	isUpdating = false;
	CameraManager::GetInstance()->mtx.unlock();
}
void MyGlWidget::initializeGL() {
	glClear(GL_COLOR_BUFFER_BIT);
	//QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	glClearColor(0.5f, 0.5f, 0.5f, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_COLOR_MATERIAL);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	paintGL();
}
void MyGlWidget::paintGL() {
	
	if (imageWidth == 0)
		return;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	iRatio = (float)imageHeight / imageWidth;
	ratio = (float)height() / width();
	if ((iRatio / ratio) <= 1) {
		glFrustum(-0.01, 0.01, -0.01 / (iRatio / ratio), 0.01 / (iRatio / ratio), 0.01, 10000);
	}
	else {


		glFrustum(-0.01*(iRatio / ratio), 0.01*(iRatio / ratio), -0.01, 0.01, 0.01, 10000);
	}
	gluLookAt(camX, camY, zoom, camX, camY, 0, 0, 1, 0);
	
	//glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);

	//glLoadIdentity();


	//glBegin(GL_TRIANGLES);
	//glColor3f(1.0, 0.0, 0.0);
	//glVertex3f(-0.5, -0.5, 0);
	//glColor3f(0.0, 1.0, 0.0);
	//glVertex3f(0.5, -0.5, 0);
	//glColor3f(0.0, 0.0, 1.0);
	//glVertex3f(0.0, 0.5, 0);
	//glEnd();


	//make a texture
	//set texture parameters
	//mat to texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	////Set texture clamping method
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


	////Depth data -> moniter data(?)

	glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
	glPixelStorei(GL_UNPACK_SWAP_BYTES, TRUE);

	int Center[8][2] = {
		{ -1, 3 },{ 1, 3 },{ -1, 1 },{ 1,1 },
		{ -1, -1 },{ 1, -1 },{ -1, -3 },{ 1, -3 }
	};

	if (imageWidth > 0 && imageHeight > 0) {
		glTexImage2D(GL_TEXTURE_2D,         // Type of texture
			0,                   // Pyramid level (for mip-mapping) - 0 is the top level
			GL_RGB,              // Internal colour format to convert to
			imageWidth,//scaleMat.cols,          // Image width  i.e. 640 for Kinect in standard mode
			imageHeight,//scaleMat.rows,          // Image height i.e. 480 for Kinect in standard mode
			0,                   // Border width in pixels (can either be 1 or 0)
			GL_RGB,              // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
			GL_UNSIGNED_BYTE,    // Image data type
			&buffer[0]);//scaleMat.ptr());        // The actual image data itself

		glEnable(GL_TEXTURE_2D);
		glColor3f(1.0f, 1.0f, 1.0f); // the drawing depth
		glBegin(GL_QUADS);
		{
			glTexCoord2f(1.0f, 1.0f); glVertex3f(100.0f, -100.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-100.0f, -100.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, 100.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(100.0f, 100.0f, 0.0f);
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	
	//QOpenGLWidget::paintGL();
}
void MyGlWidget::resizeGL(int w, int h) {

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45, (float)w / h, 0.01, 100.0);
	gluPerspective(45, 1, 0.01, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0, 0, 2.425, 0, 0, 0, 0, 1, 0);
}

void MyGlWidget::Observer()
{
	//
	QOpenGLWidget::update();
	//std::cout << "update" << std::endl;
	//repaint();
}

void MyGlWidget::mousePressEvent(QMouseEvent * event)
{
	double clickedX, clickedY, clickedZ;
	unProject(event->x(), event->y(), clickedX, clickedY, clickedZ);

	auto w = imageWidth;
	auto h = imageHeight;
	clickedX += 1;
	clickedY = 1 - clickedY;
	clickedX *= 0.5f*w;// *zoomPer;
	clickedY *= 0.5f*h;// *zoomPer;

	auto scaleCamX = camX;
	auto scaleCamY = camY;

	scaleCamX += 1;
	scaleCamY = 1 - scaleCamY;
	scaleCamX *= 0.5f*width();
	scaleCamY *= 0.5f*height();


	std::cout << " Clicked Pixel : " << (int)clickedX << " , " << (int)clickedY << std::endl;
	//std::cout << " cam Pixel : " << scaleCamX << " , " << scaleCamY << std::endl;

	isDragging = true;
	dragStartx = event->x();
	dragStarty = event->y();
	beforex = dragStartx;
	beforey = dragStarty;
}

void MyGlWidget::mouseReleaseEvent(QMouseEvent * even)
{
	QOpenGLWidget::mouseReleaseEvent(even);
	isDragging = false;
}

void MyGlWidget::mouseMoveEvent(QMouseEvent * event)
{
	QOpenGLWidget::mouseMoveEvent(event);
	double clickedX, clickedY, clickedZ;
	auto w = imageWidth;
	auto h = imageHeight;
	if (w == 0 || h == 0)
		return;
	unProject(event->x(), event->y(), clickedX, clickedY, clickedZ);



	clickedX += 1;
	clickedY = 1 - clickedY;
	clickedX *= 0.5f*w;// *zoomPer;
	clickedY *= 0.5f*h;// *zoomPer;
					   // std::cout << "mouseMoveEvent" << std::endl;

	if (isDragging) {
		int deltax = event->x() - beforex;
		int deltay = event->y() - beforey;
		auto btn = event->buttons();
		if (btn & Qt::RightButton) {
			float movex = zoom*deltax / width() * 2;
			float movey = zoom*deltay / height() * 2;
			camX -= movex;
			camY += movey;
			beforex = event->x();
			beforey = event->y();
			double topX, topY;
			double bottomX, bottomY;
			unProject(0, 0, topX, topY, clickedZ);
			topX += 1;
			topY = 1 - topY;
			topX *= 0.5f*w;// *zoomPer;
			topY *= 0.5f*h;// *zoomPer;

			unProject(width() - 1, height() - 1, bottomX, bottomY, clickedZ);
			bottomX += 1;
			bottomY = 1 - bottomY;
			bottomX *= 0.5f*w;// *zoomPer;
			bottomY *= 0.5f*h;// *zoomPer;

			//FrameMGR->SetViewPort(topX, topY, bottomX, bottomY);
			//update();
			sendCam(camX, camY, zoom);
		}
		
	}
}

void MyGlWidget::wheelEvent(QWheelEvent * event)
{
	if (event->delta()<0) {
		if(zoom<5000)
			zoom *= 2;
	}
	else {
		if(zoom>2)
			zoom /= 2;

	}
	sendCam(camX, camY, zoom);
}

void MyGlWidget::setCam(float x, float y, float z)
{
	camX = x;
	camY = y;
	zoom = z;
}
