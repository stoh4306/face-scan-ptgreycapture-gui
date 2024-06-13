#pragma once
#pragma managed
//#include "FlyCap2CameraControl_ILBridge.h"
//#include "FlyCap2CameraSelection_ILBridge.h"
//#ifdef GENICAM
//#include "FlyCap2PropertyGrid_ILBridge.h"
//#endif
//#include "AssemblyResolver.h"
#pragma unmanaged
#include "FlyCapture2GUI.h"
#include"FlyCapture2Defs.h"
#include"FlyCapture2.h"
#include"FlyCapture2Platform.h"

#include<vector>
#include<iostream>
#include<sstream>
#include<thread>
#include<Windows.h>
using namespace std;
using namespace FlyCapture2;

class MyGlWidget;

class PointGrey
{
public:
	MyGlWidget* view;
	int ObserverLen;
	std::vector<PGRGuid> guid;
	std::vector<function<void()> > ObserverFunc;
	unsigned int numOfCameras;
	std::vector<Camera*> cam;

	Error error;
	Image img;
	Image rawImage;


	PointGrey();
	bool init();
	int runCam();
	int release();
	int Fire();
	int Fire(int index);
	void PrintError(Error error);
	void PrintCameraInfo(CameraInfo *pCamInfo);
	bool FireSoftwareTrigger(Camera *pCam);

	static void callback(Image* pImg,const void* pData);

	virtual ~PointGrey();
	
};

