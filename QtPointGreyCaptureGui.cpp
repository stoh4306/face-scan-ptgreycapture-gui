#include "QtPointGreyCaptureGui.h"
#include"CameraManager.h"
#include"FlyCapture2.h"
#include"FlyCapture2Defs.h"
#include"QStdStream.h"
#include"MyGlWidget.h"
#include<QtWidgets/qmessagebox.h>
#include<QtWidgets/qsizepolicy.h>

std::streambuf *QtPointGreyCaptureGui::psbuf, *QtPointGreyCaptureGui::backup;
QPlainTextEdit* QtPointGreyCaptureGui::textBox;
QtPointGreyCaptureGui::QtPointGreyCaptureGui(QWidget *parent)
	: QMainWindow(parent)
{
	dlg = nullptr;
	pQStream = nullptr;
	
	
	//ui.LeftCamWidget->
	ui.setupUi(this);
	textBox = ui.LogText;
	//pCamManager->GetProperty(0, PropertyType::WHITE_BALANCE, brightness);
	//brightness

	
	//pPointGrey->runCam();

	
	

}

void QtPointGreyCaptureGui::FireButton()
{
	
	QLineEdit le;
	
	int preFix = ui.filePrefixText->text().toInt();
	ui.DirectoryText->text();
	ButtonOff();
	pCamManager->SetParam(1,preFix);
	//for (int i = 0; i < pCamManager->GetCameraCount(); ++i) {
	//	//pCamManager->ConnectionCheck();
	//	pCamManager->FireCam(preFix);
	//}
	ui.filePrefixText->setText(QString::fromStdString(to_string(preFix + 1)));
	pQStream->flush();
	//pCamManager->stopCam();
}

Q_SLOT void QtPointGreyCaptureGui::SeqButton()
{
	int len = ui.CaptureFrameLen->text().toInt();
	int preFix = ui.seqPrefixText->text().toInt();
	ButtonOff();
	pCamManager->SetParam(len,preFix);

	//for (int i = 0; i < pCamManager->GetCameraCount(); ++i) {
	//	//pCamManager->ConnectionCheck();
	//	
	//}
	//pCamManager->FireCamSeq(preFix, len);
	ui.seqPrefixText->setText(QString::fromStdString(to_string(preFix + 1)));
	return Q_SLOT void();
}


Q_SLOT void QtPointGreyCaptureGui::OffButton()
{
	
	//if (ui.onOffButton->text() == "OFF")
	//	ui.onOffButton->setText("ON");
	//else
	//	ui.onOffButton->setText("OFF");
	//pCamManager->TriggerModeToggle();
	
	return Q_SLOT void();
}
Q_SLOT void QtPointGreyCaptureGui::ChangedPreviewCheck(bool isChecked)
{
	pCamManager->SetParam();
	return Q_SLOT void();
}
Q_SLOT void QtPointGreyCaptureGui::OptionShow()
{
	dlg->show();
	return Q_SLOT void();
}

Q_SLOT void QtPointGreyCaptureGui::Path()
{
	if(qfd==nullptr)
		qfd = new QFileDialog();
	auto openFile = qfd->getExistingDirectory(this, "Forder Select");
	if (openFile.length() > 2) {
		ui.DirectoryText->setText(openFile);
		pCamManager->SetPath(openFile.toStdString());
		cout << "Working Directory : " << openFile.toStdString();
	}
	
	
	return Q_SLOT void();
}

Q_SLOT void QtPointGreyCaptureGui::PathText()
{
	pCamManager->SetPath(ui.DirectoryText->text().toStdString());
	cout << "Working Directory : " << ui.DirectoryText->text().toStdString();
	return Q_SLOT void();
}

