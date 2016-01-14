#include "qt_image_marker.h"
#include <QMessageBox>
#include <QValidator>
#include <QDesktopServices>
#include <string>
#include <math.h>
#include <QFileDialog>

static void getBinMask(const cv::Mat& comMask, cv::Mat& binMask)
{
	if (comMask.empty() || comMask.type() != CV_8UC1)
		CV_Error(cv::Error::StsBadArg, "comMask is empty or has incorrect type (not CV_8UC1)");
	if (binMask.empty() || binMask.rows != comMask.rows || binMask.cols != comMask.cols)
		binMask.create(comMask.size(), CV_8UC1);
	binMask = comMask & 1;
}

Qt_Image_Marker::Qt_Image_Marker(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	strokeWidthMin = 5;
	strokeWidthMax = 100;
	folderNameMarker = QString("fg_draw");
	folderNameFG = QString("grab_cut");
	suffixOfSaveMarker = QString(".png");
	suffixOfSaveResult = QString(".bmp");

	action_switchShowMark = new QAction(ui.toolButton_SwitchMark->text(),this);
	action_switchShowMark->setCheckable(true);
	ui.toolButton_SwitchMark->setDefaultAction(action_switchShowMark);
	connect(action_switchShowMark, SIGNAL(changed()), this, SLOT(link_action_switchShowMark()));
	action_switchShowMark->setShortcut(Qt::Key_Q);


	action_switchShowResult = new QAction(ui.toolButton_SwitchResult->text(),this);
	action_switchShowResult->setCheckable(true);
	ui.toolButton_SwitchResult->setDefaultAction(action_switchShowResult);
	connect(action_switchShowResult, SIGNAL(changed()), this, SLOT(link_action_switchShowResult()));
	action_switchShowResult->setShortcut(Qt::Key_W);


	action_grabCut = new QAction("Graph Cut", this);
	action_grabCut->setCheckable(false);
	ui.toolButton_runGrabCut->setDefaultAction(action_grabCut);
	connect(action_grabCut, SIGNAL(triggered()), this, SLOT(link_action_grabCut()));

	connect(ui.radioButton_markForeground, SIGNAL(toggled(bool)), this, SLOT(link_select_foreground_pen(bool)));
	connect(ui.radioButton_markBackground, SIGNAL(toggled(bool)), this, SLOT(link_select_background_pen(bool)));
	connect(ui.radioButton_markClean, SIGNAL(toggled(bool)), this, SLOT(link_select_clen_pen(bool)));
	ui.radioButton_markForeground->setShortcut(Qt::Key_A);
	ui.radioButton_markBackground->setShortcut(Qt::Key_S);
	ui.radioButton_markClean->setShortcut(Qt::Key_D);

	//connect(ui.actionOpenFile, SIGNAL(triggered()), this, SLOT(testFunction()));
	connect(ui.actionOpenFile, SIGNAL(triggered()), this, SLOT(openFolder()));
	ui.actionOpenFile->setShortcut(Qt::Key_O | Qt::CTRL);

	connect(ui.actionClose, SIGNAL(triggered()), this, SLOT(closeImages()));
	connect(ui.imageViewerWidget, SIGNAL(mouseCoorOnImgSignal(QPoint)), this, SLOT(show_coordinate(QPoint)));
	markFroegroundColor = qRgba(0, 0, 255, 128);
	markBackgroundColor = qRgba(255, 0, 0, 128);
	ui.radioButton_markForeground->setChecked(true);

	ui.lineEdit_strokeWidth->setValidator(new QIntValidator(strokeWidthMin, strokeWidthMax, this));
	connect(ui.lineEdit_strokeWidth, SIGNAL(textChanged(const QString)), this, SLOT(link_lineedit_text_change2StrokeWidth(const QString)));

	action_keyaddStrokeWidth = new QAction("AddStrokeWidth",this);
	action_keyaddStrokeWidth->setShortcut(Qt::Key_Plus);
	connect(action_keyaddStrokeWidth, SIGNAL(triggered()), this, SLOT(link_lineedit_text_add_StrokeWidth()));
	this->addAction(action_keyaddStrokeWidth); // for enable qaction, it must be bound to one widget

	action_keysubStrokeWidth = new QAction(this);
	action_keysubStrokeWidth->setShortcut(Qt::Key_Minus);
	connect(action_keysubStrokeWidth, SIGNAL(triggered()), this, SLOT(link_lineedit_text_sub_StrokeWidth()));
	this->addAction(action_keysubStrokeWidth);

	//file list viewer
	fileModel = new QFileSystemModel(this);
	ui.listView_file->setModel(fileModel);
	connect(ui.listView_file, SIGNAL(activated(const QModelIndex &)),
		this, SLOT(selectFilefromList(const QModelIndex &)));

	action_cleanAllMark = new QAction(ui.toolButton_cleanAllMark->text(),this);
	action_cleanAllMark->setCheckable(false);
	ui.toolButton_cleanAllMark->setDefaultAction(action_cleanAllMark);
	connect(action_cleanAllMark, SIGNAL(triggered()), this, SLOT(link_action_clean_mark()));

	connect(ui.lineEdit_strokeWidth, &LineEdit_K::pressEnter, [=](){ui.imageViewerWidget->setFocus(); });

	action_changeStrockWidth = new QAction(this);
	action_changeStrockWidth->setShortcut(Qt::Key_E);
	connect(action_changeStrockWidth,&QAction::triggered,[=](){ui.lineEdit_strokeWidth->setFocus();});
	this->addAction(action_changeStrockWidth);

	ui.toolButton_runGrabCut->setShortcut(Qt::Key_R);

#if 0
	//TODO
	action_undoDraw = new QAction(this);
	connect(action_undoDraw, SIGNAL(triggered()), this, SLOT(link_action_undoDraw()));
	action_undoDraw->setShortcut(Qt::Key_Z | Qt::CTRL);
	this->addAction(action_undoDraw);
#endif
}

