#pragma once
#include "QtWidgets/qopenglwidget.h"

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QOpenGLWidget>
#include<gl\GL.h>
#include<gl\GLU.h>
#include<iostream>
class MyGlWidget :
	public QOpenGLWidget
{
	Q_OBJECT


	Q_SIGNAL void sendCam(float, float, float);
public:
	MyGlWidget(QWidget *parent = 0);
	virtual ~MyGlWidget();
	//std::vector<std::vector<char> > buffers;
	std::vector<char> buffer;
	//char* buffer;
	int bufferSize;
	//std::vector<char>& setBuffer(int index, int size);
	inline void SetWidth(int w) { imageWidth = w; }
	inline void SetHeight(int h) { imageHeight = h; }
	bool isUpdating;
	bool isDragging;
	float zoom;
	float camX;
	float camY;
	float dragStartx;
	float dragStarty;
	float beforex;
	float beforey;
	float iRatio;
	float ratio;
protected:
	int imageWidth;
	int imageHeight;

	void unProject(int xCursor, int yCursor, GLdouble& wx, GLdouble& wy, GLdouble& wz);
	
public:
	void setBuffer(unsigned char* data, int size, int width, int height);
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);
	
	void Observer();

	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* even);
	void mouseMoveEvent(QMouseEvent* event);
	void wheelEvent(QWheelEvent* event);
	void setCam(float x, float y, float z);
	void setZoom(float z) { zoom = z; }
};

