#include "PointGrayCamera.h"
#include"CameraManager.h"
#include"MyGlWidget.h"
#include"QtPointGreyCaptureGui.h"

#define ErrorCheck(x) if (error != PGRERROR_OK)\
{\
	qout<<"Error! code : " <<x<<endl;\
	CameraManager::PrintError(error);\
	return x;\
}


const int NUM_CAPTURE_FRAMES = 30;
bool PointGrayCamera::captureStart;
bool PointGrayCamera::singleCap;
bool PointGrayCamera::startTrigger;
int PointGrayCamera::setTriggerFrame(int frame)
{

	TriggerMode triggerMode;
	cam.GetTriggerMode(&triggerMode);
	triggerMode.parameter = frame;
	cam.SetTriggerMode(&triggerMode);

	return 0;
}
int PointGrayCamera::init(int index, MyGlWidget* v, CameraManager* p)
{
	prevFrameNum = 0;
	numFrames = INT_MIN;
	MissedNum = 0;
	currentCount = new int;
	*currentCount = 0;
	timeArray = new std::vector<TimeStamp>;
	seqRaw = new std::vector<Image>;
	startTrigger = false;
	captureStart = false;
	isLooping = false;
	singleCap = false;
	FC2Config fc2config;
	this->view = v;
	this->parent = p;

	this->index = index;
	error = cam.Connect(&guid);
	ErrorCheck(-1);
	CameraInfo camInfo;
	error = cam.GetCameraInfo(&camInfo);
	ErrorCheck(-2);




	qout << "Starting capture... " << endl;

	//trigger setting

	TriggerMode triggerMode;
	//triggerMode.parameter = 0;
	//triggerMode.mode = 0;
	cam.GetTriggerMode(&triggerMode);
	//// Set camera to trigger mode 0
	//triggerMode.onOff = true;
	////triggerMode.mode = 0;
	////triggerMode.parameter = 0;
	//// A source of 7 means software trigger
	//triggerMode.source = 7;
	//error = cam.SetTriggerMode(&triggerMode);
	//ErrorCheck(-1);


	if (index == 0) {
		triggerMode.onOff = false;
		//triggerMode.mode = 15;
		//triggerMode.source = 7;
		//triggerMode.parameter = NUM_CAPTURE_FRAMES;// 150;
		
		StrobeControl strobe;
		//strobe.source = 2;
		error = cam.GetStrobe(&strobe);
		strobe.source = 2;
		strobe.onOff = true;
		strobe.polarity = 1;
		error = cam.SetStrobe(&strobe);
	}
	else
	{
		triggerMode.onOff = true;
		triggerMode.mode = 14;
		triggerMode.source = 3;//triggerMode.source = 2;
		triggerMode.polarity = 1;
	}

	error = cam.SetTriggerMode(&triggerMode);
	ErrorCheck(-3);
	error = cam.GetConfiguration(&fc2config);
	ErrorCheck(-4);

	fc2config.numBuffers = NUM_CAPTURE_FRAMES; //150;
	//fc2config.grabMode = BUFFER_FRAMES;
	fc2config.grabMode = DROP_FRAMES;
	fc2config.grabTimeout = 5000;

	error = cam.SetConfiguration(&fc2config);
	ErrorCheck(-5);

	Property Prop;
	Prop.type = SHUTTER;
	error = cam.GetProperty(&Prop);
	ErrorCheck(-6);
	Prop.onOff = true;
	Prop.autoManualMode = false;
	Prop.absControl = true;
	if (index == 0)
	{
		//Prop.absValue = 16.587; //33;
		Prop.absValue = 15.332f; // 33.3f; //33;
	}
	else
	{
		//Prop.absValue = 16.587 - 0.6;
		Prop.absValue = 15.332f; // 33.3f; //33;
	}

	


	//error = cam.SetProperty(&Prop);
	error = cam.SetProperty(&Prop);
	Prop.type = FRAME_RATE;
	error = cam.GetProperty(&Prop);
	std::cout << "Framerate : " << Prop.absValue << std::endl;
	if (index == 0)
	{
		Prop.onOff = true;
		Prop.autoManualMode = false;
		Prop.absControl = true;
		Prop.absValue = 60.0f;
		cam.SetProperty(&Prop);
		//pCameras[i].GetProperty(&Prop);
		//std::cout << "->" << Prop.absValue << std::endl;
	}



	//property Setting
	//Prop.type = PropertyType::BRIGHTNESS

	ErrorCheck(-7);

	EmbeddedImageInfo EmbeddedInfo;
	error = cam.GetEmbeddedImageInfo(&EmbeddedInfo);

	if (EmbeddedInfo.frameCounter.available == true)
	{
		EmbeddedInfo.frameCounter.onOff = true;
	}
	else
	{
		qprintf("Framecounter is not available! \n");
	}

	if (EmbeddedInfo.timestamp.available == true)
	{
		EmbeddedInfo.timestamp.onOff = true;
	}

	error = cam.SetEmbeddedImageInfo(&EmbeddedInfo);
	ErrorCheck(-8);

	//error = cam.StartCapture();
	//ErrorCheck(-1);
	

	return 0;
}