Qt_Image_Marker::~Qt_Image_Marker()
{
	delete action_switchShowMark;
	delete action_switchShowResult;
	delete action_grabCut;
	delete action_keyaddStrokeWidth;
	delete action_keysubStrokeWidth;
	delete action_cleanAllMark;
	delete action_changeStrockWidth;
//	delete action_undoDraw;
}


void Qt_Image_Marker::show_coordinate(QPoint p)
{
	this->ui.statusBar->showMessage(QString("%1  %2").arg(p.x()).arg(p.y()));
}

void Qt_Image_Marker::link_action_switchShowMark()
{
	if (action_switchShowMark->isChecked())
	{
		ui.imageViewerWidget->setImgObjHide(1, false);
	}
	else
	{
		ui.imageViewerWidget->setImgObjHide(1, true);
	}
}

void Qt_Image_Marker::link_action_switchShowResult()
{
	if (action_switchShowResult->isChecked())
	{
		ui.imageViewerWidget->setImgObjHide(2, false);
	}
	else
	{
		ui.imageViewerWidget->setImgObjHide(2, true);
	}
}

void Qt_Image_Marker::link_action_grabCut()
{
	graphCut(this->ui.imageViewerWidget->imgObjs[1]->displayImg);
	this->ui.imageViewerWidget->imgObjs[2]->updateImg();
	this->action_switchShowResult->setChecked(true);
}

void Qt_Image_Marker::link_select_foreground_pen(bool checked)
{
	if (checked)
	{
		ui.imageViewerWidget->setDrawColor(QColor::fromRgba(this->markFroegroundColor));
	}
}

void Qt_Image_Marker::link_select_background_pen(bool checked)
{
	if (checked)
		ui.imageViewerWidget->setDrawColor(QColor::fromRgba(this->markBackgroundColor));
}


void Qt_Image_Marker::link_select_clen_pen(bool checked)
{
	if (ui.radioButton_markClean->isChecked())	
		ui.imageViewerWidget->setDrawTranspancy();
}

