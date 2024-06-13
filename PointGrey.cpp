//#include "PointGrey.h"
//#include"MyGlWidget.h"
//void PointGrey::PrintError(Error error) { error.PrintErrorTrace(); }
//
//void PointGrey::PrintCameraInfo(CameraInfo *pCamInfo)
//{
//	cout << endl;
//	cout << "*** CAMERA INFORMATION ***" << endl;
//	cout << "Serial number - " << pCamInfo->serialNumber << endl;
//	cout << "Camera model - " << pCamInfo->modelName << endl;
//	cout << "Camera vendor - " << pCamInfo->vendorName << endl;
//	cout << "Sensor - " << pCamInfo->sensorInfo << endl;
//	cout << "Resolution - " << pCamInfo->sensorResolution << endl;
//	cout << "Firmware version - " << pCamInfo->firmwareVersion << endl;
//	cout << "Firmware build time - " << pCamInfo->firmwareBuildTime << endl<<
//		"isColorCamera - "<< pCamInfo->isColorCamera<<endl
//		<< endl;
//	
//}
//bool PointGrey::FireSoftwareTrigger(Camera *pCam)
//{
//	const unsigned int k_softwareTrigger = 0x62C;
//	const unsigned int k_fireVal = 0x80000000;
//	Error error;
//
//	error = pCam->WriteRegister(k_softwareTrigger, k_fireVal);
//	if (error != PGRERROR_OK)
//	{
//		PrintError(error);
//		return false;
//	}
//
//	return true;
//}
//
//
//
//PointGrey::PointGrey()
//{
//}
//
//bool PointGrey::init()
//{
//
//	FC2Version fc2Version;
//	Utilities::GetLibraryVersion(&fc2Version);
//
//	ostringstream version;
//	version << "FlyCapture2 library version: " << fc2Version.major << "."
//		<< fc2Version.minor << "." << fc2Version.type << "."
//		<< fc2Version.build;
//	cout << version.str() << endl;
//
//	ostringstream timeStamp;
//	timeStamp << "Application build date: " << __DATE__ << " " << __TIME__;
//	cout << timeStamp.str() << endl << endl;
//	Error error;
//
//	BusManager busMgr;
//
//	//unsigned int numCameras;
//	error = busMgr.GetNumOfCameras(&numOfCameras);
//	if (error != PGRERROR_OK)
//	{
//		PrintError(error);
//		return -1;
//	}
//	if (numOfCameras < 1)
//	{
//		cout << "No camera detected." << endl;
//		return -1;
//	}
//	else
//	{
//		cout << "Number of cameras detected: " << numOfCameras << endl;
//	}
//	//PGRGuid guid;
//	cam.resize(numOfCameras);
//	guid.resize(numOfCameras);
//	for (int i = 0; i < numOfCameras; ++i){
//		cam[i] = new Camera();
//		error = busMgr.GetCameraFromIndex(i, &guid[i]);
//		if (error != PGRERROR_OK)
//		{
//			PrintError(error);
//			return -1;
//		}
//	}
//	
//	return false;
//}
//
//int PointGrey::runCam()
//{
//	const int k_numImages = 1;
//
//	Error error;
//	
//	for (int i = 0; i < numOfCameras; ++i) {
//		// Connect to a camera
//		error = cam[i]->Connect(&guid[i]);
//		if (error != PGRERROR_OK)
//		{
//			PrintError(error);
//			return -1;
//		}
//
//		// Get the camera information
//		CameraInfo camInfo;
//		error = cam[i]->GetCameraInfo(&camInfo);
//		if (error != PGRERROR_OK)
//		{
//			PrintError(error);
//			return -1;
//		}
//
//		PrintCameraInfo(&camInfo);
//
//		// Start capturing images
//		cout << "Starting capture... " << endl;
//
//		//trigger setting
//
//		TriggerMode triggerMode;
//		triggerMode.parameter = 0;
//		triggerMode.mode = 0;
//		cam[i]->GetTriggerMode(&triggerMode);
//		// Set camera to trigger mode 0
//		triggerMode.onOff = true;
//		triggerMode.mode = 0;
//		triggerMode.parameter = 0;
//		// A source of 7 means software trigger
//		triggerMode.source = 7;
//		error = cam[i]->SetTriggerMode(&triggerMode);
//		if (error != PGRERROR_OK)
//		{
//			PrintError(error);
//			return -1;
//		}
//		error = cam[i]->StartCapture(PointGrey::callback);
//		if (error != PGRERROR_OK)
//		{
//			PrintError(error);
//			return -1;
//		}
//
//	}
//
//
//	return 0;
//}
//
//int PointGrey::release()
//{
//	for (int i = 0; i < numOfCameras; ++i) {
//		cout << "Stopping capture... " << endl;
//		error = cam[i]->StopCapture();
//		if (error != PGRERROR_OK)
//		{
//			PrintError(error);
//			return -1;
//		}
//		delete cam[i];
//	}
//	
//
//
//	//file save//
//	//PNGOption pOp;
//	//pOp.compressionLevel = 9;
//	//pOp.interlaced = 0;
//
//	//JPEGOption jOp;
//	//jOp.quality = 100;
//
//	//vecImages[0].Save("asdf.png", &pOp);
//	//img.Save("asdf2.png", &pOp);
//
//	//vecImages[0].Save("asdf.jpg", &jOp);
//	//img.Save("asdf2.jpg", &jOp);
//	//cout << "Using frame rate of " << fixed << setprecision(1) << frameRateToUse<< endl;
//	return 0;
//
//}
//void PointGrey::callback(Image* pImg,const void* pData) {
//	cout << "Call Back!! " << endl;
//}
//int PointGrey::Fire()
//{
//
//	for (int i = 0; i < numOfCameras; ++i) {
//		
//		//error = cam[i]->StartCapture(PointGrey::callback);
//		//if (error != PGRERROR_OK)
//		//{
//		//	PrintError(error);
//		//	return -1;
//		//}
//		FireSoftwareTrigger(cam[i]);
//		//error = cam[i]->RetrieveBuffer(&rawImage);
//		//if (error != PGRERROR_OK)
//		//{
//		//	cout << "Error grabbing image " << endl;
//		//	return -2;
//		//}
//		//else
//		//{
//		//	cout << "Grabbed image " << endl;
//		//}
//		//Image img;
//		//rawImage.Convert(PixelFormat::PIXEL_FORMAT_RGB8, &img);
//
//		//int buffersize = img.GetDataSize();
//		//if (view->buffer.size() == 0)
//		//	view->buffer.resize(buffersize);
//		//int tmp = img.GetCols();
//		//if (tmp > 0)
//		//	view->SetWidth(tmp);
//		//tmp = img.GetRows();
//		//if (tmp > 0)
//		//	view->SetHeight(tmp);
//
//
//		//CopyMemory(&view->buffer[0], img.GetData(), buffersize);
//		//
//		//view->paintGL();
//		//
//		//for_each(ObserverFunc.begin(), ObserverFunc.end(), [&](function<void()> fun) {
//		//	fun();
//		//});
//
//
//		//cout << "Stopping capture... " << endl;
//		//error = cam[i]->StopCapture();
//		//if (error != PGRERROR_OK)
//		//{
//		//	PrintError(error);
//		//	return -1;
//		//}
//	}
//	return 0;
//}
//
//int PointGrey::Fire(int index)
//{
//	error = cam[index]->StartCapture();
//	if (error != PGRERROR_OK)
//	{
//		PrintError(error);
//		return -1;
//	}
//	FireSoftwareTrigger(cam[index]);
//	error = cam[index]->RetrieveBuffer(&rawImage);
//	if (error != PGRERROR_OK)
//	{
//		cout << "Error grabbing image " << endl;
//		return -2;
//	}
//	else
//	{
//		cout << "Grabbed image " << endl;
//	}
//	Image img;
//	rawImage.Convert(PixelFormat::PIXEL_FORMAT_RGB8, &img);
//
//	int buffersize = img.GetDataSize();
//	if (view->buffer.size() == 0)
//		view->buffer.resize(buffersize);
//	int tmp = img.GetCols();
//	if (tmp > 0)
//		view->SetWidth(tmp);
//	tmp = img.GetRows();
//	if (tmp > 0)
//		view->SetHeight(tmp);
//
//
//	CopyMemory(&view->buffer[0], img.GetData(), buffersize);
//
//	view->paintGL();
//
//	for_each(ObserverFunc.begin(), ObserverFunc.end(), [&](function<void()> fun) {
//		fun();
//	});
//
//
//	cout << "Stopping capture... " << endl;
//	error = cam[index]->StopCapture();
//	if (error != PGRERROR_OK)
//	{
//		PrintError(error);
//		return -1;
//	}
//	return 0;
//}
//
//
//PointGrey::~PointGrey()
//{
//	release();
//}
