#include <QDebug>
#include <cmath>
#include <numeric>
#include "In3DTestWidget.h"
#include "CustomColorTransferFunction.h"

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
	originalColors = (vtkDoubleArray*)polyData->GetPointData()->GetScalars();


	// Create a new array to store the HSV values
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



	//polyData->Print(std::cout);




	// Visualize
	mapper->SetInputData(polyData);


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
