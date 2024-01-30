
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
// ?뚯뒪 肄붾뱶 -> .obj ->而댄뙆??>留곹겕(湲곌퀎?대줈 踰덉뿭)
//0bj?뚯씪濡?exe?뚯씪 ?앹꽦
//以묎컙 ?④퀎??moc_ ~~~ .obj ==>>以묎컙 ?④퀎瑜??쒕쾲??嫄곗튇??
//臾몄젣? 而댄뙆???띾룄媛 ?먮━??
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


	//?대뼡 ?⑥닔?먯꽌 ?대뼸寃?泥섎━?좎?
	connect(ui.pushButton, &QPushButton::clicked, this, &QtWidgetsApplication1::test);
	cl = vtkUnsignedCharArray::New();
	connect(ui.pushButton_2, &QPushButton::clicked, this, &QtWidgetsApplication1::colchan);
	//horizontalSlider
	connect(ui.horizontalSlider, &QSlider::valueChanged, this, &QtWidgetsApplication1::setLight);
	connect(ui.pushButton_3, &QPushButton::clicked, this, &QtWidgetsApplication1::blend);
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{
}
void QtWidgetsApplication1::colchan()
{
	ui.pushButton_2->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "captured!", nullptr));


	this->screenShot = new CScreenShot();
	
	// 중앙 위젯의 레이아웃을 설정합니다.
	QVBoxLayout* layout{ new QVBoxLayout(scShot) };
	layout->addWidget(this->screenShot);
	this->screenShot->setStyleSheet("background-color: steelblue;");
	
}
void QtWidgetsApplication1::test()
{
	ui.pushButton->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "start!", nullptr));

	this->widget->renderWindow->AddRenderer(this->widget->renderer);
	// CustomInteractor ?몄뒪?댁뒪瑜??앹꽦?⑸땲??
	vtkSmartPointer<CustomInteractor> interactorInstance{ vtkSmartPointer<CustomInteractor>::New() };


	this->widget->interactor->SetInteractorStyle(interactorInstance);

	this->widget->setRenderWindow(this->widget->renderWindow);


	// CommandSubclass ?몄뒪?댁뒪瑜??앹꽦?섍퀬, CustomInteractor ?몄뒪?댁뒪瑜??꾨떖?⑸땲??
	CustomInteractor::CommandSubclass* rawCommand{ CustomInteractor::CommandSubclass::New(interactorInstance) };
	vtkSmartPointer<CustomInteractor::CommandSubclass> myCommand{ vtkSmartPointer<CustomInteractor::CommandSubclass>::Take(rawCommand) };

	// 留덉슦???쇱そ 踰꾪듉 ?대┃ ?대깽?몄뿉 ????듭?踰꾨? 異붽??⑸땲??
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

void QtWidgetsApplication1::blend()
{
	if (!transparent) {
		ui.pushButton_3->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "transparent!", nullptr));
		transparent = true;
		auto sp{ widget->actor->GetShaderProperty() };

		//auto un{ sp->GetFragmentCustomUniforms() };
		//un->SetUniform("u_color", vtkUniforms::TupleTypeVector, 3, std::vector<float>{1.0, 0.0, 0.0});
		sp->AddFragmentShaderReplacement(
			"//VTK::Coincident::Impl",
			true,
			"fragOutput0.a=0.5f;",
			true
		);
	}
	else {
		ui.pushButton_3->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "non", nullptr));
		transparent = false;
		auto sp{ widget->actor->GetShaderProperty() };

		//auto un{ sp->GetFragmentCustomUniforms() };
		//un->SetUniform("u_color", vtkUniforms::TupleTypeVector, 3, std::vector<float>{1.0, 0.0, 0.0});
		sp->AddFragmentShaderReplacement(
			"//VTK::Coincident::Impl",
			true,
			"fragOutput0.a=1.0f;",
			true
		);
	}
	

	
}