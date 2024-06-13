#include"Common.h"
#include "CameraManager.h"
#include"PointGrayCamera.h"
#include"QtPointGreyCaptureGui.h"
#include"MyGlWidget.h"
#include"QStdStream.h"
std::mutex CameraManager::mtx;
CameraManager* CameraManager::instance;
CameraManager * CameraManager::GetInstance()
{

	if (instance == nullptr) {
		mtx.lock();
		if (instance == nullptr) {
			instance = new CameraManager;
		}
		mtx.unlock();
	}

	return instance;
}
void CameraManager::SetParam(int _numFrames, int _prefix)
{
	std::string str="";
	ostringstream ststr(str);
	
	while (captureing)
		this_thread::yield();
	mtx.lock();

	if (_numFrames==0) {
		//CamList[0]->SettingBeforeCapture(0);
		//for (int i = 1; i<numOfCameras; ++i)
		//	CamList[i]->SettingBeforeCapture(1);
		ststr << "Preview mode";
		*isTriggerOnOFF = !*isTriggerOnOFF;
		*isCapture = false;
		//*isPreview = !*
		if (*isTriggerOnOFF) {
			*numFrames = 0;
			ststr << " off" << endl;
		}
		else {
			*numFrames = 0;
			ststr << " on" << endl;
		}
		
	}
	else {
		*prefix = _prefix;
		*numFrames = _numFrames;
		*isCapture = true;
		if(_numFrames == 1)
			ststr << "SingleFrame Capture" << endl;
		else
			ststr << "Sequence " << _numFrames << "Frame Capture start" << endl;
		//for (int i = 0; i<numOfCameras; ++i)
		//	CamList[i]->SettingBeforeCapture(*numFrames);
	}
	mtx.unlock();
	std::cout << str;
}
void CameraManager::loop()
{
	int frames;
	bool isCap;
	std::vector<std::thread> th(numOfCameras);
	while (life) {
		
		mtx.lock();
		captureing = true;
		frames = *numFrames;
		isCap = *isCapture;
		mtx.unlock();
		for (int i = 0; i<numOfCameras; ++i)
			CamList[i]->SettingBeforeCapture(frames);


		

		//for (int j = numOfCameras - 1; j >= 0; --j) {
		//	th[j] = std::thread([=](std::vector<PointGrayCamera*>& CamList) {
		//		int error = 0;
		//		//error = CamList[j]->ImgProcessing(path, i, frames, *prefix);
		//		error = CamList[j]->GetDataFromBuffer();
		//		if (error < 0) {
		//			std::cout << "Error code : " << error << std::endl;
		//		}
		//	}, std::ref(CamList));

		//}
		//for (int j = numOfCameras - 1; j >= 0; --j) {
		//	th[j].join();
		//}


		if (isCap) {
			std::cout << "Fire!" << std::endl;
			for (int i = 0; i < frames; ++i) {

				
				for (int j = numOfCameras - 1; j >= 0; --j) {
					th[j] = std::thread([=](std::vector<PointGrayCamera*>& CamList) {
						int error = 0;
						error = CamList[j]->ImgProcessing(path, i, frames, *prefix);
						//error = CamList[j]->GetDataFromBuffer();
						if (error < 0) {
							std::cout << "Error code : " << error << std::endl;
						}
					},std::ref(CamList));

				}
				for (int j = numOfCameras - 1; j >= 0; --j) {
					th[j].join();
				}
				//std::cout << "Frame : " << i <<" / " << frames<< std::endl;
			}
			*isCapture = false;
		}
		else{
			for (int j = 0; j < numOfCameras; ++j) {
				if(form!=nullptr && form->viewState(j))
					CamList[j]->ImgProcessing();
			}
		}

		//mtx.lock();
		captureing = false;
		//mtx.unlock();


		////////////////////////////////////////////////////////////////

		//mtx.lock();
		//for (int i = 0; i<numOfCameras; ++i)
			//CamList[i]->SettingBeforeCapture(frames);
		
		//if (isCap) {
		//	//mtx.unlock();
		//	std::cout << "Fire!" << std::endl;
		//	//int errorCode = CamList[0]->FireSoftwareTrigger();
		//	//if (errorCode != 0) {
		//	//	std::cout << "Fire Error code : " << errorCode << std::endl;
		//	//}
		//	for (int i = 0; i < frames; ++i) {

		//		std::vector<std::thread> th(numOfCameras);
		//		for (int j = numOfCameras - 1; j >= 0; --j) {
		//			th[j] = std::thread([=](std::vector<PointGrayCamera*>& CamList) {
		//				int error = 0;
		//				error = CamList[j]->ImgProcessing(path, i, frames, *prefix);
		//				if (error != 0) {
		//					std::cout << "Error code : " << error << std::endl;
		//				}
		//			},std::ref(CamList));

		//		}
		//		for (int j = numOfCameras - 1; j >= 0; --j) {
		//			th[j].join();
		//		}
		//		//std::cout << "Frame : " << i <<" / " << frames<< std::endl;
		//	}
		//	*isCapture = false;
		//}
		//else if (!*isTriggerOnOFF) {
		//	//mtx.unlock();
		//	int errorCode = CamList[0]->FireSoftwareTrigger();
		//	for (int j = 0; j < numOfCameras; ++j) {
		//		CamList[j]->ImgProcessing();
		//	}
		//}
		//else {
		//	//mtx.unlock();
		//}
		////mtx.lock();
		//captureing = false;
		////mtx.unlock();
		//
	}
}
void CameraManager::PrintError(Error error)
{
	error.PrintErrorTrace();
}

