#ifndef C2DQT_H
#define C2DQT_H

#include <QtGui/QMainWindow>
#include "ui_c2dqt.h"

class c2dqt : public QMainWindow
{
	Q_OBJECT

public:
	c2dqt(QWidget *parent = 0, Qt::WFlags flags = 0);
	~c2dqt();

private:
	Ui::c2dqtClass ui;
};

#endif // C2DQT_H