int PointGrayCamera::start()
{

	//while (!startTrigger)
	//{
	//	this_thread::yield();
	//}
	error = cam.StartCapture();
	//error = cam.StartCapture(Callback,this);
	ErrorCheck(-9);
	startTrigger = false;
	return 0;
}

int PointGrayCamera::stop()
{
	error = cam.StopCapture();
	ErrorCheck(-2);
	return 0;
}

int PointGrayCamera::release()
{
	error = cam.Disconnect();
	ErrorCheck(-3);
	return 0;
}
int PointGrayCamera::ManagerCurrentCount(int operation, int value)
{
	CameraManager::mtx.lock();
	if (operation == 0) {

	}
	else if (operation == 1) {
		*currentCount += value;
	}
	else if (operation == 2) {
		*currentCount -= value;
	}
	else if (operation == 3) {
		*currentCount = 0;
	}
	CameraManager::mtx.unlock();
	return *currentCount;
}
int PointGrayCamera::SettingBeforeCapture(int NumFrames) {
	if (numFrames != NumFrames) {
		TriggerMode triggerMode;
		cam.GetTriggerMode(&triggerMode);

		if (NumFrames > 0) {
			//triggerMode.onOff = true;
			//triggerMode.parameter = NumFrames;

			timeArray->resize(NumFrames);
			seqRaw->resize(NumFrames);
		}
		else if (NumFrames == 0) {
			//triggerMode.onOff = true;
			//triggerMode.parameter = 1;
			timeArray->resize(1);
			seqRaw->resize(1);
		}
		else {
			//triggerMode.onOff = true;
			//triggerMode.parameter = 1;
			timeArray->resize(1);
			seqRaw->resize(1);
		}
		numFrames = NumFrames;
		cam.SetTriggerMode(&triggerMode);
	}
	return 0;
}

