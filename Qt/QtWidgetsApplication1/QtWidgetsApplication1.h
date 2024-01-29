#pragma once
#include <vtkSmartPointer.h>
#include <vtkPLYReader.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include <QtWidgets/QMainWindow>

#include "ui_QtWidgetsApplication1.h"




class In3DTestWidget;
class CScreenShot;


// CustomInteractor 클래스 내부에 CommandSubclass 정의
class CustomInteractor : public vtkInteractorStyleTrackballCamera {
public:
	static CustomInteractor* New() {
		return new CustomInteractor;
	}

	// ... 기타 멤버 함수들 ...

	void OnLeftButtonDown()
	{
		// 마우스 왼쪽 버튼을 눌렀을 때의 처리
		vtkInteractorStyleTrackballCamera::OnLeftButtonDown(); // 기본 동작을 유지
		// ... 여기에 추가적인 처리를 추가 ...
	}

	class CommandSubclass : public vtkCommand
	{
	public:
		CustomInteractor* self;

		CommandSubclass(CustomInteractor* self) : self(self) {}

		static CommandSubclass* New(CustomInteractor* self)
		{
			return new CommandSubclass(self);
		}

		void Execute(vtkObject* caller, unsigned long eventId, void* callData) override
		{
			cout << "left button pressed!" << endl;
			self->OnLeftButtonDown();
		}
	};
};


class QtWidgetsApplication1 : public QMainWindow//메인 윈도우
{
	//
	Q_OBJECT

public:
	QtWidgetsApplication1(QWidget* parent = nullptr);
	~QtWidgetsApplication1();

public:
	void test();
	void colchan();
	void setLight();
public:


	In3DTestWidget* widget;
	
	vtkUnsignedCharArray* cl;

	QWidget* scShot;
	CScreenShot* screenShot;

private:
	Ui::QtWidgetsApplication1Class ui;

};



