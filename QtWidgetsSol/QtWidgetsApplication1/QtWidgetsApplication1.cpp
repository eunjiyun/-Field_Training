#include "QtWidgetsApplication1.h"
#include<iostream>
//moc : 
// �ҽ� �ڵ� -> .obj ->������->��ũ(����� ����)
//0bj���Ϸ� exe���� ����
//�߰� �ܰ迡 moc_ ~~~ .obj ==>>�߰� �ܰ踦 �ѹ��� ��ģ��.
//����? ������ �ӵ��� ������.
QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //� �Լ����� ��� ó������
    //connect(ui.pushButton, &QPushButton::clicked, this);
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{}

void QtWidgetsApplication1::test()
{
    std::cout << "hello" << std::endl;
}