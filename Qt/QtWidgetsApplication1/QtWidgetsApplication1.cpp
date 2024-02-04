#include <QVBoxLayout>
#include <QLabel>
#include<iostream>
#include "vtkInteractorObserver.h"
#include "QVTKOpenGLNativeWidget.h ";
#include "QtWidgetsApplication1.h"
#include "In3DVTK_Def.h"
#include "In3DTestWidget.h"
#include "ScreenShot.h" 
#include "CustomColorTransferFunction.h"




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
	connect(ui.pushButton_2, &QPushButton::clicked, this, &QtWidgetsApplication1::capture);
	//horizontalSlider
	connect(ui.horizontalSlider, &QSlider::valueChanged, this, &QtWidgetsApplication1::setLight);
	connect(ui.pushButton_3, &QPushButton::clicked, this, &QtWidgetsApplication1::blend);
	connect(ui.pushButton_4, &QPushButton::clicked, this, &QtWidgetsApplication1::curve);
	connect(ui.pushButton_5, &QPushButton::clicked, this, &QtWidgetsApplication1::clip);
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{
}
void QtWidgetsApplication1::capture()
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
	//cout << "inten : " << this->widget->intensity << endl;
	this->widget->intensity = ui.horizontalSlider->value() / (float)10.f;

	this->widget->light->SetIntensity(this->widget->intensity);
}

void QtWidgetsApplication1::blend()
{
	if (!widget->transparent) {
		ui.pushButton_3->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "transparent!", nullptr));
		widget->transparent = true;
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
		ui.pushButton_3->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "non blend", nullptr));
		widget->transparent = false;
		auto sp{ widget->actor->GetShaderProperty() };

		sp->AddFragmentShaderReplacement(
			"//VTK::Coincident::Impl",
			true,
			"fragOutput0.a=1.0f;",
			true
		);
	}
}

void QtWidgetsApplication1::curve()
{
	if (!widget->cur) {
		ui.pushButton_4->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "curved!", nullptr));
		widget->cur = true;
		vtkNew<CustomColorTransferFunction> ctf;


		vtkNew<vtkCurvatures> curv;
		curv->SetInputData(widget->polyData);
		curv->SetCurvatureTypeToMean();
		curv->Update();
		widget->polyData->DeepCopy(curv->GetOutput());


		auto curvature{ widget->polyData->GetPointData()->GetArray("Mean_Curvature") };

		vtkDoubleArray* curvArr{ vtkDoubleArray::New() };

		for (int i{}; i < curvature->GetNumberOfTuples(); ++i) 
			curvArr->InsertNextTuple1(*curvature->GetTuple(i));
		
		
		double sum{};
		for (vtkIdType i{}; i < curvArr->GetNumberOfTuples(); ++i) 
			sum += curvArr->GetValue(i);
		

		double mean{ sum / curvArr->GetNumberOfTuples() };

		double sumSquaredDiffs{};
		//Calculate the sum of squared differences

		for (vtkIdType i{}; i < curvArr->GetNumberOfTuples(); ++i) {
			double diff{ curvArr->GetValue(i) - mean };
			sumSquaredDiffs += diff * diff;
		}

		// Calculate the sample standard deviation
		double stdDev{ std::sqrt(sumSquaredDiffs / (curvArr->GetNumberOfTuples() - 1)) };

		ctf->SetColorSpaceToLab();

		
		ctf->AddRGBPoints(curvArr, widget->originalColors); 


		ctf->Build();

		widget->mapper->SetColorModeToMapScalars();
		widget->mapper->SetLookupTable(ctf);
	}
	else {
		ui.pushButton_4->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "non curved", nullptr));
		widget->cur = false;

		vtkSmartPointer<vtkPLYReader> reader{ vtkSmartPointer<vtkPLYReader>::New() };

		reader->SetFileName(u8"C:\\Users\\UserK\\Desktop\\Field_Training\\Qt\\QtWidgetsApplication1\\upperJaw_1.ply");
		reader->Update();

		widget->polyData = reader->GetOutput();

		// 클리핑 전 RGB 색상 정보를 저장합니다.
		vtkUnsignedCharArray* originalColors{ vtkUnsignedCharArray::SafeDownCast(widget->polyData->GetPointData()->GetScalars()) };


		widget->polyData->GetPointData()->SetScalars(originalColors);


		// Visualize
		widget->mapper->SetInputData(widget->polyData);


		widget->actor->SetMapper(widget->mapper);

		widget->renderer->AddActor(widget->actor);
	}

}

void QtWidgetsApplication1::clip()
{
	if (!widget->clipped) {
		ui.pushButton_5->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "clipped!", nullptr));
		widget->clipped = true;
		widget->polyData->GetPointData()->SetScalars(widget->hsvValues);
		// 클리핑을 수행합니다.
		vtkClipPolyData* clipper{ vtkClipPolyData::New() };
		clipper->SetInputData(widget->polyData);

		clipper->SetValue(20);
		clipper->Update();

		// 클리핑된 폴리데이터를 가져옵니다.
		widget->polyData = clipper->GetOutput();

		// 클리핑된 폴리데이터의 각 점에 대한 새로운 RGB 색상 배열을 생성합니다.
		vtkUnsignedCharArray* clippedColors{ vtkUnsignedCharArray::New() };
		clippedColors->SetNumberOfComponents(3); // R, G, B
		clippedColors->SetName("Colors");


		// 클리핑된 폴리데이터의 각 점에 대해 가장 가까운 원본 점의 색상을 매핑합니다.
		for (int i{}; i < widget->polyData->GetNumberOfPoints(); ++i)
			clippedColors->InsertNextTuple3(255,255, 255);


		// 클리핑된 폴리데이터에 매핑된 색상 정보를 설정합니다.
		widget->polyData->GetPointData()->SetScalars(clippedColors);

		widget->mapper->SetInputData(widget->polyData);

		widget->actor->SetMapper(widget->mapper);

		widget->renderer->AddActor(widget->actor);

		// Visualize
		widget->mapper->SetInputData(widget->polyData);

		widget->actor->SetMapper(widget->mapper);

		widget->renderer->AddActor(widget->actor);
	}
	else {
		ui.pushButton_5->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "non clipped", nullptr));
		widget->clipped = false;

		vtkSmartPointer<vtkPLYReader> reader{ vtkSmartPointer<vtkPLYReader>::New() };

		reader->SetFileName(u8"C:\\Users\\UserK\\Desktop\\Field_Training\\Qt\\QtWidgetsApplication1\\upperJaw_1.ply");
		reader->Update();

		widget->polyData = reader->GetOutput();

		// 클리핑 전 RGB 색상 정보를 저장합니다.
		widget->originalColors = (vtkDoubleArray*)widget->polyData->GetPointData()->GetScalars();


		widget->polyData->GetPointData()->SetScalars(widget->originalColors);


		// Visualize
		widget->mapper->SetInputData(widget->polyData);


		widget->actor->SetMapper(widget->mapper);

		widget->renderer->AddActor(widget->actor);
	}
}