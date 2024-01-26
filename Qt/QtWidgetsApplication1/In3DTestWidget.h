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
	vtkFloatArray* hsvValues;
	vtkUnsignedCharArray* clippedColors;
	vtkPolyData* polyData;

	vtkPolyDataMapper* mapper{ vtkPolyDataMapper::New() };

	vtkActor* actor{ vtkActor::New() };

	vtkRenderer* renderer{ vtkRenderer::New() };

	vtkRenderWindow* renderWindow{ vtkRenderWindow::New() };
	vtkRenderWindowInteractor* interactor{ vtkRenderWindowInteractor::New() };

private:
	Ui::In3DTestWidgetClass* ui;
};
