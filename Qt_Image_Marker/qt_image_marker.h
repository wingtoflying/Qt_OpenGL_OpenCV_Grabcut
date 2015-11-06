#ifndef QT_IMAGE_MARKER_H
#define QT_IMAGE_MARKER_H

#include <QtWidgets/QMainWindow>
#include <QSharedPointer>
#include <QFileSystemModel>
#include "ui_qt_image_marker.h"
#include "cqtopencvviewergl.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp> 


class Qt_Image_Marker : public QMainWindow
{
	Q_OBJECT

public:
	Qt_Image_Marker(QWidget *parent = 0);
	~Qt_Image_Marker();
	cv::Mat img;
	QImage mat2QImage(cv::Mat src);
	cv::Mat QImage2Mat(QImage src);
	cv::Mat graphCut(QImage maskImg);
	void deleteAction(QAction* obj);
	void showEvent(QShowEvent* event);//overload for initial UI
	void closeEvent(QCloseEvent *event);// overload for save image

private:
	Ui::Qt_Image_MarkerClass ui;
	QRgb markFroegroundColor;
	QRgb markBackgroundColor;
	QAction* action_switchShowMark;
	QAction* action_switchShowResult;
	QAction* action_grabCut;
	QAction* action_keyaddStrokeWidth;
	QAction* action_keysubStrokeWidth;
	QAction* action_cleanAllMark;
	QAction* action_changeStrockWidth;
	QAction* action_undoDraw;
	int strokeWidthMax;
	int strokeWidthMin;
	QFileSystemModel* fileModel;
	QString folderNameMarker;
	QString folderNameFG;
	QFileInfo fileInfoSrc;
	QFileInfo fileInfoMarker; // remember for save
	QFileInfo fileInfoFG; // remember for save
	QString suffixOfSaveMarker;// for marker save image type
	QString suffixOfSaveResult;// for fg save image type
	cv::Mat grabResult;
	QSize oriImageSize; // save original image size
public slots:
	void show_coordinate(QPoint p);
	void link_action_switchShowMark();
	void link_action_switchShowResult();
	void link_action_grabCut();
	void link_select_foreground_pen(bool checked);
	void link_select_background_pen(bool checked);
	void link_select_clen_pen(bool checked);
	void link_lineedit_text_change2StrokeWidth(const QString& text);
	void link_lineedit_text_add_StrokeWidth();
	void link_lineedit_text_sub_StrokeWidth();
	void link_action_clean_mark();
	void link_action_undoDraw();
	void closeImages();
	void openFolder();

	void selectFilefromList(const QModelIndex & current);
	bool openImage(const QString& filepath);
	QImage loadAccessImgs(const QFileInfo &testFileInfo, QString subFolderName, QString suffixOfFile, QImage testImg, QFileInfo &subFileInfo, bool tryload);
	QImage loadOrMakeImg(const QString& filename, const QImage &refImage);
	QImage makeImg(const QSize imgSize);
	//return ARGB888 image, load image in filename, if not exist or wrong size, generate new one
};

#endif // QT_IMAGE_MARKER_H
