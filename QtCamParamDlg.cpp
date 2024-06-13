#include "QtCamParamDlg.h"
#include "QtPointGreyCaptureGui.h"
#include"CameraManager.h"

void QtCamParamDlg::dataUpdate(bool toVal,QString exception)
{
	CamPropertyData& brightness = parents->brightness;
	CamPropertyData& exposure = parents->exposure;
	CamPropertyData& sharpness = parents->sharpness;
	CamPropertyData& hue = parents->hue;
	CamPropertyData& saturation = parents->saturation;
	CamPropertyData& gamma = parents->gamma;
	CamPropertyData& shutter = parents->shutter;
	CamPropertyData& gain = parents->gain;
	CamPropertyData& framerate = parents->framerate;
	CamPropertyData& WBRed = parents->WBRed;
	CamPropertyData& WBBlue = parents->WBBlue;
	if (toVal) {
		brightness.value = ui.BrightnessBox->value();

		exposure.value = ui.exposureBox->value();

		exposure.isAuto = ui.exposureAuto->isChecked();
		exposure.onOff = ui.exposureOnOff->isChecked();
		exposure.onePush = ui.exposureOnePush->isChecked();

		sharpness.value = ui.sharpnessBox->value();

		hue.value = ui.hueBox->value();
		saturation.value = ui.saturationBox->value();
		gamma.value = ui.gammaBox->value();
		gamma.onOff = ui.gammaOnOff->isChecked();

		shutter.value = ui.shutterBox->value();
		shutter.isAuto = ui.shutterAuto->isChecked();
		shutter.onePush = ui.shutterOnePush->isChecked();

		gain.value = ui.gainBox->value();
		gain.isAuto = ui.gainAuto->isChecked();
		gain.onePush = ui.gainOnePush->isChecked();


		framerate.value = 1000 / shutter.value;
		//framerate.value = ui.frameRateBox->value();

		framerate.isAuto = ui.frameRateAuto->isChecked();
		framerate.onOff = ui.frameRateOnOff->isChecked();

		WBRed.value = ui.WBRedBox->value();
		WBBlue.value = ui.WBBlueBox->value();
		WBRed.isAuto = ui.WBAuto->isChecked();
		WBRed.onOff = ui.WBOnOff->isChecked();
		WBRed.onePush = ui.WBOnePush->isChecked();

	}
	else {
		if(exception != "BrightnessBox")
			ui.BrightnessBox->setValue(brightness.value);
		double tmp = (brightness.value - ui.BrightnessBox->minimum()) / (ui.BrightnessBox->maximum() - ui.BrightnessBox->minimum()) * 1000;
		if (exception != "BrightnessSlider")
		ui.BrightnessSlider->setValue(tmp);
		
		ui.exposureBox->setValue(exposure.value);
		tmp = (exposure.value - ui.exposureBox->minimum()) / (ui.exposureBox->maximum() - ui.exposureBox->minimum()) * 1000;
		if (exception != "exposureSlider")
		ui.exposureSlider->setValue(tmp);

		ui.exposureAuto->setChecked(exposure.isAuto);
		ui.exposureOnOff->setChecked(exposure.onOff);
		ui.exposureOnePush->setChecked(exposure.onePush);

		ui.sharpnessBox->setValue(sharpness.value);
		tmp = (sharpness.value - ui.sharpnessBox->minimum()) / (ui.sharpnessBox->maximum() - ui.sharpnessBox->minimum()) * 1000;
		if (exception != "sharpnessSlider")
		ui.sharpnessSlider->setValue(tmp);


		ui.hueBox->setValue(hue.value);
		tmp = (hue.value - ui.hueBox->minimum()) / (ui.hueBox->maximum() - ui.hueBox->minimum()) * 1000;
		if (exception != "hueSlider")
		ui.hueSlider->setValue(tmp);

		ui.saturationBox->setValue(saturation.value);
		tmp = (saturation.value - ui.saturationBox->minimum()) / (ui.saturationBox->maximum() - ui.saturationBox->minimum()) * 1000;
		if (exception != "saturationSlider")
		ui.saturationSlider->setValue(tmp);

		ui.gammaBox->setValue(gamma.value);
		tmp = (gamma.value - ui.gammaBox->minimum()) / (ui.gammaBox->maximum() - ui.gammaBox->minimum()) * 1000;
		if (exception != "gammaSlider")
		ui.gammaSlider->setValue(tmp);

		ui.gammaOnOff->setChecked(gamma.onOff);

		ui.shutterBox->setValue(shutter.value);
		tmp = (shutter.value - ui.shutterBox->minimum()) / (ui.shutterBox->maximum() - ui.shutterBox->minimum()) * 1000;
		if (exception != "shutterSlider")
			ui.shutterSlider->setValue(tmp);

		ui.shutterAuto->setChecked(shutter.isAuto);
		ui.shutterOnePush->setChecked(shutter.onePush);

		ui.gainBox->setValue(gain.value);
		tmp = (gain.value - ui.gainBox->minimum()) / (ui.gainBox->maximum() - ui.gainBox->minimum()) * 1000;
		if (exception != "gainSlider")
			ui.gainSlider->setValue(tmp);

		ui.gainAuto->setChecked(gain.isAuto);
		ui.gainOnePush->setChecked(gain.onePush);

		//framerate.value = 1000 / shutter.value;

		ui.frameRateBox->setValue(framerate.value);
		tmp = (framerate.value - ui.frameRateBox->minimum()) / (ui.frameRateBox->maximum() - ui.frameRateBox->minimum()) * 1000;
		if (exception != "frameRateSlider")
			ui.frameRateSlider->setValue(tmp);

		ui.frameRateAuto->setChecked(framerate.isAuto);
		ui.frameRateOnOff->setChecked(framerate.onOff);

		ui.WBRedBox->setValue(WBRed.value);
		tmp = (WBRed.value - ui.WBRedBox->minimum()) / (ui.WBRedBox->maximum() - ui.WBRedBox->minimum()) * 1000;
		if (exception != "WBRedSlider")
			ui.WBRedSlider->setValue(tmp);

		ui.WBBlueBox->setValue(WBBlue.value);
		tmp = (WBBlue.value - ui.WBBlueBox->minimum()) / (ui.WBBlueBox->maximum() - ui.WBBlueBox->minimum()) * 1000;
		if (exception != "WBBlueSlider")
			ui.WBBlueSlider->setValue(tmp);

		ui.WBAuto->setChecked(WBRed.isAuto);
		ui.WBOnOff->setChecked(WBRed.onOff);
		ui.WBOnePush->setChecked(WBRed.onePush);
	}
}