int PointGrayCamera::ImgProcessing(string path, int nowFrame, int endFrame, int prefix)
{

	//error = cam.RetrieveBuffer(&(*seqRaw)[nowFrame]);
	error = cam.RetrieveBuffer(&imgBuffer);
	//while (ManagerCurrentCount(0,0) < endFrame)
	//	this_thread::yield();
	if (error != PGRERROR_OK) {
		cout << "RetrieveBuffer Error : " << error.GetDescription() << std::endl;
	}
	//if ((endFrame == 0 && view->isUpdating)) {
	//	//puts("DropFrame0");
	//	//--nowFrame;
	//	return 0;
	//}
	prefixNum = prefix;

	if (imgBuffer.GetRows() <= 0) {
		cout << "Cam" << index << " Frame " << nowFrame << " Missed!" << endl;
		return -1;
	}
	
	
	ImageMetadata metaData = imgBuffer.GetMetadata();
	if (prevFrameNum == 0) {
		prevFrameNum = metaData.embeddedFrameCounter;
		deltaFrameNum = 1;
	}
	else {
		deltaFrameNum = metaData.embeddedFrameCounter - prevFrameNum;
	}
	if (deltaFrameNum != 1) {
		MissedNum += deltaFrameNum - 1;
		std::cout << "Cam "<<index<<"'s Missing Frame : " << deltaFrameNum - 1 << "Frames" << std::endl;
		//std::cout << "-"<< MissedNum<<"-";
		//MissedNum = 0;
		//deltaFrameNum = 1;
		prevFrameNum = metaData.embeddedFrameCounter;
		//return 0;
	}
	else {
		//std::cout << ".";
	}
	prevFrameNum = metaData.embeddedFrameCounter;
	if (true) {
		Image* tmp=nullptr;
		if (endFrame != 0) {
			(*timeArray)[nowFrame] = imgBuffer.GetTimeStamp();
			(*seqRaw)[nowFrame].DeepCopy(&imgBuffer);
		}
		if (endFrame == 1) {
			tmp = new Image;
			tmp->DeepCopy(&(*seqRaw)[nowFrame]);
		}
		//file save
		if (endFrame == 1) {
			//thread th([=]() {

			BMPOption bmpOp;
			//char buffer[256];
			wchar_t str[255] = { 0 };
			wstring ww;
			ww.assign(path.begin(), path.end());
			wsprintf(str, L"%s/Cam%d", ww.c_str(), index);
			CreateDirectory(str, NULL);
			sprintf_s(buffer, "%s/Cam%d/pg_color_%04d.bmp", path.c_str(), index, prefixNum);
			Image img;
			tmp->Convert(PixelFormat::PIXEL_FORMAT_RGB8, &img);
			img.Save(buffer, &bmpOp);
			rgb.DeepCopy(&img);
			std::cout << buffer << " Saved" << endl;

			CameraManager::GetInstance()->GetQForm()->ButtonOn();
			delete tmp;
			//if (view->isUpdating) {

			//	return 0;
			//}
			//view->isUpdating = true;
			//(*seqRaw)[nowFrame].Convert(PixelFormat::PIXEL_FORMAT_RGB8, &rgb);
			//view->setBuffer(rgb.GetData(), rgb.GetDataSize(), rgb.GetCols(), rgb.GetRows());
			//return 0;
			//});
			//th.detach();
		}
		else if (endFrame > 1 && nowFrame == endFrame - 1) {

			thread th([=]() {
				std::cout << "Cam " << index << " Capture End." << std::endl;
			wchar_t str[255] = { 0 };
			wstring ww;
			ww.assign(path.begin(), path.end());
			wsprintf(str, L"%s/Cam%d", ww.c_str(), index);
			CreateDirectory(str, NULL);
			//for(int i=0;i<100;++i)
				//std::this_thread::yield();
			char buffer[255];
			sprintf_s(buffer, "%s/Cam%d/Seq_%04d.pgd", path.c_str(), index, prefix);

			ofstream outFile(buffer, std::ios::binary);
			outFile.write((char*)&endFrame, sizeof(int));
			int startFrame = 0;
			outFile.write((char*)&startFrame, sizeof(int));
			auto tmp = (*seqRaw)[0].GetCols();
			outFile.write((char*)&(tmp), sizeof(unsigned int));
			tmp = (*seqRaw)[0].GetRows();
			outFile.write((char*)&(tmp), sizeof(unsigned int));
			tmp = (*seqRaw)[0].GetStride();
			outFile.write((char*)&(tmp), sizeof(unsigned int));
			auto tmp2 = (*seqRaw)[0].GetPixelFormat();
			outFile.write((char*)&(tmp2), sizeof(tmp2));
			auto tmp3 = (*seqRaw)[0].GetBayerTileFormat();
			outFile.write((char*)&(tmp3), sizeof(tmp3));
			tmp = (*seqRaw)[0].GetDataSize();
			outFile.write((char*)&(tmp), sizeof(unsigned int));

			for (int j = 0; j < endFrame; ++j) {
				if ((*seqRaw)[j].GetRows() > 0) {
					auto datas = (*seqRaw)[j].GetData();
					outFile.write((char*)datas, tmp);
				}

			}
			outFile.close();
			cout << buffer << " Saveed" << endl;
			sprintf_s(buffer, "%s/Cam%d/TimeStamp_%04d.txt", path.c_str(), index, prefix);


			outFile.open(buffer, ofstream::out);
			//cout << index << " : " << mode << endl;
			outFile << "index : " << index << " \n";
			for (int i = 0; i < endFrame; ++i) {
				ImageMetadata metaData = (*seqRaw)[i].GetMetadata();
				outFile << i << " : " << (*timeArray)[i].microSeconds << " ";
				if (i > 0) {
					int currTIme = (*timeArray)[i].microSeconds;
					int prevTIme = (*timeArray)[i - 1].microSeconds;
					int interval = currTIme - prevTIme;
					if (interval < 0) {
						interval = 1000000 + interval;
					}

					outFile << 1.0E+6 / (double)interval << "    " << metaData.embeddedFrameCounter << "\n";
				}
				else {
					outFile << "0.0" << "    " << metaData.embeddedFrameCounter << "\n";
				}
			}

			outFile.close();
			cout << buffer << " Saved" << endl;
			ManagerCurrentCount(3, 0);
			CameraManager::GetInstance()->GetQForm()->ButtonOn();
			SettingBeforeCapture(1);
			//if (view->isUpdating) {

			//	return 0;
			//}
			//view->isUpdating = true;
			//(*seqRaw)[nowFrame].Convert(PixelFormat::PIXEL_FORMAT_RGB8, &rgb);
			//view->setBuffer(rgb.GetData(), rgb.GetDataSize(), rgb.GetCols(), rgb.GetRows());
			//return 0;
			});
			th.detach();
		}
	}

	//if(false)
	if(endFrame==0)
	{
		bool isUp = view->isUpdating;
		if (isUp) {
			return 0;
		}
		std::thread th([=](FlyCapture2::Image* rgb) {
			CameraManager::GetInstance()->mtx.lock();
			view->isUpdating = true;
			CameraManager::GetInstance()->mtx.unlock();
			imgBuffer.Convert(PixelFormat::PIXEL_FORMAT_RGB8, rgb);
			view->setBuffer(rgb->GetData(), rgb->GetDataSize(), rgb->GetCols(), rgb->GetRows());
			
		},&rgb);
		th.detach();
	}
	
	

	return 0;
}

