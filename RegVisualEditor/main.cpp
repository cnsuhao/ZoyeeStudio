#include "regvisualeditor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RegVisualEditor w;
	w.SetInstance(&w);
	w.show();
	return a.exec();
}
