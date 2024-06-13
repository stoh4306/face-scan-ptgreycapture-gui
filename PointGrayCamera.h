#pragma once

#include "FlyCapture2GUI.h"
#include"FlyCapture2Defs.h"
#include"FlyCapture2.h"
#include"FlyCapture2Platform.h"
#include"C\FlyCapture2Defs_C.h"


#include<vector>
#include<iostream>
#include<sstream>
#include<thread>
#include<Windows.h>

using namespace std;
using namespace FlyCapture2;

class MyGlWidget;
class CameraManager;


class PointGrayCamera
{
public:
	static bool startTrigger;
	static bool captureStart;
protected:
	Image imgBuffer;
	std::vector<TimeStamp>* timeArray;
	std::vector<Image>* seqRaw;
	unsigned int prevFrameNum;
	unsigned int deltaFrameNum;	
	unsigned int MissedNum;
	PGRGuid guid;
	Camera cam;
	Error error;
	Image img;
	Image rawImage;
	int index;
	MyGlWidget* view;

	int* currentCount;
	int fileCount;
	int startCount;
	bool isContinuous;
	TimeStamp beforeTime;

	int prefixNum;  
	bool isLooping;
	CameraManager* parent;

	static bool singleCap;
	//Image img;
	Image rgb;
	BMPOption bmpOp;
	char buffer[256];
	int numFrames;
public:
	
	int ManagerCurrentCount(int operation, int value);
	int SettingBeforeCapture(int NumFrames);
	int setTriggerFrame(int frame);
	MyGlWidget* GetView() { return view; }
	bool getConLoopEnd() { return isLooping; }
	PGRGuid& getGuid(){ return guid; }
	Camera* GetCam() { return &cam; }
	int init(int, MyGlWidget*, CameraManager*);
	int start();
	int stop();
	int release();
	int ImgProcessing(string path="",int nowFrame=0,int endFrame = 0,int prefix = 0);
	int GetDataFromBuffer();
	int SaveFIleSeq();
	int SaveFile();

	int fire(int prefix);
	int fireSeq(int prefix, int frame);
	int setFileFrame(int);
	int fileCapture(int);
	int Connect();
	int SetTriggerMode(bool);
	static void Callback(Image* pImg, const void* pData);
	
	int FireSoftwareTrigger();
	
	int GetWB(float& valueR ,float& valueB, bool& isOn, bool& isAuto, bool& isabsContorl);
	int GetProperty(PropertyType type, float& value, bool& isOn, bool& isAuto, bool& isabsContorl);
	int GetProperty(PropertyType type, float & value, bool & isOn, bool & isAuto, bool & isabsContorl, int valueSet);
	int SetProperty(PropertyType type, float value ,bool isOn=true,bool isAuto=false ,bool isabsContorl=true, int valueSet=0);
	int SetWB(float& valueR, float& valueB, bool& isOn, bool& isAuto, bool& isabsContorl);
	PointGrayCamera();
	~PointGrayCamera();
};