int PointGrayCamera::GetDataFromBuffer()
{
	return 0;
}

int PointGrayCamera::SaveFIleSeq()
{
	return 0;
}

int PointGrayCamera::SaveFile()
{
	return 0;
}

int PointGrayCamera::fire(int prefix)
{
	prefixNum = prefix;

	if (isContinuous) {
		CameraStats camst;
		cam.GetStats(&camst);
		FireSoftwareTrigger();
		Image img;
		error = cam.RetrieveBuffer(&img);
		Image rgb;
		img.Convert(PixelFormat::PIXEL_FORMAT_RGB8, &rgb);
		TimeStamp time;
		time = img.GetTimeStamp();
		auto dSec = time.seconds - beforeTime.seconds;
		auto dMic = time.microSeconds - beforeTime.microSeconds;

		qout << "delta Time : " << dSec << " microsec : " << dMic << endl;
		int fc = 0;
		if (fileCount > 0) {
			fc = fileCount;
			fileCount--;
		}
		view->setBuffer(rgb.GetData(), rgb.GetDataSize(), rgb.GetCols(), rgb.GetRows());
		if (fc > 0) {
			int fileIdx = startCount - fc;


			//PNGOption pngOp;
			BMPOption bmpOp;
			//pngOp.compressionLevel = 0;
			char buffer[256];
			sprintf_s(buffer, "Cam%dcolor%04d.bmp", index, prefixNum);
			//img->Save(buffer, &pngOp);

			rgb.Save(buffer, &bmpOp);
		}
		beforeTime = time;
	}
	else {
		singleCap = true;
	}
	startTrigger = false;
	return 0;
}