QtCamParamDlg::QtCamParamDlg(QWidget *parent) : QDialog(parent), parents((QtPointGreyCaptureGui*)parent)
{
	ui.setupUi(this);

	camMgr = parents->GetCamMgr();
	parents->getProperties();
	dataUpdate();
}


QtCamParamDlg::~QtCamParamDlg()
{
}

Q_SLOT void QtCamParamDlg::BrightnessSlider()
{	
	
		double min, max, val;
		double  tmp;
		min = ui.BrightnessBox->minimum();
		val = ui.BrightnessSlider->value();
		max = ui.BrightnessBox->maximum();
		tmp = min + (0.001f*val*(max - min));
		parents->brightness.value = tmp;
		ui.BrightnessBox->setValue(tmp);
		for (int i = 0; i < camMgr->GetCameraCount(); ++i) {
		camMgr->SetProperty(i, PropertyType::BRIGHTNESS, tmp);
	}
		dataUpdate(false,"BrightnessSlider");
	return Q_SLOT void();
}

Q_SLOT void QtCamParamDlg::ExposureSlider()
{
	
		double min, max, val;
		double  tmp;
		min = ui.exposureBox->minimum();
		val = ui.exposureSlider->value();
		max = ui.exposureBox->maximum();
		tmp = min + (0.001f*val*(max - min));
		parents->exposure.value = tmp;
		ui.exposureBox->setValue(tmp);
		for (int i = 0; i < camMgr->GetCameraCount(); ++i) {
		camMgr->SetProperty(i, PropertyType::AUTO_EXPOSURE, tmp);
	}
		dataUpdate(false, "exposureSlider");
	return Q_SLOT void();
}

