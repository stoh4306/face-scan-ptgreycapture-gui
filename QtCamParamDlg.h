#pragma once
#include<Common.h>
#include<QtWidgets/QDialog>
#include"ui_CameraParameter.h"
class CameraManager;
class QtPointGreyCaptureGui;
class QtCamParamDlg :
	public QDialog
{
private:
	Ui::QtCamParamDlgClass ui;
	Q_OBJECT
protected:
	QtPointGreyCaptureGui* parents;
	CameraManager* camMgr;
public:
	Q_SLOT void BrightnessSlider();
	Q_SLOT void ExposureSlider();
	Q_SLOT void SharpnessSlider();
	Q_SLOT void HueSlider();
	Q_SLOT void SaturationSlider();
	Q_SLOT void GammaSlider();
	Q_SLOT void ShutterSlider();
	Q_SLOT void GainSlider();
	Q_SLOT void FrameRateSlider();
	Q_SLOT void WBRSlider();
	Q_SLOT void WBBSlider();

	Q_SLOT void BrightnessBox();
	Q_SLOT void ExposureBox();
	Q_SLOT void SharpnessBox();
	Q_SLOT void HueBox();
	Q_SLOT void SaturationBox();
	Q_SLOT void GammaBox();
	Q_SLOT void ShutterBox();
	Q_SLOT void GainBox();
	Q_SLOT void FrameRateBox();
	Q_SLOT void WBRBox();
	Q_SLOT void WBBBox();

	
public:
	void dataUpdate(bool toVal=false, QString exception="");
	QtCamParamDlg(QWidget *parent = Q_NULLPTR);
	virtual ~QtCamParamDlg();
};

