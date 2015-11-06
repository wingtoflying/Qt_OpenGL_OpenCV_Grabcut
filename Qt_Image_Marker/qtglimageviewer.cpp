#include "qtglimageviewer.h"
#include <QOpenGLFunctions>
#include <QMessageBox>
#include <math.h>

ImageObject::ImageObject()
{
	translucency = 1.0f;
	isHide = false;
}
ImageObject::~ImageObject()
{
	texture->destroy();
	delete texture;
}
void ImageObject::create_vertex(QImage img, float z)
{
	this->displayImg = img;
	this->width = img.width();
	this->height = img.height();
	this->whRatio = (float)(this->width) / (float)(this->height);
	vertData.clear();
	int hW = this->width / 2;
	int hW2 = this->width - hW;
	int hH = this->height / 2;
	int hH2 = this->height - hH;
	const float coor[4][4] = { { +1, -1, 1, 0 }, { -1, -1, 0, 0 }, { -1, +1, 0, 1 }, { +1, +1, 1, 1 } };
	for (int i = 0; i < 4; i++)
	{
		//vertex
		int w = coor[i][0] > 0?hW2 : hW;
		int h = coor[i][1] > 0?hH2 : hH;
		//vertData.append(w*(coor[i][0] + z)); //DEBUG for check layer
		vertData.append(w*(coor[i][0])); //x
		vertData.append(h*coor[i][1]); //y
		vertData.append(z); 
		//texture
		vertData.append(coor[i][2]); //s
		vertData.append(coor[i][3]); //t
	}
	this->texture = new QOpenGLTexture(img, QOpenGLTexture::DontGenerateMipMaps);
	this->texture->setMagnificationFilter(QOpenGLTexture::Nearest);
	this->texture->setMinificationFilter(QOpenGLTexture::Nearest);
}

void ImageObject::updateImg()
{
	texture->setData(this->displayImg, QOpenGLTexture::DontGenerateMipMaps);
}

void ImageObject::changeImg(QImage newImg)
{
	if (newImg.width() == this->displayImg.width() && newImg.height() == this->displayImg.height())
	{
		displayImg = newImg;
		this->updateImg();
	}
	else
	{
		qDebug() << "Size of newImg is different from original";
	}
}

QtGlImageviewer::QtGlImageviewer(QWidget *parent) :QOpenGLWidget(parent)
{
	shift_x = 0;
	shift_y = 0;
	scale = 1;
	this->setMouseTracking(true);// for tracking mouse position all the time
	painter = nullptr;
	drawTarget = nullptr;
}
QtGlImageviewer::~QtGlImageviewer()
{
	this->vao.destroy();
	shaderProgram->destroyed();
	delete shaderProgram;
	this->vbo.destroy();
	if (painter != nullptr)
		delete painter;
}

QMatrix4x4 QtGlImageviewer::getMatrix()
{
	QMatrix4x4 m;
	m.ortho(-this->glWidth / 2, this->glWidth - (this->glWidth / 2), this->glHeight / 2, -this->glHeight + this->glHeight / 2, -10.0f, 150.0f);
	m.scale(this->scale);
	m(0, 3) += this->shift_x;
	m(1, 3) += this->shift_y;
	return m;
}

void QtGlImageviewer::addImageObject(QImage src, float z)
{
	this->makeCurrent();
	ImageObject* imgObj = new ImageObject();
	imgObj->create_vertex(src, z);
	this->imgObjs.append(imgObj);
	refreshVBO();
	update();
	paintGL();
}

void QtGlImageviewer::cleanImageObjects()
{
	imgObjs.clear();
	if (painter != nullptr)
		delete painter;
	this->makeCurrent();
	refreshVBO();
	update();
	paintGL();
}

void QtGlImageviewer::setImgObjHide(int num, bool state)
{
	if (num < imgObjs.size())
	{
		ImageObject* imgObj =  this->imgObjs[num];
		imgObj->isHide = state;
	}
	this->refreshVBO();
}

void QtGlImageviewer::setDrawTarget(ImageObject* ptr)
{
	this->drawTarget = ptr;
	this->painter = new QPainter(&(ptr->displayImg));
	updatePainter();
}

void QtGlImageviewer::setDrawColor(QColor c)
{
	painterDescribtion.bush = QBrush(c);
	painterDescribtion.mode = QPainter::CompositionMode_Source;
	updatePainter();
}

void QtGlImageviewer::setDrawTranspancy()
{
	painterDescribtion.bush = QBrush(qRgba(0, 0, 0, 0));
	painterDescribtion.mode = QPainter::CompositionMode_Clear;
	updatePainter();
}

void QtGlImageviewer::updatePainter()
{
	if (this->painter != nullptr)
	{
		this->painter->setBrush(painterDescribtion.bush);
		this->painter->setPen(Qt::NoPen);// for board-less
		this->painter->setCompositionMode(painterDescribtion.mode);
	}

}

void QtGlImageviewer::setDrawRadius(qreal r)
{
	this->drawRadius = r;
}

void QtGlImageviewer::drawCircle(QPoint p)
{
	if (painter != nullptr)
	{
		painter->drawEllipse(p, (int)this->drawRadius, (int)this->drawRadius);
		this->drawTarget->updateImg();
	}
}

void QtGlImageviewer::undoDraw()
{
	if (painter != nullptr)
	{
		//TODO undo function
		//should be implement by multi-layer
	}
}

