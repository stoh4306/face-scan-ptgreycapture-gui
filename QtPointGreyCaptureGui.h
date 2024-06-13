#pragma once
#include"Common.h"
#include <QtWidgets/QMainWindow>
#include "ui_QtPointGreyCaptureGui.h"
#include"QtCamParamDlg.h"
#include<QtWidgets/qfiledialog>
#include<QtWidgets/qlineedit>
#include<QtCore/qprocess.h>

#define qout std::cout
#define qprintf printf
//#define puts QtPointGreyCaptureGui::qOut->puts
//#define qout std::cout
//#define qprintf printf
#define puts puts
class MyGlWidget;
class CameraManager;
class QStdStream;
class QtPointGreyCaptureGui : public QMainWindow
{
	Q_OBJECT

public:
	std::vector<MyGlWidget*> views;
	QtPointGreyCaptureGui(QWidget *parent = Q_NULLPTR);
	//PointGrey* pPointGrey;
	CameraManager* pCamManager;
	QtCamParamDlg* dlg;
	
	bool event(QEvent* event);
	void update();
	int init();
	virtual ~QtPointGreyCaptureGui();

	Q_SLOT void FireButton();
	Q_SLOT void SeqButton();
	Q_SLOT void OffButton();
	Q_SLOT void ChangedPreviewCheck(bool);

	Q_SLOT void OptionShow();
	Q_SLOT void Path();
	Q_SLOT void PathText();
	Q_SLOT void NosChanged();

	Q_SLOT void Redirect();
	
	Q_SLOT void SendData(float camX, float camY, float zoom);

	Q_SLOT void zoomEdit();
	Q_SLOT void AddCamListWidget(const QString& label);
	Q_SLOT void ViewOnlyCheckedView();
	Q_SLOT void SButtonOn();
	Q_SLOT void SButtonOff();
	Q_SIGNAL void ButtonOn();
	Q_SIGNAL void ButtonOff();
public:
	QGroupBox* viewBox;
	CameraManager* GetCamMgr() { return pCamManager; }
	static std::streambuf *psbuf, *backup;
private:
	QProcess* pQprocess;
	QStdStream* pQStream;
	QFileDialog* qfd;
	Ui::QtPointGreyCaptureGuiClass ui;
	
	static QPlainTextEdit* textBox;
public:
	CamPropertyData brightness;
	CamPropertyData exposure;
	CamPropertyData sharpness;
	CamPropertyData hue;
	CamPropertyData saturation;
	CamPropertyData gamma;
	CamPropertyData shutter;
	CamPropertyData gain;
	CamPropertyData framerate;
	CamPropertyData WBRed;
	CamPropertyData WBBlue;

	CamPropertyData Dbrightness;
	CamPropertyData Dexposure;
	CamPropertyData Dsharpness;
	CamPropertyData Dhue;
	CamPropertyData Dsaturation;
	CamPropertyData Dgamma;
	CamPropertyData Dshutter;
	CamPropertyData Dgain;
	CamPropertyData Dframerate;
	CamPropertyData DWBRed;
	CamPropertyData DWBBlue;

	void getProperties();
	void setProperties();
	inline bool viewState(int i) { return ui.CamListWidget->item(i)->checkState(); }
};
