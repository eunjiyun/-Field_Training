#include "QtWidgetsApplication1.h"
//moc : 
// �ҽ� �ڵ� -> .obj ->������->��ũ(����� ����)
//0bj���Ϸ� exe���� ����
//�߰� �ܰ迡 moc_ ~~~ .obj ==>>�߰� �ܰ踦 �ѹ��� ��ģ��.
//����? ������ �ӵ��� ������.
QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{}
