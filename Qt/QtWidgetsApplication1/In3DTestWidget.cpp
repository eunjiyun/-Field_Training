#include "In3DTestWidget.h"



In3DTestWidget::In3DTestWidget(QWidget* parent)
	: QVTKOpenGLNativeWidget(parent)
	, ui(new Ui::In3DTestWidgetClass())
{


	// 배경색을 설정합니다.
	//setStyleSheet("background-color: blue;");
	//setAutoFillBackground(true);
	ui->setupUi(this);


	renderWindow = vtkSmartPointer<vtkRenderWindow>::New();

	renderer = vtkRenderer::New();

	vtkPLYReader* plyReader = vtkPLYReader::New();
	plyReader->SetFileName(u8"C:\\Users\\dbzho\\OneDrive\\Desktop\\Field_Training\\vtk\\vtk_실습용_프로젝트\\upperJaw_1.ply");
	plyReader->Update();

	mapper = vtkPolyDataMapper::New();
	mapper->SetInputData(plyReader->GetOutput());

	actor = vtkActor::New();
	actor->SetMapper(mapper);

	renderer->AddActor(actor);




	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize(800, 600);

	//SetRenderWindow(vtkRenderWindow);
	//this->show();
	renderWindow->SetShowWindow(renderWindow);

	interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);

	renderWindow->Render();
	interactor->Start();


}

In3DTestWidget::~In3DTestWidget()
{
	delete ui;
}
