#include <QtWidgets/QMainWindow>
#include <QScreen>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QSpinBox>
#include <QTimer>

#include <QDir>
#include <QStandardPaths>
#include <QFileDialog>
#include <QMessageBox>

#include <QImageWriter>
#include <QGuiApplication>
#include <QApplication>
#include <QWindow>
#include "ScreenShot.h"

CScreenShot::CScreenShot()

	: screenshotLabel(new QLabel(this))
{
	screenshotLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	screenshotLabel->setAlignment(Qt::AlignCenter);

	const QRect screenGeometry{ screen()->geometry() };
	screenshotLabel->setMinimumSize(screenGeometry.width() / 8, screenGeometry.height() / 8);

	QVBoxLayout* mainLayout{ new QVBoxLayout(this) };
	mainLayout->addWidget(screenshotLabel);

	QGroupBox* optionsGroupBox{ new QGroupBox(tr("Options"), this) };
	delaySpinBox = new QSpinBox(optionsGroupBox);
	delaySpinBox->setSuffix(tr(" s"));
	delaySpinBox->setMaximum(60);

	connect(delaySpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
		this, &CScreenShot::updateCheckBox);

	hideThisWindowCheckBox = new QCheckBox(tr("Hide This Window"), optionsGroupBox);

	QGridLayout* optionsGroupBoxLayout{ new QGridLayout(optionsGroupBox) };
	optionsGroupBoxLayout->addWidget(new QLabel(tr("Screenshot Delay:"), this), 0, 0);
	optionsGroupBoxLayout->addWidget(delaySpinBox, 0, 1);
	optionsGroupBoxLayout->addWidget(hideThisWindowCheckBox, 1, 0, 1, 2);

	mainLayout->addWidget(optionsGroupBox);

	QHBoxLayout* buttonsLayout{ new QHBoxLayout };
	newScreenshotButton = new QPushButton(tr("New Screenshot"), this);
	connect(newScreenshotButton, &QPushButton::clicked, this, &CScreenShot::newScreenshot);
	buttonsLayout->addWidget(newScreenshotButton);
	QPushButton* saveScreenshotButton{ new QPushButton(tr("Save Screenshot"), this) };
	connect(saveScreenshotButton, &QPushButton::clicked, this, &CScreenShot::saveScreenshot);
	buttonsLayout->addWidget(saveScreenshotButton);
	QPushButton* quitScreenshotButton{ new QPushButton(tr("Quit"), this) };
	quitScreenshotButton->setShortcut(Qt::CTRL | Qt::Key_Q);
	connect(quitScreenshotButton, &QPushButton::clicked, this, &QWidget::close);
	buttonsLayout->addWidget(quitScreenshotButton);
	buttonsLayout->addStretch();
	mainLayout->addLayout(buttonsLayout);

	shootScreen();
	delaySpinBox->setValue(5);

	setWindowTitle(tr("Screenshot"));
	resize(300, 200);

}

void CScreenShot::resizeEvent(QResizeEvent* /* event */)
{

	QSize scaledSize{ originalPixmap.size() };
	scaledSize.scale(screenshotLabel->size(), Qt::KeepAspectRatio);

	if (!screenshotLabel->pixmap() || scaledSize != screenshotLabel->pixmap()->size())
		updateScreenshotLabel();

}

void CScreenShot::newScreenshot()
{

	if (hideThisWindowCheckBox->isChecked())
		hide();
	newScreenshotButton->setDisabled(true);

	QTimer::singleShot(delaySpinBox->value() * 1000, this, &CScreenShot::shootScreen);

}

void CScreenShot::saveScreenshot()
{

	const QString format{ "png" };
	QString initialPath{ QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) };
	if (initialPath.isEmpty())
		initialPath = QDir::currentPath();
	initialPath += tr("/untitled.") + format;

	QFileDialog fileDialog(this, tr("Save As"), initialPath);
	fileDialog.setAcceptMode(QFileDialog::AcceptSave);
	fileDialog.setFileMode(QFileDialog::AnyFile);
	fileDialog.setDirectory(initialPath);
	QStringList mimeTypes;
	const QList<QByteArray> baMimeTypes{ QImageWriter::supportedMimeTypes() };
	for (const QByteArray& bf : baMimeTypes)
		mimeTypes.append(QLatin1String(bf));
	fileDialog.setMimeTypeFilters(mimeTypes);
	fileDialog.selectMimeTypeFilter("image/" + format);
	fileDialog.setDefaultSuffix(format);
	if (fileDialog.exec() != QDialog::Accepted)
		return;
	const QString fileName{ fileDialog.selectedFiles().first() };
	if (!originalPixmap.save(fileName)) {
		QMessageBox::warning(this, tr("Save Error"), tr("The image could not be saved to \"%1\".")
			.arg(QDir::toNativeSeparators(fileName)));
	}

}

void CScreenShot::shootScreen()
{

	QScreen* screen{ QGuiApplication::primaryScreen() };
	if (const QWindow * window{ windowHandle() })
		screen = window->screen();
	if (!screen)
		return;

	if (delaySpinBox->value() != 0)
		QApplication::beep();



	originalPixmap = screen->grabWindow(0);

	QRect rect(600, 200, 1050, 700);  // 캡처할 영역을 정의
	QPixmap capturedArea{ originalPixmap.copy(rect) };  // 해당 영역을 잘라냄

	originalPixmap = capturedArea;
	updateScreenshotLabel();

	newScreenshotButton->setDisabled(false);
	if (hideThisWindowCheckBox->isChecked())
		show();

}

void CScreenShot::updateCheckBox()
{

	if (0 == delaySpinBox->value()) {
		hideThisWindowCheckBox->setDisabled(true);
		hideThisWindowCheckBox->setChecked(false);
	}
	else
		hideThisWindowCheckBox->setDisabled(false);

}

void CScreenShot::updateScreenshotLabel()
{

	screenshotLabel->setPixmap(originalPixmap.scaled(screenshotLabel->size(),
		Qt::KeepAspectRatio,
		Qt::SmoothTransformation));

}
