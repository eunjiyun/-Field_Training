#include <QtWidgets/QApplication>
#include "QtWidgetsApplication1.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);//qt���ø����̼� �����
    QtWidgetsApplication1 w;//��ü�� ����

    w.show();//â�� ����
    return a.exec();
}
