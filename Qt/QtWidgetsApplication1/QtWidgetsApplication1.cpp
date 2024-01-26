#include<iostream>
#include "vtkInteractorObserver.h"
#include "In3DVTK_Def.h"
#include "QtWidgetsApplication1.h"
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


	this->widget = new In3DTestWidget(this);
	this->widget->setGeometry(300, 100, 200, 200);
	this->widget->resize(900, 400);
	this->resize(1300, 700);
	this->widget->LoadTest();


	//어떤 함수에서 어떻게 처리할지
	connect(ui.pushButton, &QPushButton::clicked, this, &QtWidgetsApplication1::test);
	cl = vtkUnsignedCharArray::New();
	connect(ui.pushButton_2, &QPushButton::clicked, this, &QtWidgetsApplication1::colchan);

}

QtWidgetsApplication1::~QtWidgetsApplication1()
{
}
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
	CustomInteractor inst;

	this->widget->renderWindow->AddRenderer(this->widget->renderer);
	// CustomInteractor 인스턴스를 생성합니다.
	vtkSmartPointer<CustomInteractor> interactorInstance{ vtkSmartPointer<CustomInteractor>::New() };

	// interactorInstance를 interactor의 인터랙터 스타일로 설정합니다.
	this->widget->interactor->SetInteractorStyle(interactorInstance);


	this->widget->renderWindow->SetSize(1300, 700);

	// CommandSubclass 인스턴스를 생성하고, CustomInteractor 인스턴스를 전달합니다.
	CustomInteractor::CommandSubclass* rawCommand{ CustomInteractor::CommandSubclass::New(interactorInstance) };
	vtkSmartPointer<CustomInteractor::CommandSubclass> myCommand{ vtkSmartPointer<CustomInteractor::CommandSubclass>::Take(rawCommand) };

	// 마우스 왼쪽 버튼 클릭 이벤트에 대한 옵저버를 추가합니다.
	this->widget->interactor->AddObserver(vtkCommand::LeftButtonPressEvent, myCommand);

	this->widget->interactor->SetRenderWindow(this->widget->renderWindow);

	this->widget->renderWindow->Render();

	this->widget->interactor->Start();

}