int PointGrayCamera::fireSeq(int prefix, int frame)
{



	std::thread th([=]() {
		
		int quo = frame / 256;
		int mod = frame % 256;
		int mode = ofstream::out;
		ImageMetadata count;
		TimeStamp time;

		timeArray->resize(frame);
		seqRaw->resize(frame);
		Image Img;
		cam.WaitForBufferEvent(&Img, 1);
		for (int i = 0; i < frame; ++i) {
			if (i % 256 == 0) {
				if (quo > 0) {
					if (index == 0) {
						TriggerMode triggerMode;
						cam.GetTriggerMode(&triggerMode);
						triggerMode.parameter = 256;
						cout << "Set Triggdser Frame :" << 256 << endl;
						cam.SetTriggerMode(&triggerMode);
						FireSoftwareTrigger();
						mode |= ofstream::app;
					}

				}
				else if (quo == 0) {
					if (index == 0) {
						TriggerMode triggerMode;
						cam.GetTriggerMode(&triggerMode);
						triggerMode.parameter = mod;
						cout << "Set Triggdser Frame :" << mod << endl;
						cam.SetTriggerMode(&triggerMode);
						FireSoftwareTrigger();
						mode |= ofstream::app;
					}
				}
				quo--;
			}

			//Image img;
			error = cam.RetrieveBuffer(&((*seqRaw)[i]));
			if (error != PGRERROR_OK) {
				cout << "Cannot Get Buffer Image" << endl;
				CameraManager::PrintError(error);
			}
			count = (*seqRaw)[i].GetMetadata();
			time = (*seqRaw)[i].GetTimeStamp();
			(*timeArray)[i] = time;
			//cout << "Cam " << index << " : " << time.microSeconds << endl;
			Image rgb;
			(*seqRaw)[i].Convert(PixelFormat::PIXEL_FORMAT_RGB8, &rgb);

			view->setBuffer(rgb.GetData(), rgb.GetDataSize(), rgb.GetCols(), rgb.GetRows());
			//if (fc > 0 && false) {
			//	int fileIdx = startCount - fc;


			//	//PNGOption pngOp;
			//	BMPOption bmpOp;
			//	//pngOp.compressionLevel = 0;
			//	char buffer[255];
			//	sprintf_s(buffer, "Cam%dcolor%04d.bmp", index, fileIdx);
			//	//img->Save(buffer, &pngOp);
			//	rgb.Save(buffer, &bmpOp);
			//}
			cout<<".";

		}
		CameraManager::mtx.lock();
		char buffer[255];
		sprintf_s(buffer, "Cam%d_Seq%04d.pgd", index, prefix);
		ofstream outFile(buffer, std::ios::binary);
		outFile.write((char*)&frame, sizeof(int));
		int startFrame = 0;
		outFile.write((char*)&startFrame, sizeof(int));
		auto tmp = (*seqRaw)[0].GetCols();
		outFile.write((char*)&(tmp), sizeof(unsigned int));
		tmp = (*seqRaw)[0].GetRows();
		outFile.write((char*)&(tmp), sizeof(unsigned int));
		tmp = (*seqRaw)[0].GetStride();
		outFile.write((char*)&(tmp), sizeof(unsigned int));
		auto tmp2 = (*seqRaw)[0].GetPixelFormat();
		outFile.write((char*)&(tmp2), sizeof(tmp2));
		auto tmp3 = (*seqRaw)[0].GetBayerTileFormat();
		outFile.write((char*)&(tmp3), sizeof(tmp3));
		tmp = (*seqRaw)[0].GetDataSize();
		outFile.write((char*)&(tmp), sizeof(unsigned int));
		try {
			for (int j = 0; j < frame; ++j) {

				auto datas = (*seqRaw)[j].GetData();
				outFile.write((char*)datas, tmp);

			}
		}
		catch (...) {
			cout<<"FIle Write error!"<<endl;
		}

		outFile.close();

		outFile.open("timeStamp.txt", mode);
		//cout << index << " : " << mode << endl;
		outFile << "index : " << index << " \n";
		for (int i = 0; i < frame; ++i) {
			outFile << i << " : " << (*timeArray)[i].microSeconds << " ";
			if (i > 0) {
				int currTIme = (*timeArray)[i].microSeconds;
				int prevTIme = (*timeArray)[i - 1].microSeconds;
				int interval = currTIme - prevTIme;
				if (interval < 0) {
					interval = 1000000 + interval;
				}
				outFile << 1.0E+6 / (double)interval << "\n";
			}
			else {
				outFile << "0.0 \n";
			}
		}

		outFile.close();
		CameraManager::mtx.unlock();
	});
	th.detach();
	return 0;
}

int PointGrayCamera::setFileFrame(int frame)
{
	fileCount = frame;
	startCount = frame;
	return 0;
}

int PointGrayCamera::fileCapture(int frame)
{

	fire(frame);
	return 0;
}

int PointGrayCamera::Connect()
{
	if (!cam.IsConnected()) {
		cam.Connect(&guid);
	}

	return 0;
}

int PointGrayCamera::SetTriggerMode(bool onoff)
{
	isContinuous = onoff;
	if (index == 0) {
		TriggerMode triggerMode;
		cam.GetTriggerMode(&triggerMode);
		triggerMode.onOff = isContinuous;
		cam.SetTriggerMode(&triggerMode);
	}
	return 0;
}
void PointGrayCamera::Callback(Image * pImg, const void * pData) {
	PointGrayCamera* instance = (PointGrayCamera*)pData;
	instance->img.DeepCopy(pImg);
	auto pCamMgr = CameraManager::GetInstance();
	if (pCamMgr->getIsCapture() == true) {
		int idx = instance->ManagerCurrentCount(0, 0);
		int dd = instance->ManagerCurrentCount(1, 1);
		if (idx <= pCamMgr->GetNumFrames()) {
			(*(instance->seqRaw))[idx].DeepCopy(pImg);
		}
		printf("Cam %d CallBack : %d\n", instance->index, idx);

	}
}


