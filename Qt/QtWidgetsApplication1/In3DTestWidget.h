#pragma once

#include <QWidget>
#include "ui_In3DTestWidget.h"
#include "In3DVTK_Def.h"
#include "QtWidgetsApplication1.h"


QT_BEGIN_NAMESPACE
namespace Ui { class In3DTestWidgetClass; };
QT_END_NAMESPACE



class In3DTestWidget : public QVTKOpenGLNativeWidget
{
	Q_OBJECT

public:
	In3DTestWidget(QWidget* parent = nullptr);
	~In3DTestWidget();

public:
	void LoadTest();

public:
	vtkFloatArray* hsvValues{ vtkFloatArray::New() };
	vtkDoubleArray* originalColors{ vtkDoubleArray::New() };
	vtkPolyData* polyData;

	vtkPolyDataMapper* mapper{ vtkPolyDataMapper::New() };

	vtkActor* actor{ vtkActor::New() };

	vtkRenderer* renderer{ vtkRenderer::New() };
	vtkSmartPointer<vtkLight> light{ vtkSmartPointer<vtkLight>::New() };
	

	vtkGenericOpenGLRenderWindow* renderWindow{ vtkGenericOpenGLRenderWindow::New() };
	vtkRenderWindowInteractor* interactor{ vtkRenderWindowInteractor::New() };

public:
	bool cur{};
	bool transparent{};
	bool clipped{};
	float intensity{};
private:
	Ui::In3DTestWidgetClass* ui;
};
