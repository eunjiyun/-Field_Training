#include "QtWidgetsApplication1.h"
#include<iostream>
//moc : 
// 소스 코드 -> .obj ->컴파일->링크(기계어로 번역)
//0bj파일로 exe파일 생성
//중간 단계에 moc_ ~~~ .obj ==>>중간 단계를 한번더 거친다.
//문제? 컴파일 속도가 느리다.
QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //어떤 함수에서 어떻게 처리할지
    //connect(ui.pushButton, &QPushButton::clicked, this);
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{}

void QtWidgetsApplication1::test()
{
    std::cout << "hello" << std::endl;
}