int PointGrayCamera::FireSoftwareTrigger()
{
	const unsigned int k_softwareTrigger = 0x62C;
	const unsigned int k_fireVal = 0x80000000;
	Error error;
	error = cam.WriteRegister(k_softwareTrigger, k_fireVal);
	if (error != PGRERROR_OK)
	{
		CameraManager::PrintError(error);
		return -2;
	}
	return 0;
}

int PointGrayCamera::GetWB(float & valueR, float & valueB, bool & isOn, bool & isAuto, bool & isabsContorl)
{
	Property Prop;
	Prop.type = WHITE_BALANCE;
	error = cam.GetProperty(&Prop);
	ErrorCheck(-13);
	isOn = Prop.onOff;
	isAuto = Prop.autoManualMode;
	isabsContorl = Prop.absControl;
	valueR = Prop.valueB;
	valueB = Prop.valueA;
	return 0;
}
int PointGrayCamera::SetWB(float & valueR, float & valueB, bool & isOn, bool & isAuto, bool & isabsContorl)
{
	Property Prop;
	Prop.type = WHITE_BALANCE;
	error = cam.GetProperty(&Prop);
	ErrorCheck(-14);
	Prop.onOff = isOn;
	Prop.autoManualMode = isAuto;
	Prop.absControl = isabsContorl;
	Prop.valueB = valueR;
	Prop.valueA = valueB;
	cam.SetProperty(&Prop);
	return 0;
}


int PointGrayCamera::GetProperty(PropertyType type, float & value, bool & isOn, bool & isAuto, bool & isabsContorl)
{
	Property Prop;
	Prop.type = type;
	error = cam.GetProperty(&Prop);
	ErrorCheck(-15);
	isOn = Prop.onOff;
	isAuto = Prop.autoManualMode;
	isabsContorl = Prop.absControl;
	value = Prop.absValue;
	return 0;
}
int PointGrayCamera::GetProperty(PropertyType type, float & value, bool & isOn, bool & isAuto, bool & isabsContorl, int valueSet)
{
	Property Prop;
	Prop.type = type;
	error = cam.GetProperty(&Prop);
	ErrorCheck(-16);
	isOn = Prop.onOff;
	isAuto = Prop.autoManualMode;
	isabsContorl = Prop.absControl;
	if (valueSet == 0)
		value = Prop.absValue;
	else if (valueSet == 1)
		value = Prop.valueA;
	else if (valueSet == 2)
		value = Prop.valueB;
	return 0;
}

int PointGrayCamera::SetProperty(PropertyType type, float value, bool isOn, bool isAuto, bool isabsContorl, int valueSet)
{

	Property Prop;
	Prop.type = type;
	error = cam.GetProperty(&Prop);
	ErrorCheck(-17);
	Prop.onOff = isOn;
	Prop.autoManualMode = isAuto;
	Prop.absControl = isabsContorl;
	if (type == PropertyType::SHUTTER) {
		if (index == 0)
		{
			Prop.absValue = value; //33;
		}
		else
		{
			Prop.absValue = value;// -0.3f;
		}
	}
	else {

		if (valueSet == 0)
			Prop.absValue = value;
		else if (valueSet == 1)
			Prop.valueA = value;
		else if (valueSet == 2)
			Prop.valueB = value;
	}
	//error = cam.SetProperty(&Prop);
	error = cam.SetProperty(&Prop);
	return 0;
}

PointGrayCamera::PointGrayCamera()
{
	deltaFrameNum = 0;
	MissedNum = 0;
	timeArray = nullptr;
	seqRaw = nullptr;
	view = nullptr;
}


PointGrayCamera::~PointGrayCamera()
{
	if(timeArray !=nullptr)
		delete timeArray;
	if (seqRaw != nullptr)
		delete seqRaw;
	if (view != nullptr)
		delete this->view;
	std::cout << "~PointGrayCam" << std::endl;
}