void Qt_Image_Marker::link_lineedit_text_change2StrokeWidth(const QString& text)
{
	int newWidth = text.toInt();
	ui.imageViewerWidget->setDrawRadius(newWidth);
}

void Qt_Image_Marker::link_lineedit_text_add_StrokeWidth()
{
	int width = ui.lineEdit_strokeWidth->text().toInt();
	ui.lineEdit_strokeWidth->setText(QString::number((width == strokeWidthMax - 1) ? width : (width + 1)));
}

void Qt_Image_Marker::link_lineedit_text_sub_StrokeWidth()
{
	int width = ui.lineEdit_strokeWidth->text().toInt();
	ui.lineEdit_strokeWidth->setText(QString::number((width == strokeWidthMin) ? width : (width - 1)));
}

void Qt_Image_Marker::link_action_clean_mark()
{
	if (ui.imageViewerWidget->imgObjs.size() == 3)
	{
		QImage* img = &(ui.imageViewerWidget->imgObjs[1]->displayImg);
		for (int x = 0; x < img->width();x++)
		{
			for (int y = 0; y < img->height(); y++)
			{
				img->setPixel(x, y, qRgba(0, 0, 0, 0));
			}
		}
		ui.imageViewerWidget->imgObjs[1]->updateImg();
	}
}

void Qt_Image_Marker::link_action_undoDraw()
{
	ui.imageViewerWidget->undoDraw();
}

void Qt_Image_Marker::closeImages()
{
	if (fileInfoSrc.isFile())
	{// already load and edit some image -> save the results
		if (ui.imageViewerWidget->imgObjs.size() == 3)
		{//should always right if image is correct loading
			ui.imageViewerWidget->imgObjs[1]->displayImg.save(fileInfoMarker.absoluteFilePath());
		}
		if (!grabResult.empty()) // only non-empty if grabCut is executed
		{
			if (grabResult.rows == oriImageSize.height())
				cv::imwrite(fileInfoFG.absoluteFilePath().toStdString(), grabResult);
			else
			{
				cv::Mat temp;
				cv::resize(grabResult, temp, cv::Size(oriImageSize.width(), oriImageSize.height()), 0,0,CV_INTER_LINEAR);
				cv::imwrite(fileInfoFG.absoluteFilePath().toStdString(), temp);
			}
		}
	}
	ui.imageViewerWidget->cleanImageObjects(); // clean image in widget
}

void Qt_Image_Marker::openFolder()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		"/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	fileModel->setFilter(QDir::Files | QDir::NoDotAndDotDot);
	ui.listView_file->setRootIndex(fileModel->setRootPath(dir));
}

void Qt_Image_Marker::selectFilefromList(const QModelIndex & current)
{
	QString filename = fileModel->fileInfo(current).absoluteFilePath();
	openImage(filename);
}

bool Qt_Image_Marker::openImage(const QString& filepath)
{
	closeImages();
	fileInfoSrc = QFileInfo(filepath);
	if (!fileInfoSrc.isFile())
	{
		return false;
	}
	QImage testImg(filepath);
	int l = testImg.width() > testImg.height() ? testImg.width() : testImg.height();
	int image_scale = pow(2,((int)ceilf(log2f((float)l/1000)))); // assume 1000 is maximum resolution grab cut can handle
	image_scale = (image_scale > 1) ? image_scale : 1;
	QSize newImgSize = testImg.size() / image_scale;
	this->oriImageSize = testImg.size();
	if (oriImageSize != newImgSize)
	{
		QMessageBox::information(this, "INFO", "Rescale image for grab cut may fail in large image");
		testImg = testImg.scaled(newImgSize);
	}
	// load or create mark and result layer
	QImage markImg = loadAccessImgs(fileInfoSrc, folderNameMarker, suffixOfSaveMarker, testImg, fileInfoMarker, true);
	QImage resultImg = loadAccessImgs(fileInfoSrc,folderNameFG, suffixOfSaveResult, testImg, fileInfoFG, false);
	grabResult = cv::Mat(); // clean exist result

	// dealing with widget //
	ui.imageViewerWidget->addImageObject(testImg, 0); // original
	ui.imageViewerWidget->addImageObject(markImg, 0.1f); // background
	ui.imageViewerWidget->addImageObject(resultImg, 0.2f); // background

	ui.imageViewerWidget->setDrawTarget(ui.imageViewerWidget->imgObjs[1]);
	link_action_switchShowMark();
	link_action_switchShowResult();
	ui.imageViewerWidget->updatePainter(); // painter existed by image( device) hence load new image need to applied it again
	return true;
}

