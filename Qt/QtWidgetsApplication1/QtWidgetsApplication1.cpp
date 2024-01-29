
#include <QVBoxLayout>
#include <QLabel>

#include<iostream>

#include "vtkInteractorObserver.h"

#include "QVTKOpenGLNativeWidget.h ";
#include "QtWidgetsApplication1.h"
#include "In3DVTK_Def.h"
#include "In3DTestWidget.h"
#include "ScreenShot.h" 












//moc : 
// ?�스 코드 -> .obj ->컴파??>링크(기계?�로 번역)
//0bj?�일�?exe?�일 ?�성
//중간 ?�계??moc_ ~~~ .obj ==>>중간 ?�계�??�번??거친??
//문제? 컴파???�도가 ?�리??
QtWidgetsApplication1::QtWidgetsApplication1(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);


	this->widget = new In3DTestWidget(this);
	this->widget->setGeometry(400, 150, 1050, 700);



	this->scShot = new QWidget(this);
	this->scShot->setGeometry(10, 370, 390, 500);


	this->resize(1500, 900);

	this->widget->LoadTest();


	//?�떤 ?�수?�서 ?�떻�?처리?��?
	connect(ui.pushButton, &QPushButton::clicked, this, &QtWidgetsApplication1::test);
	cl = vtkUnsignedCharArray::New();
	connect(ui.pushButton_2, &QPushButton::clicked, this, &QtWidgetsApplication1::colchan);
	//horizontalSlider
	connect(ui.horizontalSlider, &QSlider::valueChanged, this, &QtWidgetsApplication1::setLight);
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{
}
void QtWidgetsApplication1::colchan()
{
	ui.pushButton_2->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "captured!", nullptr));


	this->screenShot = new CScreenShot();
	
	// �߾� ������ ���̾ƿ��� �����մϴ�.
	QVBoxLayout* layout{ new QVBoxLayout(scShot) };
	layout->addWidget(this->screenShot);
	this->screenShot->setStyleSheet("background-color: steelblue;");
	
}
void QtWidgetsApplication1::test()
{
	ui.pushButton->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "start!", nullptr));

	this->widget->renderWindow->AddRenderer(this->widget->renderer);
	// CustomInteractor ?�스?�스�??�성?�니??
	vtkSmartPointer<CustomInteractor> interactorInstance{ vtkSmartPointer<CustomInteractor>::New() };


	this->widget->interactor->SetInteractorStyle(interactorInstance);

	this->widget->setRenderWindow(this->widget->renderWindow);


	// CommandSubclass ?�스?�스�??�성?�고, CustomInteractor ?�스?�스�??�달?�니??
	CustomInteractor::CommandSubclass* rawCommand{ CustomInteractor::CommandSubclass::New(interactorInstance) };
	vtkSmartPointer<CustomInteractor::CommandSubclass> myCommand{ vtkSmartPointer<CustomInteractor::CommandSubclass>::Take(rawCommand) };

	// 마우???�쪽 버튼 ?�릭 ?�벤?�에 ?�???��?버�? 추�??�니??
	this->widget->interactor->AddObserver(vtkCommand::LeftButtonPressEvent, myCommand);

	this->widget->interactor->SetRenderWindow(this->widget->renderWindow);

	this->widget->interactor->Start();

}

void QtWidgetsApplication1::setLight()
{
	cout << "inten : " << this->widget->intensity<< endl;
	this->widget->intensity = ui.horizontalSlider->value()/(float)10.f;

	this->widget->light->SetIntensity(this->widget->intensity);
}