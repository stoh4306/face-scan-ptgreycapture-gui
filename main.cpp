#include "QtPointGreyCaptureGui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtPointGreyCaptureGui w;
	w.show();
	w.init();
	return a.exec();
}