Q_SLOT void QtCamParamDlg::SharpnessSlider()
{
	
		double min, max, val;
		double  tmp;
		min = ui.sharpnessBox->minimum();
		val = ui.sharpnessSlider->value();
		max = ui.sharpnessBox->maximum();
		tmp = min + (0.001f*val*(max - min));
		parents->sharpness.value = tmp;
		ui.sharpnessBox->setValue(tmp);
		for (int i = 0; i < camMgr->GetCameraCount(); ++i) {
		camMgr->SetProperty(i, PropertyType::SHARPNESS, tmp);
	}
		dataUpdate(false, "sharpnessSlider");
	return Q_SLOT void();
}

Q_SLOT void QtCamParamDlg::HueSlider()
{
	
		double min, max, val;
		double  tmp;
		min = ui.hueBox->minimum();
		val = ui.hueSlider->value();
		max = ui.hueBox->maximum();
		tmp = min + (0.001f*val*(max - min));
		parents->hue.value = tmp;
		ui.hueBox->setValue(tmp);
		for (int i = 0; i < camMgr->GetCameraCount(); ++i) {
		camMgr->SetProperty(i, PropertyType::HUE, tmp);
	}
		dataUpdate(false, "hueSlider");
	return Q_SLOT void();
}

Q_SLOT void QtCamParamDlg::SaturationSlider()
{
	
		double min, max, val;
		double  tmp;
		min = ui.saturationBox->minimum();
		val = ui.saturationSlider->value();
		max = ui.saturationBox->maximum();
		tmp = min + (0.001f*val*(max - min));
		parents->saturation.value = tmp;
		ui.saturationBox->setValue(tmp);
		for (int i = 0; i < camMgr->GetCameraCount(); ++i) {
		camMgr->SetProperty(i, PropertyType::SATURATION, tmp);
	}
		dataUpdate(false, "saturationSlider");
	return Q_SLOT void();
}

Q_SLOT void QtCamParamDlg::GammaSlider()
{
	
		double min, max, val;
		double  tmp;
		min = ui.gammaBox->minimum();
		val = ui.gainSlider->value();
		max = ui.gammaBox->maximum();
		tmp = min + (0.001f*val*(max - min));
		parents->gamma.value = tmp;
		ui.gammaBox->setValue(tmp);
		for (int i = 0; i < camMgr->GetCameraCount(); ++i) {
		camMgr->SetProperty(i, PropertyType::GAMMA, tmp);
	}
		dataUpdate(false, "gainSlider");
	return Q_SLOT void();
}

Q_SLOT void QtCamParamDlg::ShutterSlider()
{
	
		double min, max, val;
		double  tmp;
		min = ui.shutterBox->minimum();
		val = ui.shutterSlider->value();
		max = ui.shutterBox->maximum();
		tmp = min + (0.001f*val*(max - min));
		parents->shutter.value = tmp;
		ui.shutterBox->setValue(tmp);
		for (int i = 0; i < camMgr->GetCameraCount(); ++i) {
			camMgr->SetProperty(i, PropertyType::SHUTTER, tmp);
		}
		dataUpdate(false, "shutterSlider");
		parents->NosChanged();
	return Q_SLOT void();
}

Q_SLOT void QtCamParamDlg::GainSlider()
{
	
		double min, max, val;
		double  tmp;
		min = ui.gainBox->minimum();
		val = ui.gainSlider->value();
		max = ui.gainBox->maximum();
		tmp = min + (0.001f*val*(max - min));
		parents->gain.value = tmp;
		ui.gainBox->setValue(tmp);
		for (int i = 0; i < camMgr->GetCameraCount(); ++i) {
		camMgr->SetProperty(i, PropertyType::GAIN, tmp);
	}
		dataUpdate(false, "gainSlider");
	return Q_SLOT void();
}

