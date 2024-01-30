#include <QDebug>
#include <cmath>
#include <numeric>
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
	std::vector<double> vecCurv;
	for (int i{}; i < curvature->GetNumberOfTuples(); ++i) 
		vecCurv.push_back(*curvature->GetTuple(i));



	//vtkNew<vtkLookupTable> lt;
	vtkNew<vtkColorTransferFunction> ctf;

	double mean{ std::accumulate(vecCurv.begin(), vecCurv.end(), 0.0) / vecCurv.size() };

	double sq_sum{ std::inner_product(vecCurv.begin(), vecCurv.end(), vecCurv.begin(), 0.0) };
	double std_dev{ std::sqrt(sq_sum / vecCurv.size() - mean * mean) };

	std::cout << "Mean : " << mean << '\n' << "Standard Deviation : " << std_dev << std::endl;


	ctf->AddRGBPoint(mean - std_dev, 1, 0, 0); // 표준편차만큼 평균보다 낮은 굴곡률을 파란색으로
	ctf->AddRGBPoint(mean, 0, 1, 0); // 평균 굴곡률을 초록색으로
	ctf->AddRGBPoint(mean + std_dev, 0, 0, 1);


	ctf->Build();


	// Visualize
	mapper->SetInputData(polyData);


	mapper->SetColorModeToMapScalars();
	mapper->SetLookupTable(ctf);

	
	actor->SetMapper(mapper);

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