QImage Qt_Image_Marker::loadAccessImgs(const QFileInfo &testFileInfo, QString subFolderName, QString suffixOfFile, QImage testImg, QFileInfo &subFileInfo, bool tryload)
{
	QDir imgdir = testFileInfo.absoluteDir();
	QDir subFolderDir(imgdir);
	if (!subFolderDir.cd(subFolderName))
	{
		subFolderDir = imgdir;
		if (!subFolderDir.mkdir(subFolderName))
			QMessageBox::critical(this, "ERROR", "Fail to create new folder for save image");
		subFolderDir.cd(subFolderName);
	}
	QString newFileName = testFileInfo.completeBaseName() + suffixOfFile;
	subFileInfo.setFile(subFolderDir, newFileName); // just make file info with the name and path, not necessary image exist
	QImage subImg;
	if (tryload)
		subImg = loadOrMakeImg(subFileInfo.absoluteFilePath(), testImg);
	else
		subImg = makeImg(testImg.size());
	return subImg;
}

QImage Qt_Image_Marker::loadOrMakeImg(const QString& filename, const QImage &refImage)
{
	//assume folder of filename should already existed
	QFileInfo fileInfo(filename);
	if (fileInfo.isFile())
	{
		QImage img = QImage(filename);
		if (img.size() == refImage.size())
		{
			QImage imgRGBA8888 = img.convertToFormat(QImage::Format_RGBA8888);
			return imgRGBA8888;
		}
	}
	QImage img(refImage.size(), QImage::Format_RGBA8888);
	for (int x = 0; x < img.width(); x++)
	{
		for (int y = 0; y < img.height(); y++)
			img.setPixel(x, y, qRgba(0, 0, 0, 0));
	}
	return img;
}

QImage Qt_Image_Marker::makeImg(const QSize imgSize)
{
	QImage img(imgSize, QImage::Format_RGBA8888);
	for (int x = 0; x < img.width(); x++)
	{
		for (int y = 0; y < img.height(); y++)
			img.setPixel(x, y, qRgba(0, 0, 0, 0));
	}
	return img;
}

QImage Qt_Image_Marker::mat2QImage(cv::Mat src)
{
	int type = src.type();
	if (type & CV_MAT_DEPTH_MASK == CV_8U)
	{
		return QImage(src.size().width, src.size().height, QImage::Format_ARGB32);
	}
	else
	{
		QImage::Format format;// = QImage::Format_ARGB32
		unsigned char chans = 1 + (type >> CV_CN_SHIFT);
		bool supportType = true;
		switch (chans)
		{
		case 1:
			format = QImage::Format_Indexed8;
			break;
		case 3:
			format = QImage::Format_RGB888;
			cv::cvtColor(src, src, CV_BGR2GRAY);
			break;
		case 4:
			format = QImage::Format_RGBA8888;
			cv::cvtColor(src, src, CV_BGRA2RGBA);
			break;
		default:
			supportType = false;
			break;
		};
		if (supportType)
		{
			QImage dst(src.data, src.cols, src.rows, (int)src.step1(), format);
			return dst;
		}
		else
		{
			QImage dst(src.size().width, src.size().height, QImage::Format_ARGB32);
			return dst;
		}
	}
}

