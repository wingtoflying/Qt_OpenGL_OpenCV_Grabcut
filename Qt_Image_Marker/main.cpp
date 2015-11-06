#include "qt_image_marker.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Qt_Image_Marker w;
	w.show();
	return a.exec();
}
