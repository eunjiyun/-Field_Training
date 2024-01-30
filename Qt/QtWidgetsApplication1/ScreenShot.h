#pragma once
#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
class CScreenShot : public QWidget
{
	Q_OBJECT

public:
	CScreenShot();

protected:
	void resizeEvent(QResizeEvent* event) override;

private slots:
	void newScreenshot();
	void saveScreenshot();
	void shootScreen();
	void updateCheckBox();

private:
	void updateScreenshotLabel();

public:
	QPixmap originalPixmap;

	QLabel* screenshotLabel;
	QSpinBox* delaySpinBox;
	QCheckBox* hideThisWindowCheckBox;
	QPushButton* newScreenshotButton;
};