Q_SLOT void QtPointGreyCaptureGui::NosChanged()
{
	double numberOfShots  = ui.CaptureFrameLen->text().toDouble();
	numberOfShots *= (1.0/ framerate.value);
	char buffer[256];
	sprintf_s(buffer, "%0.3lf", numberOfShots);
	
	ui.DurationText->setText(buffer);
	return Q_SLOT void();
}

Q_SLOT void QtPointGreyCaptureGui::Redirect()
{
	QByteArray newData = pQprocess->readAllStandardError();
	QString text = QString::fromLocal8Bit(newData);
	ui.LogText->appendPlainText(text);
	return Q_SLOT void();
}

Q_SLOT void QtPointGreyCaptureGui::SendData(float camX, float camY, float zoom)
{
	ui.ZoomEdit->setText(QString::asprintf("%d", 10000 / (int)zoom));
	for (int i = 0; i < views.size(); ++i) {
		views[i]->setCam(camX, camY, zoom);
		views[i]->update();
	}
	return Q_SLOT void();
}

Q_SLOT void QtPointGreyCaptureGui::zoomEdit()
{
	for (int i = 0; i < views.size(); ++i) { views[i]->setZoom(10000/ui.ZoomEdit->text().toFloat()); }
	return Q_SLOT void();
}

Q_SLOT void QtPointGreyCaptureGui::AddCamListWidget(const QString& label)
{
	ui.CamListWidget->addItem(label);
	int i = ui.CamListWidget->count();
	auto item = ui.CamListWidget->item(i - 1);
	item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
	item->setCheckState(Qt::Checked);
	
	return Q_SLOT void();
}

Q_SLOT void QtPointGreyCaptureGui::ViewOnlyCheckedView()
{
	QListWidgetItem* item = 0;
	for (int i = 0; i < ui.CamListWidget->count(); ++i) {
		item = ui.CamListWidget->item(i);
		if (item->checkState()) {
			views[i]->show();
			views[i]->update();
		}
		else {
			views[i]->hide();
		}
		//item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		//item->setCheckState(Qt::Unchecked);
	}
	return Q_SLOT void();
}

Q_SLOT void QtPointGreyCaptureGui::SButtonOn()
{
	ui.pushButton->setEnabled(true);
	ui.SeqCapture->setEnabled(true);
	return Q_SLOT void();
}

Q_SLOT void QtPointGreyCaptureGui::SButtonOff()
{
	ui.pushButton->setEnabled(false);
	ui.SeqCapture->setEnabled(false);
	return Q_SLOT void();
}


void QtPointGreyCaptureGui::getProperties()
{
	pCamManager->GetProperty(0, PropertyType::BRIGHTNESS, brightness);
	pCamManager->GetProperty(0, PropertyType::AUTO_EXPOSURE, exposure);
	pCamManager->GetProperty(0, PropertyType::SHARPNESS, sharpness, 1);
	pCamManager->GetProperty(0, PropertyType::HUE, hue);
	pCamManager->GetProperty(0, PropertyType::SATURATION, saturation);
	pCamManager->GetProperty(0, PropertyType::GAMMA, gamma);
	pCamManager->GetProperty(0, PropertyType::SHUTTER, shutter);
	pCamManager->GetProperty(0, PropertyType::GAIN, gain);
	pCamManager->GetProperty(0, PropertyType::FRAME_RATE, framerate);
	pCamManager->GetWhteBalance(0, WBRed.value, WBBlue.value, WBRed.onOff, WBRed.isAuto, WBRed.onePush);
	NosChanged();
}

