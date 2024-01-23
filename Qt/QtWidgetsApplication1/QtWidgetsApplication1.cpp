#include "QtWidgetsApplication1.h"
#include<iostream>
#include "In3DVTK_Def.h"
#include "In3DTestWidget.h"
#include "ScreenShot.h" 


//moc : 
// �ҽ� �ڵ� -> .obj ->������->��ũ(����� ����)
//0bj���Ϸ� exe���� ����
//�߰� �ܰ迡 moc_ ~~~ .obj ==>>�߰� �ܰ踦 �ѹ��� ��ģ��.
//����? ������ �ӵ��� ������.
QtWidgetsApplication1::QtWidgetsApplication1(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	this->screenShot = new CScreenShot();
	
	// ���� �������� �߾� ������ �����մϴ�.
	QWidget* centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	// �߾� ������ ���̾ƿ��� �����մϴ�.
	QVBoxLayout* layout = new QVBoxLayout(centralWidget);
	layout->addWidget(this->screenShot);
	this->screenShot->setStyleSheet("background-color: steelblue;");
	this->resize(1200, 800); // ���� �������� ũ�⸦ �ʺ� 1200, ���� 800���� ����
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{}
void QtWidgetsApplication1::colchan()
{
	ui.pushButton_2->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "changed!", nullptr));

	
	cl->SetNumberOfComponents(3);
	for (int i{}; i < this->widget->polyData->GetNumberOfPoints(); ++i)
		cl->InsertNextTuple3(0, 0, 255);


	// Ŭ���ε� ���������Ϳ� ���ε� ���� ������ �����մϴ�.
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