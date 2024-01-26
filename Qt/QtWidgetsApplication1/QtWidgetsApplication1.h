#pragma once
#include <QtWidgets/QMainWindow>
#include <vtkSmartPointer.h>
#include <vtkPLYReader.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include "ui_QtWidgetsApplication1.h"

#include <vtkInteractorStyleTrackballCamera.h>


class In3DTestWidget;
class CScreenShot;



// CustomInteractor Ŭ���� ���ο� CommandSubclass ����
class CustomInteractor : public vtkInteractorStyleTrackballCamera {
public:
	static CustomInteractor* New() {
		return new CustomInteractor;
	}

	// ... ��Ÿ ��� �Լ��� ...

	void OnLeftButtonDown()
	{
		// ���콺 ���� ��ư�� ������ ���� ó��
		vtkInteractorStyleTrackballCamera::OnLeftButtonDown(); // �⺻ ������ ����
		// ... ���⿡ �߰����� ó���� �߰� ...
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
			std::cout << "!!";
			self->OnLeftButtonDown();
		}
	};
};

class QtWidgetsApplication1 : public QMainWindow//���� ������
{
	//
	Q_OBJECT

public:
	QtWidgetsApplication1(QWidget* parent = nullptr);
	~QtWidgetsApplication1();

public:
	void test();
	void colchan();

public:

	In3DTestWidget* widget;
	vtkUnsignedCharArray* cl;

private:
	Ui::QtWidgetsApplication1Class ui;

};



