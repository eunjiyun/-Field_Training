#include <QDebug>
#include "In3DTestWidget.h"


In3DTestWidget::In3DTestWidget(QWidget* parent)
	: QVTKOpenGLNativeWidget(parent)
	, ui(new Ui::In3DTestWidgetClass())
{
	ui->setupUi(this);
}

In3DTestWidget::~In3DTestWidget()
{
	delete ui;
}

void In3DTestWidget::LoadTest()
{
	qDebug() << "LoadTest Call";

	vtkSmartPointer<vtkPLYReader> reader{ vtkSmartPointer<vtkPLYReader>::New() };

	reader->SetFileName(u8"C:\\Users\\dbzho\\OneDrive\\Desktop\\Field_Training\\Qt\\QtWidgetsApplication1\\upperJaw_1.ply");
	reader->Update();

	polyData = reader->GetOutput();

	// 클리핑 전 RGB 색상 정보를 저장합니다.
	vtkUnsignedCharArray* originalColors{ vtkUnsignedCharArray::SafeDownCast(polyData->GetPointData()->GetScalars()) };


	// 클리핑된 폴리데이터의 각 점에 대한 새로운 RGB 색상 배열을 생성합니다.
	clippedColors = vtkUnsignedCharArray::New();
	clippedColors->SetNumberOfComponents(3); // R, G, B
	clippedColors->SetName("Colors");

	// Create a new array to store the HSV values
	hsvValues = vtkFloatArray::New();

	hsvValues->SetNumberOfComponents(3);
	hsvValues->SetName("HSVValues");

	for (int i{}; i < originalColors->GetNumberOfTuples(); ++i) {
		float r{ static_cast<float>(originalColors->GetComponent(i, 0)) };
		float g{ static_cast<float>(originalColors->GetComponent(i, 1)) };
		float b{ static_cast<float>(originalColors->GetComponent(i, 2)) };

		// Convert RGB to HSV
		float maxVal{ std::max(std::max(r, g), b) };
		float minVal{ std::min(std::min(r, g), b) };
		float delta{ maxVal - minVal };

		float hue, saturation, value;
		if (0 == delta) {
			hue = 0;
			saturation = 0;
			value = maxVal;
		}
		else {
			if (maxVal == r)
				hue = (g - b) / delta;
			else if (maxVal == g)
				hue = 2 + (b - r) / delta;
			else
				hue = 4 + (r - g) / delta;

			hue *= 60;
			if (0 > hue)
				hue += 360;

			saturation = delta / maxVal;
			value = maxVal;
		}

		// 변환된 RGB 값을 vtkFloatArray에 추가합니다.
		hsvValues->InsertNextTuple3(hue, saturation, value);
	}

	polyData->GetPointData()->SetScalars(originalColors);

	//// 클리핑을 수행합니다.
	//vtkClipPolyData* clipper{ vtkClipPolyData::New() };
	//clipper->SetInputData(polyData);

	//clipper->SetValue(20);
	//clipper->Update();

	//// 클리핑된 폴리데이터를 가져옵니다.
	//polyData = clipper->GetOutput();




	//// 클리핑된 폴리데이터의 각 점에 대해 가장 가까운 원본 점의 색상을 매핑합니다.
	//for (int i{}; i < polyData->GetNumberOfPoints(); ++i)
	//	clippedColors->InsertNextTuple3(255, 255, 255);




	//// 클리핑된 폴리데이터에 매핑된 색상 정보를 설정합니다.
	//polyData->GetPointData()->SetScalars(clippedColors);

	polyData->Print(std::cout);
	vtkNew<vtkCurvatures> curv;
	curv->SetInputData(polyData);
	curv->SetCurvatureTypeToMean();
	curv->Update();
	polyData->DeepCopy(curv->GetOutput());
	polyData->Print(std::cout);


	auto curvature{ polyData->GetPointData()->GetArray("Mean_Curvature") };
	auto range{ curvature->GetRange() };//[min : max]
	auto min{ range[0] };
	auto max{ range[1] };

	std::vector<double> vecCurv;

	std::cout << "min : " << min << '\n' << "max : " << max << endl;

	for (int i{}; i < curvature->GetNumberOfTuples(); ++i) 
		vecCurv.push_back(*curvature->GetTuple(i));



	//vtkNew<vtkLookupTable> lt;
	vtkNew<vtkColorTransferFunction> ctf;
	//ctf->AddRGBPoint(+20, 1, 0, 0);//20 이상이면 빨강
	ctf->AddRGBPoint(+0.0001, 1, 0, 0);//0.0001 부터 20이면 a,b의 선형보간
	ctf->AddRGBPoint(-0.00005, 0, 1, 0);
	ctf->AddRGBPoint(-0.0001, 0, 0, 1);
	//ctf->AddRGBPoint(-0.0001, 0, 0, 1);
	ctf->Build();


	// Visualize
	mapper->SetInputData(polyData);


	mapper->SetColorModeToMapScalars();
	mapper->SetLookupTable(ctf);

	
	actor->SetMapper(mapper);

	auto sp{ actor->GetShaderProperty() };

	auto un{ sp->GetFragmentCustomUniforms() };
	un->SetUniform("u_color", vtkUniforms::TupleTypeVector, 3, std::vector<float>{1.0, 0.0, 0.0});
	sp->AddFragmentShaderReplacement(
		"//VTK::Coincident::Impl",                        
		true,
		"fragOutput0.a=0.5f;",                      
		true
	);

	renderer->AddActor(actor);

	// vtkLight 객체를 생성합니다.
	

	// 조명의 위치를 설정합니다.
	light->SetPosition(1.0, 2.0, 3.0);

	// 조명의 색상을 설정합니다. 이 경우 RGB 값이 모두 1이므로 흰색 조명이 됩니다.
	light->SetColor(1.0, 1.0, 1.0);

	// 조명의 밝기를 설정합니다.
	light->SetIntensity(1.0);

	renderer->AddLight(light);

	vtkSmartPointer<vtkNamedColors> colors{ vtkSmartPointer<vtkNamedColors>::New() };
	renderer->SetBackground(colors->GetColor3d("yellow").GetData());
}
