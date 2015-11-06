#include <QtWidgets/QMainWindow>
#include <QLineEdit>
#include <QValidator>
#include <QKeyEvent>
#include <QMessageBox>
#include <QApplication>

class LineEdit_K : public QLineEdit
{
	//line edit for send non valid key to upper class
	Q_OBJECT
signals:
	void pressEnter();
public:
	LineEdit_K(QWidget *parent) :QLineEdit(parent)
	{
	};
	void keyPressEvent(QKeyEvent *event)
	{
		int pos = 0;
		QString s = event->text();
		//if (this->validator()->validate(event->text(), pos) == QValidator::Invalid)
		if ((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return))
		{
			//QMessageBox::information(this, "S", s);
			event->ignore();
			//-QApplication::sendEvent(parent(), event);
			emit pressEnter();
		}
		QLineEdit::keyPressEvent(event);
	}
	void focusInEvent(QFocusEvent * event)
	{
		this->selectAll();
	}
};