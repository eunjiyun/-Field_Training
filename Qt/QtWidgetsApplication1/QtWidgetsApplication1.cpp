
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
// ?ŒìŠ¤ ì½”ë“œ -> .obj ->ì»´íŒŒ??>ë§í¬(ê¸°ê³„?´ë¡œ ë²ˆì—­)
//0bj?Œì¼ë¡?exe?Œì¼ ?ì„±
//ì¤‘ê°„ ?¨ê³„??moc_ ~~~ .obj ==>>ì¤‘ê°„ ?¨ê³„ë¥??œë²ˆ??ê±°ì¹œ??
//ë¬¸ì œ? ì»´íŒŒ???ë„ê°€ ?ë¦¬??
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


	//?´ë–¤ ?¨ìˆ˜?ì„œ ?´ë–»ê²?ì²˜ë¦¬? ì?
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
	
	// Áß¾Ó À§Á¬ÀÇ ·¹ÀÌ¾Æ¿ôÀ» ¼³Á¤ÇÕ´Ï´Ù.
	QVBoxLayout* layout{ new QVBoxLayout(scShot) };
	layout->addWidget(this->screenShot);
	this->screenShot->setStyleSheet("background-color: steelblue;");
	
}
void QtWidgetsApplication1::test()
{
	ui.pushButton->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "start!", nullptr));

	this->widget->renderWindow->AddRenderer(this->widget->renderer);
	// CustomInteractor ?¸ìŠ¤?´ìŠ¤ë¥??ì„±?©ë‹ˆ??
	vtkSmartPointer<CustomInteractor> interactorInstance{ vtkSmartPointer<CustomInteractor>::New() };


	this->widget->interactor->SetInteractorStyle(interactorInstance);

	this->widget->setRenderWindow(this->widget->renderWindow);


	// CommandSubclass ?¸ìŠ¤?´ìŠ¤ë¥??ì„±?˜ê³ , CustomInteractor ?¸ìŠ¤?´ìŠ¤ë¥??„ë‹¬?©ë‹ˆ??
	CustomInteractor::CommandSubclass* rawCommand{ CustomInteractor::CommandSubclass::New(interactorInstance) };
	vtkSmartPointer<CustomInteractor::CommandSubclass> myCommand{ vtkSmartPointer<CustomInteractor::CommandSubclass>::Take(rawCommand) };

	// ë§ˆìš°???¼ìª½ ë²„íŠ¼ ?´ë¦­ ?´ë²¤?¸ì— ?€???µì?ë²„ë? ì¶”ê??©ë‹ˆ??
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