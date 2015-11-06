#ifndef QTGLIMAGEVIEWER_H
#define QTGLIMAGEVIEWER_H

#include <QOpenGLWidget>
#include <QMouseEvent>
#include <opencv2/core/core.hpp>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QPainter>
#include <QBrush>

struct GLVertex
{
	float x, y, z;
	float u, v;
};
/*
Save all need data in plane object
*/

class ImageObject
{
public:
	ImageObject();
	~ImageObject();
	QOpenGLBuffer v_vertex;
	//QOpenGLVertexArrayObject vao;
	QOpenGLTexture* texture;
	//QOpenGLShaderProgram* shaderProgram;
	int width;
	int height;
	float whRatio;
	float translucency;
	QImage displayImg;
	void create_vertex(QImage img, float z);
	QVector<GLfloat> vertData;
	bool isHide;
	void updateImg();
	void changeImg(QImage newImg);
};
class DrawPainterDescribtion
{
public:
	DrawPainterDescribtion()
	{
		bush = QBrush(qRgba(0, 0, 255, 255));
		mode = QPainter::CompositionMode_Source;
	};
	QBrush bush;
	QPainter::CompositionMode mode;
};
class QtGlImageviewer : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	QtGlImageviewer(QWidget *parent = 0);
	~QtGlImageviewer();
	QVector<ImageObject*> imgObjs;
	QMatrix4x4 getMatrix();
signals:
	void mouseCoorOnImgSignal(QPoint p);
public slots :
		//bool setImage(cv::Mat image); //set cv::Mat
	void addImageObject(QImage src, float z);
	void cleanImageObjects();
	void setImgObjHide(int num,bool state);
	void setDrawTarget(ImageObject* ptr);
	void setDrawColor(QColor c);
	void setDrawTranspancy();
	void updatePainter(); // update pen description to painter setting
	void setDrawRadius(qreal r);
	void drawCircle(QPoint p);
	void undoDraw();

protected:
	void initializeGL() Q_DECL_OVERRIDE; // OpenGL initialization
	void paintGL() Q_DECL_OVERRIDE; // OpenGL Rendering
	void resizeGL(int width, int height) Q_DECL_OVERRIDE; // call as widget size change
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
private:
	//QPaint
	ImageObject* drawTarget;
	QPainter* painter;
	DrawPainterDescribtion painterDescribtion;
	qreal drawRadius;

	//OpenGL
	QOpenGLVertexArrayObject vao;
	QOpenGLBuffer vbo;
	QOpenGLShaderProgram* shaderProgram;
	QVector<GLfloat> vecData;
	void initialShader();
	void refreshVBO();

	const bool zoomInScreenCenterRotate = false; // false for mouse center zoom in
	float shift_x;
	float shift_y;
	int glWidth;
	int glHeight;
	float scale;

	QPoint lastPressPoint;
};
#endif