int CameraManager::initCameras(QtPointGreyCaptureGui* form, QWidget* layout)
{
	this->form = form;
	ifstream fs;
	fs.open("serial.txt");
	int numOfLine;
	fs >> numOfLine;
	std::vector<int > serialList(numOfLine);
	for (int i = 0; i < numOfLine; ++i) {
		int serial;
		int idx;
		fs >> serial;
		fs >> idx;
		serialList[idx] = serial;
	}
	isPreview = new bool;
	*isPreview = false;
	
	life = true;
	prefix = new int;
	*prefix = 0;
	numFrames = new int;
	*numFrames = 0;
	isCapture = new bool;
	*isCapture = false;

	isTriggerOnOFF = new bool;
	*isTriggerOnOFF = true;


	camMode = 0;
	FC2Version fc2Version;
	Utilities::GetLibraryVersion(&fc2Version);

	ostringstream version;
	version << "FlyCapture2 library version: " << fc2Version.major << "."
		<< fc2Version.minor << "." << fc2Version.type << "."
		<< fc2Version.build;
	qout << version.str() << endl;

	ostringstream timeStamp;
	timeStamp << "Application build date: " << __DATE__ << " " << __TIME__;
	qout << timeStamp.str() << endl << endl;
	Error error;

	
	
	//unsigned int numCameras;
	error = busMgr.GetNumOfCameras(&numOfCameras);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}
	if (numOfCameras < 1)
	{
		qout << "No camera detected." << endl;
		return -1;
	}
	else
	{
		qout << "Number of cameras detected: " << numOfCameras << endl;
	}
	//PGRGuid guid;

	int re = 0;

	//CamList.resize(numOfCameras);
	numOfCameras = numOfLine;
	form->views.resize(numOfCameras);
	for (int i = 0; i < numOfCameras; ++i) {
		auto newCam = new PointGrayCamera();
		unsigned int serial;
		error = busMgr.GetCameraFromSerialNumber(serialList[i], &newCam->getGuid());
		if (error != PGRERROR_OK)
		{
			cout << "Can not Find Camera! Cehck Serial : " <<serialList[i]<< endl;
			//PrintError(error);
			life = false;
			re =-1;
			delete newCam;
			continue;
		}
		
		form->views[i] = new MyGlWidget(layout);
		MyGlWidget* view = form->views[i];
		view->setParent(layout);
		view->setGeometry((i % 2) * 465 + 320, (i / 2) * 390 + 10, 400, 300);

		view->resize(459, 384);
		view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		view->show();
		((QGridLayout*)(layout->layout()))->addWidget(view,i/2,i%2);
		form->connect(view, SIGNAL(sendCam(float, float, float)), form, SLOT(SendData(float, float, float)));

		auto dd = view->size();
		auto ee = view->geometry();
		view->setVisible(true);
		int re = newCam->init(i, view,this);

		if (re != 0) {
			cout << "cam " << i << " init error!" << endl;
			break;
		}
		form->AddCamListWidget(QString::asprintf("%d", serialList[i]));
		CamList.push_back(newCam);
	}
	int h = max((CamList.size() / 2) * 390 + 15, 565);
	form->resize(465 * 2 + 325, h);
	form->ViewOnlyCheckedView();
	
	return re;
}