void QtPointGreyCaptureGui::setProperties()
{
	for (int i = 0; i < pCamManager->GetCameraCount(); ++i){
		pCamManager->SetProperty(i, PropertyType::BRIGHTNESS, brightness);
		pCamManager->SetProperty(i, PropertyType::AUTO_EXPOSURE, exposure);
		pCamManager->SetProperty(i, PropertyType::SHARPNESS, sharpness, 1);
		pCamManager->SetProperty(i, PropertyType::HUE, hue);
		pCamManager->SetProperty(i, PropertyType::SATURATION, saturation);
		pCamManager->SetProperty(i, PropertyType::GAMMA, gamma);
		//pCamManager->SetProperty(i, PropertyType::SHUTTER, shutter);
		pCamManager->SetShutterVal(i, shutter);
		pCamManager->SetProperty(i, PropertyType::GAIN, gain);
		pCamManager->SetProperty(i, PropertyType::FRAME_RATE, framerate);
		pCamManager->SetWhteBalance(i, WBRed.value, WBBlue.value, WBRed.onOff, WBRed.isAuto, WBRed.onePush);
	}
}

bool QtPointGreyCaptureGui::event(QEvent * _event)
{
	if(pQStream!=nullptr)
		pQStream->flush();
	return QMainWindow::event(_event);
}

void QtPointGreyCaptureGui::update()
{
	
	QMainWindow::update();
}

int QtPointGreyCaptureGui::init()
{
	this->setUpdatesEnabled(true);
	//pQprocess = new QProcess(this);
	//connect(this, SIGNAL(readyReadStandardError()), this, SLOT(Redirect()));
	//connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(Redirect()));
	//connect(pQprocess, SIGNAL(readyReadStandardError()), this, SLOT(Redirect()));
	//connect(pQprocess, SIGNAL(readyReadStandardOutput()), this, SLOT(Redirect()));
	//pQprocess->start();
	backup = std::cout.rdbuf();     // back up cout's streambuf
	pQStream = new QStdStream(std::cout, ui.LogText);
	psbuf = pQStream;
	std::cout.rdbuf(psbuf);
	
	viewBox = ui.ViewBox;
	QLayout* viewBoxLayout = new QGridLayout(viewBox);
	viewBox->setLayout(viewBoxLayout);
	

	ui.CaptureFrameLen->setText("30");
	ui.CaptureFrameLen->setValidator(new QIntValidator(0, 9999, this));
	ui.filePrefixText->setValidator(new QIntValidator(0, 9999, this));
	ui.seqPrefixText->setValidator(new QIntValidator(0, 9999, this));
	ui.ZoomEdit->setValidator(new QIntValidator(1, 10000, this));
	ui.filePrefixText->setText("0");
	ui.seqPrefixText->setText("0");

	ui.DirectoryText->setText(".");
	
	qfd = nullptr;
	pCamManager = CameraManager::GetInstance();
	
	pCamManager->SetPath(".");
	auto errorCode = pCamManager->initCameras(this, viewBox);
	ui.centralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	if (errorCode < 0) {
		QMessageBox msgbox;
		int msg = QMessageBox::warning(this, tr("error"), tr("Can not find Camera!"),QMessageBox::Ok, QMessageBox::Ok);
		//msgbox.setText(QString("Can not find Camera.  check seiral.txt"));
		ui.pushButton->setEnabled(false);
		ui.pushButton_4->setEnabled(false);
		//ui.PreviewCheck->setEnabled(false);
		ui.pushButton_5->setEnabled(false);
		ui.SeqCapture->setEnabled(false);
		//ui.pushButton->setEnabled(false);
		delete pCamManager;
		return errorCode;
	}
	pCamManager->startCam();
	getProperties();	
	connect(this, SIGNAL(ButtonOn()), this, SLOT(SButtonOn()));
	connect(this, SIGNAL(ButtonOff()), this, SLOT(SButtonOff()));
	dlg = new QtCamParamDlg(this);

	return 0;
}

QtPointGreyCaptureGui::~QtPointGreyCaptureGui()
{
	std::cout.rdbuf(backup);
	delete pQStream;
	pQStream = nullptr;
	if(qfd!=nullptr)
		delete qfd;
	if(dlg!=nullptr)
		delete dlg;

	if (pCamManager != nullptr) {
		pCamManager->stopCam();
		pCamManager->Release();
	}
}
