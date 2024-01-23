#include "QtWidgetsApplication1.h"
#include<iostream>
#include "In3DVTK_Def.h"
#include "In3DTestWidget.h"
#include "ScreenShot.h" 


//moc : 
// 소스 코드 -> .obj ->컴파일->링크(기계어로 번역)
//0bj파일로 exe파일 생성
//중간 단계에 moc_ ~~~ .obj ==>>중간 단계를 한번더 거친다.
//문제? 컴파일 속도가 느리다.
QtWidgetsApplication1::QtWidgetsApplication1(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	this->screenShot = new CScreenShot();
	
	// 메인 윈도우의 중앙 위젯을 설정합니다.
	QWidget* centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	// 중앙 위젯의 레이아웃을 설정합니다.
	QVBoxLayout* layout = new QVBoxLayout(centralWidget);
	layout->addWidget(this->screenShot);
	this->screenShot->setStyleSheet("background-color: steelblue;");
	this->resize(1200, 800); // 메인 윈도우의 크기를 너비 1200, 높이 800으로 조절
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{}
void QtWidgetsApplication1::colchan()
{
	ui.pushButton_2->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "changed!", nullptr));

	
	cl->SetNumberOfComponents(3);
	for (int i{}; i < this->widget->polyData->GetNumberOfPoints(); ++i)
		cl->InsertNextTuple3(0, 0, 255);


	// 클리핑된 폴리데이터에 매핑된 색상 정보를 설정합니다.
	this->widget->polyData->GetPointData()->SetScalars(cl);


	this->widget->renderWindow->Render();
	//interactor->Start();
}
void QtWidgetsApplication1::test()
{
	ui.pushButton->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "pressed", nullptr));

	this->widget->renderWindow->AddRenderer(this->widget->renderer);
	this->widget->renderWindow->SetWindowId((void*)this->widget->winId());
	
	this->widget->interactor->SetRenderWindow(this->widget->renderWindow);

	this->widget->renderWindow->Render();
	this->widget->interactor->Start();
}