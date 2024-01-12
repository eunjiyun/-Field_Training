#include "QtWidgetsApplication1.h"
#include <QtWidgets/QApplication>


#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <iostream>
#include <QDebug>
#include <Qt3DRender/QMesh>
#include "In3DVTK_Def.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);//qt���ø����̼� �����
    QtWidgetsApplication1 w;//��ü�� ����
    w.show();//â�� ����
    return a.exec();
}
