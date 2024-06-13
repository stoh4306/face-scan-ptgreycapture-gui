#pragma once
#include"Common.h"


using namespace std;
using namespace FlyCapture2;

class PointGrayCamera;
class MyGlWidget;
class QtPointGreyCaptureGui;



class CameraManager
{
public:
	static CameraManager* instance;
protected:
	std::string path;
	CameraManager();
	int* numFrames;
	bool* isCapture;
	bool* isPreview;
	int* prefix;

	std::vector<PointGrayCamera*> CamList;
	unsigned int numOfCameras;
	bool* isTriggerOnOFF;
	
	BusManager busMgr;
	int camMode;
	bool life;
	thread capThread;
	bool captureing;
	QtPointGreyCaptureGui* form;
public:
	QtPointGreyCaptureGui* GetQForm() { return form; }
	bool getTriggerOnOFF(){ return *isTriggerOnOFF; }
	void SetPath(std::string str) { path = str; }
	int GetNumFrames() { return *numFrames; }
	static CameraManager* GetInstance();
	bool getIsCapture() { return *isCapture; }
	void SetParam(int _numFrames = 0, int _prefix = 0);
	void loop();
	static std::mutex mtx;
	static void PrintError(Error error);
	int initCameras(QtPointGreyCaptureGui*,QWidget*);
	int startCam();
	int startCam(int);
	int startSyncCam();

	int stopCam();
	int stopCam(int);

	int FireCam(int);
	int FireCamSeq(int,int );
	inline unsigned int GetCameraCount()const {return numOfCameras;}
	
	int GetWhteBalance(int index ,float & valueR, float & valueB, bool & isOn, bool & isAuto, bool & isabsContorl);
	int GetProperty(int, int, CamPropertyData&, int valueSet = 0);
	int GetProperty(int, int, float&, bool&, bool&, bool&,int);
	int SetProperty(int, int type, float value);
	int SetProperty(int, int, CamPropertyData&, int valueSet = 0);
	int SetShutterVal(int, CamPropertyData&, int valueSet = 0);
	int SetWhteBalance(int index, float & valueR, float & valueB, bool & isOn, bool & isAuto, bool & isabsContorl);
	
	bool isLooping();

	int Release();
	
	~CameraManager();
};