int CameraManager::startCam()
{
	//std::vector<thread> th(numOfCameras);
	for (int i = 0; i < numOfCameras; ++i) {
		CamList[i]->start();
		//th[i] = thread(&PointGrayCamera::start, CamList[i]);
	}
	//PointGrayCamera::startTrigger = true;
	//for (int i = 0; i < numOfCameras; ++i) {
	//	th[i].join();
	//}
	//startSyncCam();
	//if (re >= 0)
	capThread = thread(&CameraManager::loop, this);
	return 0;
}

int CameraManager::startCam(int index)
{
	CamList[index]->start();
	//startSyncCam();
	return 0;
}

int CameraManager::startSyncCam()
{
	std::vector<Camera*> pCamList(numOfCameras);
	for (int i = 0; i < numOfCameras; ++i) {
		pCamList[i] = CamList[i]->GetCam();
	}
	Camera::StartSyncCapture(numOfCameras, (const Camera**)(&pCamList[0]));
	
	return 0;
}

int CameraManager::stopCam()
{
	for (int i = 0; i < CamList.size(); ++i) {
		CamList[i]->stop();
	}
	return 0;
}

int CameraManager::stopCam(int index)
{
	CamList[index]->stop();
	return 0;
}

int CameraManager::FireCam(int prefix)
{
	//startCam();
	for (int i = 0; i < numOfCameras; ++i) {
		
		CamList[i]->setFileFrame(1);
		CamList[i]->fire(prefix);
	}

	for (int i = 0; i < numOfCameras; ++i) {

		CamList[i]->setFileFrame(0);
	}
	//PointGrayCamera::startTrigger = true;
	return 0;
}

int CameraManager::FireCamSeq(int prefix,int frame)
{
	for(int i=0;i<numOfCameras;++i)
		CamList[i]->fireSeq(prefix,frame);
	return 0;
}

int CameraManager::GetWhteBalance(int index, float & valueR, float & valueB, bool & isOn, bool & isAuto, bool & isabsContorl)
{
	return CamList[index]->GetWB(valueR, valueB, isOn, isAuto, isabsContorl);
}

int CameraManager::GetProperty(int index, int type, CamPropertyData& out,int valueSet)
{
	return CamList[index]->GetProperty((PropertyType)type, out.value, out.onOff, out.isAuto, out.onePush,valueSet);
	
}

int CameraManager::GetProperty(int index, int type, float& value, bool& a, bool& b, bool& c,int d)
{
	CamList[index]->GetProperty((PropertyType)type,value,a,b,c,d);
	return 0;
}

int CameraManager::SetProperty(int index, int type, float value)
{
	CamList[index]->SetProperty((PropertyType)type, value);
	return 0;
}

int CameraManager::SetProperty(int index, int type, CamPropertyData & in, int valueSet)
{
	return CamList[index]->SetProperty((PropertyType)type, in.value, in.onOff, in.isAuto, in.onePush, valueSet);
}

int CameraManager::SetShutterVal(int index, CamPropertyData & in, int valueSet)
{
	if(index == 0)
		return CamList[index]->SetProperty(PropertyType::SHUTTER, in.value, in.onOff, in.isAuto, in.onePush, valueSet);
	else
		return CamList[index]->SetProperty(PropertyType::SHUTTER, in.value, in.onOff, in.isAuto, in.onePush, valueSet);
}	

int CameraManager::SetWhteBalance(int index, float & valueR, float & valueB, bool & isOn, bool & isAuto, bool & isabsContorl)
{
	return CamList[index]->SetWB(valueR, valueB, isOn, isAuto, isabsContorl);
}
 

bool CameraManager::isLooping()
{
	{
		bool result = false;
		for (int i = 0; i < numOfCameras; ++i) {
			result |= CamList[i]->getConLoopEnd();
		}
		return result;
	}
}


int CameraManager::Release()
{
	life = false;
	std::this_thread::sleep_for(chrono::milliseconds(1000));

	for (int i = 0; i < CamList.size(); ++i) {
		CamList[i]->release();
	}
	for (int i = 0; i <  CamList.size(); ++i) {
		delete CamList[i];
	}
	return 0;
}

CameraManager::CameraManager()
{
	form = nullptr;
	
}


CameraManager::~CameraManager()
{
	
	life = false;
	std::this_thread::sleep_for(chrono::milliseconds(1000));

	delete isPreview;
	delete prefix;
	delete numFrames;
	delete isCapture;
	delete isTriggerOnOFF;
	std::this_thread::sleep_for(chrono::milliseconds(1000));
	
}