void QtGlImageviewer::initializeGL()
{
	initializeOpenGLFunctions();
	initialShader();

	glDisable(GL_DEPTH_TEST); //for transparency image
	glEnable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glWidth = this->width();
	glHeight = this->height();
}
void QtGlImageviewer::initialShader()
{
#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1
	QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex);
	const char *vsrc =
		"attribute highp vec4 vertex;\n"
		"attribute mediump vec4 texCoord;\n"
		"varying mediump vec4 texc;\n"
		"uniform mediump mat4 matrix;\n"
		"void main(void)\n"
		"{\n"
		"    gl_Position = matrix * vertex;\n"
		"    texc = texCoord;\n"
		"}\n";
	vshader->compileSourceCode(vsrc);

	QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment);
	const char *fsrc =
		"uniform sampler2D texture;\n"
		"varying mediump vec4 texc;\n"
//		"uniform float translucency;\n"
		"void main(void)\n"
		"{\n"
		"    gl_FragColor = texture2D(texture, texc.st);\n"
//		"	 gl_FragColor.a = translucency;\n"
		"}\n";
	fshader->compileSourceCode(fsrc);

	shaderProgram = new QOpenGLShaderProgram();
	shaderProgram->addShader(vshader);
	shaderProgram->addShader(fshader);
	shaderProgram->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
	shaderProgram->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
	shaderProgram->link();
	shaderProgram->bind();
	shaderProgram->setUniformValue("texture", 0);

}

void QtGlImageviewer::refreshVBO()
{
	vecData.clear();
	if (imgObjs.size() > 0)
	{
		for (auto imgObj : imgObjs)
		{
			if (!imgObj->isHide)
				vecData.append(imgObj->vertData);
		}
		if (!vbo.isCreated())
		{
			vbo.create();
		}
		vbo.bind();
		vbo.allocate(vecData.constData(), vecData.count()*sizeof(GLfloat));
	}
	else
		vbo.release();
}

void QtGlImageviewer::paintGL()
{
	//background color
	float r = 0.5f;
	float g = 0.5f;
	float b = 0.5f;
	glClearColor(r, g, b, 0.5f);
	if (imgObjs.size() > 0)
	{
		QMatrix4x4 m = getMatrix();
		shaderProgram->setUniformValue("matrix", m);
		shaderProgram->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
		shaderProgram->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
		shaderProgram->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
		shaderProgram->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

		for (auto imgObjP = imgObjs.begin(); imgObjP < imgObjs.end(); imgObjP++)
		{
			if (!(*imgObjP)->isHide)
			{
				auto imgObj = *imgObjP;
				imgObj->texture->bind();
				glDrawArrays(GL_TRIANGLE_FAN, (imgObjP - imgObjs.begin()) * 4, 4); //each image have 4 point
			}
		}
	}
	update();

}
void QtGlImageviewer::resizeGL(int width, int height)
{
	glWidth = width;
	glHeight = height;
	glViewport(0, 0, width, height);
}
void QtGlImageviewer::mousePressEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::RightButton)
		lastPressPoint = event->pos();
}
void QtGlImageviewer::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - lastPressPoint.x();
	int dy = event->y() - lastPressPoint.y();
	if (event->buttons() & Qt::LeftButton)
	{
		this->setFocusPolicy(Qt::StrongFocus);
		this->setFocus();
	}
	static bool mouseIsPressed = false;
	if (imgObjs.size() > 0)
	{// at least one image exist
		QMatrix4x4 m = getMatrix().inverted();
		float xf = (float)event->x() / glWidth * 2 - 1.0f;
		float yf = (float)event->y() / glHeight * 2 - 1.0f;
		QVector4D o = m.map(QVector4D(xf, -yf, 0, 1.0f)); // -yf for OpenGL y coordinate direction
		int img_x = (int)(o.x() + (float)imgObjs[0]->width / 2);
		// (int) after shift by width or   -1 < x < 1 would equal to 0
		int img_y = (int)(o.y() + (float)imgObjs[0]->height / 2);
		QPoint mousePos(img_x, img_y);
		emit mouseCoorOnImgSignal(mousePos);

		if (event->buttons() & Qt::LeftButton) 
		{
			if (this->drawTarget != nullptr)
			{
				this->drawCircle(mousePos);
			}
		}
	}
	if (event->buttons() & Qt::RightButton) {
		shift_x += (float)dx / glWidth*2;
		shift_y -= (float)dy / glHeight*2; // mouse y coordinate is different from OpenGL
	}
	lastPressPoint = event->pos();
	update();
}
void QtGlImageviewer::mouseReleaseEvent(QMouseEvent * /* event */)
{
//	emit clicked();
}
void QtGlImageviewer::wheelEvent(QWheelEvent *event)
{
	QPoint numDegrees = event->angleDelta() / 8;
	if (!numDegrees.isNull())
	{
		float angle = numDegrees.y();
		const float r = 1.5;
		if (this->zoomInScreenCenterRotate)
		{
			float ratio = (angle > 0) ? r : 1.0f / r; //angle > 0 -> rotate forward
			this->scale *= ratio;
			this->shift_x *= ratio;
			this->shift_y *= ratio;
		}
		else
		{//rotate by mouse center
			QPointF mouseP = event->posF();
			QPointF disC = mouseP - QPointF(this->glWidth / 2, this->glHeight / 2);
			QPointF disC_GL = QPointF( disC.x()/(this->glWidth / 2), disC.y()/(this->glHeight / 2));
			float shift_x_t = this->shift_x - disC_GL.x();
			float shift_y_t = this->shift_y + disC_GL.y();
			float ratio = (angle > 0) ? r : 1.0f / r; //angle > 0 -> rotate forward
			this->scale *= ratio;
			shift_x_t *= ratio;
			shift_y_t *= ratio;
			this->shift_x = shift_x_t + disC_GL.x();
			this->shift_y = shift_y_t - disC_GL.y();
		}
		update();
	}
	event->accept();
}