Q_SLOT void QtCamParamDlg::FrameRateSlider()
{
	
		double min, max, val;
		double  tmp;
		min = ui.frameRateBox->minimum();
		val = ui.frameRateSlider->value();
		max = ui.frameRateBox->maximum();
		tmp = min + (0.001f*val*(max - min));
		parents->framerate.value = tmp;
		ui.frameRateBox->setValue(tmp);
		for (int i = 0; i < camMgr->GetCameraCount(); ++i) {
		camMgr->SetProperty(i, PropertyType::FRAME_RATE, tmp);
	}
		dataUpdate(false, "frameRateSlider");
	return Q_SLOT void();
}

Q_SLOT void QtCamParamDlg::WBRSlider()
{
	
		double min, max, val;
		double  tmp;
		min = ui.WBRedBox->minimum();
		val = ui.WBRedSlider->value();
		max = ui.WBRedBox->maximum();
		tmp = min + (0.001f*val*(max - min));
		parents->WBRed.value = tmp;
		ui.WBRedBox->setValue(tmp);
		for (int i = 0; i < camMgr->GetCameraCount(); ++i) {
		camMgr->SetWhteBalance(i, parents->WBRed.value, parents->WBBlue.value, parents->WBRed.onOff, parents->WBRed.isAuto, parents->WBRed.onePush);
	}
		dataUpdate(false, "WBRedSlider");
	return Q_SLOT void();
}

Q_SLOT void QtCamParamDlg::WBBSlider()
{
	
		double min, max, val;
		double  tmp;
		min = ui.WBBlueBox->minimum();
		val = ui.WBBlueSlider->value();
		max = ui.WBBlueBox->maximum();
		tmp = min + (0.001f*val*(max - min));
		parents->WBBlue.value = tmp;
		ui.WBBlueBox->setValue(tmp);
		for (int i = 0; i < camMgr->GetCameraCount(); ++i) {
		camMgr->SetWhteBalance(i, parents->WBRed.value, parents->WBBlue.value, parents->WBRed.onOff, parents->WBRed.isAuto, parents->WBRed.onePush);
	}
		dataUpdate(false, "WBBlueSlider");
	return Q_SLOT void();
}

Q_SLOT void QtCamParamDlg::BrightnessBox()
{
	auto& box = ui.BrightnessBox;
	double value = box->value();
	double tmp = (value - box->minimum()) / (box->maximum() - box->minimum()) * 1000;
	parents->brightness.value = value;
	ui.BrightnessSlider->setValue(tmp);
	for (int i = 0; i < camMgr->GetCameraCount(); ++i) {
		camMgr->SetProperty(i, PropertyType::BRIGHTNESS, value);
	}
	dataUpdate();
	return Q_SLOT void();
}

Q_SLOT void QtCamParamDlg::ExposureBox()
{
	auto& box = ui.exposureBox;
	double value = box->value();
	double tmp = (value - box->minimum()) / (box->maximum() - box->minimum()) * 1000;
	parents->exposure.value = value;
	ui.exposureSlider->setValue(tmp);
	for (int i = 0; i < camMgr->GetCameraCount(); ++i) {
		camMgr->SetProperty(i, PropertyType::AUTO_EXPOSURE, value);
	}
	dataUpdate();
	return Q_SLOT void();
}

Q_SLOT void QtCamParamDlg::SharpnessBox()
{
	auto& box = ui.sharpnessBox;
	double value = box->value();
	double tmp = (value - box->minimum()) / (box->maximum() - box->minimum()) * 1000;
	parents->sharpness.value = value;
	ui.sharpnessSlider->setValue(tmp);
	for (int i = 0; i < camMgr->GetCameraCount(); ++i) {
		camMgr->SetProperty(i, PropertyType::SHARPNESS, value);
	}
	dataUpdate();
	return Q_SLOT void();
}

Q_SLOT void QtCamParamDlg::HueBox()
{
	auto& box = ui.hueBox;
	double value = box->value();
	double tmp = (value - box->minimum()) / (box->maximum() - box->minimum()) * 1000;
	parents->hue.value = value;
	ui.hueSlider->setValue(tmp);
	for (int i = 0; i < camMgr->GetCameraCount(); ++i) {
		camMgr->SetProperty(i, PropertyType::HUE, value);
	}
	dataUpdate();
	return Q_SLOT void();
}

