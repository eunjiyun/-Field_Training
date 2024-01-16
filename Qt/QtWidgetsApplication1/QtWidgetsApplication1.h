#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication1.h"
#include <vtkSmartPointer.h>
#include <vtkPLYReader.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

class In3DTestWidget;

class QtWidgetsApplication1 : public QMainWindow//메인 윈도우
{
    //
    Q_OBJECT

public:
   //vtkFloatArray* hsvValues = vtkFloatArray::New();
    vtkFloatArray* hsvValues;
    vtkUnsignedCharArray* clippedColors;// = vtkUnsignedCharArray::New();
    vtkPolyData* polyData;
    vtkSmartPointer<vtkPolyDataMapper> mapper ;
   
    vtkSmartPointer<vtkActor> actor ;
    
    vtkSmartPointer<vtkRenderer> renderer ;
   
    vtkSmartPointer<vtkRenderWindow> renderWindow ;
   

    vtkSmartPointer<vtkRenderWindowInteractor> interactor;
    QtWidgetsApplication1(QWidget* parent = nullptr);
    ~QtWidgetsApplication1();

    void test();
    void colchan();
    In3DTestWidget* widget;
private:
    Ui::QtWidgetsApplication1Class ui;
    
};
