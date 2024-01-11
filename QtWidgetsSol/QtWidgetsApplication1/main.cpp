#include "QtWidgetsApplication1.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);//qt어플리케이션 만들기
    QtWidgetsApplication1 w;//객체를 생성
    w.show();//창을 띄우기
    return a.exec();
}