Q_SLOT void QtCamParamDlg::SaturationBox()
{
	auto& box = ui.saturationBox;
	double value = box->value();
	double tmp = (value - box->minimum()) / (box->maximum() - box->minimum()) * 1000;
	parents->saturation.value = value;
	ui.saturationSlider->setValue(tmp);
	for (int i = 0; i < camMgr->GetCameraCount(); ++i) {
		camMgr->SetProperty(i, PropertyType::SATURATION, value);
	}
	dataUpdate();
	return Q_SLOT void();
}

Q_SLOT void QtCamParamDlg::GammaBox()
{
	auto& box = ui.gammaBox;
	double value = box->value();
	double tmp = (value - box->minimum()) / (box->maximum() - box->minimum()) * 1000;
	parents->gamma.value = value;
	ui.gammaSlider->setValue(tmp);
	for (int i = 0; i < camMgr->GetCameraCount(); ++i) {
		camMgr->SetProperty(i, PropertyType::GAMMA, value);
	}
	dataUpdate();
	return Q_SLOT void();
}

Q_SLOT void QtCamParamDlg::ShutterBox()
{
	auto& box = ui.shutterBox;
	double value = box->value();
	double tmp = (value - box->minimum()) / (box->maximum() - box->minimum()) * 1000;
	parents->shutter.value = value;
	ui.shutterSlider->setValue(tmp);
	for (int i = 0; i < camMgr->GetCameraCount(); ++i) {
		camMgr->SetProperty(i, PropertyType::SHUTTER, value);
	}
	dataUpdate();
	parents->NosChanged();
	return Q_SLOT void();
}

Q_SLOT void QtCamParamDlg::GainBox()
{
	auto& box = ui.gainBox;
	double value = box->value();
	double tmp = (value - box->minimum()) / (box->maximum() - box->minimum()) * 1000;
	parents->gain.value = value;
	ui.gainSlider->setValue(tmp);
	for (int i = 0; i < camMgr->GetCameraCount(); ++i) {
		camMgr->SetProperty(i, PropertyType::GAIN, value);
	}
	dataUpdate();
	return Q_SLOT void();
}

Q_SLOT void QtCamParamDlg::FrameRateBox()
{
	auto& box = ui.frameRateBox;
	double value = box->value();
	double tmp = (value - box->minimum()) / (box->maximum() - box->minimum()) * 1000;
	parents->framerate.value = value;
	ui.frameRateSlider->setValue(tmp);
	for (int i = 0; i < camMgr->GetCameraCount(); ++i) {
		camMgr->SetProperty(i, PropertyType::FRAME_RATE, value);
	}
	dataUpdate();
	return Q_SLOT void();
}

Q_SLOT void QtCamParamDlg::WBRBox()
{
	auto& box = ui.WBRedBox;
	double value = box->value();
	double tmp = (value - box->minimum()) / (box->maximum() - box->minimum()) * 1000;
	parents->WBRed.value = value;
	ui.WBRedSlider->setValue(tmp);
	for (int i = 0; i < camMgr->GetCameraCount(); ++i) {
		camMgr->SetWhteBalance(i, parents->WBRed.value, parents->WBBlue.value, parents->WBRed.onOff, parents->WBRed.isAuto, parents->WBRed.onePush);
	}
	dataUpdate();
	return Q_SLOT void();
}

Q_SLOT void QtCamParamDlg::WBBBox()
{
	auto& box = ui.WBBlueBox;
	double value = box->value();
	double tmp = (value - box->minimum()) / (box->maximum() - box->minimum()) * 1000;
	parents->WBBlue.value = value;
	ui.WBBlueSlider->setValue(tmp);
	for (int i = 0; i < camMgr->GetCameraCount(); ++i) {
		camMgr->SetWhteBalance(i, parents->WBRed.value, parents->WBBlue.value, parents->WBRed.onOff, parents->WBRed.isAuto, parents->WBRed.onePush);
	}
	dataUpdate();
	return Q_SLOT void();
}