cv::Mat Qt_Image_Marker::QImage2Mat(QImage src)
{
	int cvType = 0;
	QImage::Format f = src.format();
	switch (f)
	{
	case QImage::Format_RGB32:
		cvType = CV_8UC3;
		break;
	case QImage::Format_ARGB32:
	case QImage::Format_RGBA8888:
		cvType = CV_8UC4;
		break;
	case QImage::Format_RGB888:
		cvType = CV_8UC4;
		break;
	default:
		break;
	}
	if (cvType != 0)
	{
		if (cvType == CV_8UC4)
		{
			cv::Mat dst(src.height(), src.width(), cvType, src.bits(), src.bytesPerLine());
			return dst;
		}
		else if (cvType == CV_8UC3 && f == QImage::Format_RGB32)
		{
			QImage temp = src.convertToFormat(QImage::Format_RGB888);
			cv::Mat dst(temp.height(), temp.width(), cvType, temp.bits(), temp.bytesPerLine());
			return dst.clone(); // temp would delete after leave this function
		}
		else
			return cv::Mat();
	}
	else
	{
		cv::Mat dst;
		return dst;
	}
}

cv::Mat Qt_Image_Marker::graphCut(QImage maskImg)
{
	grabResult = cv::Mat(maskImg.height(), maskImg.width(), CV_8UC1);
	for (int i = 0; i < grabResult.rows; i++)
	{
		for (int j = 0; j < grabResult.cols; j++)
		{
			QRgb c = maskImg.pixel(j, i);
			if (c == markFroegroundColor)
				grabResult.at<uchar>(i, j) = cv::GC_FGD;
			else if (c == markBackgroundColor)
			{
				grabResult.at<uchar>(i, j) = cv::GC_BGD;
			}
			else
			{
				grabResult.at<uchar>(i, j) = cv::GC_PR_BGD;
			}
		}
	}
	cv::Mat bgdModel, fgdModel;
	cv::Mat srcMat = QImage2Mat(ui.imageViewerWidget->imgObjs[0]->displayImg);
	cv::Mat mat = srcMat.clone();
	cv::Rect rect(0,0,mat.cols,mat.cols);
	try
	{
		cv::grabCut(mat, grabResult, rect, bgdModel, fgdModel, 1, cv::GC_INIT_WITH_MASK);
		for (int i = 0; i < grabResult.rows; i++)
		{
			for (int j = 0; j < grabResult.cols; j++)
			{
				auto val = grabResult.at<uchar>(i, j);
				if (val == cv::GC_BGD)
				{
					ui.imageViewerWidget->imgObjs[2]->displayImg.setPixel(j, i, markBackgroundColor);
				}
				else if (val == cv::GC_FGD)
				{
					ui.imageViewerWidget->imgObjs[2]->displayImg.setPixel(j, i, markFroegroundColor);
				}
				else if (val == cv::GC_PR_BGD)
				{
					ui.imageViewerWidget->imgObjs[2]->displayImg.setPixel(j, i, markBackgroundColor);
				}
				else if (val == cv::GC_PR_FGD)
				{
					ui.imageViewerWidget->imgObjs[2]->displayImg.setPixel(j, i, markFroegroundColor);
				}
			}
		}
		ui.imageViewerWidget->setImgObjHide(2, false);
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		QMessageBox::critical(this, "ERROR", "openCV grabCub fail, possible due to selection only mark image as all-foreground or all-background");
	}
	return grabResult;
}

void Qt_Image_Marker::showEvent(QShowEvent* event)
{
	ui.radioButton_markBackground->setChecked(true);
	link_select_background_pen(true);
	ui.lineEdit_strokeWidth->setText(QString::number(strokeWidthMin));
	link_lineedit_text_change2StrokeWidth(ui.lineEdit_strokeWidth->text());
}

void Qt_Image_Marker::closeEvent(QCloseEvent *event)
{
	closeImages();
	event->accept(); //close window
}

