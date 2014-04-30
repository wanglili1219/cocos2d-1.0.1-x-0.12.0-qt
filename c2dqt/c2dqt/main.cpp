#include "c2dqt.h"
#include <QtGui/QApplication>
#include <QtGui/QHBoxLayout>
#include <QtGui/qlistwidget>
#include<QStandardItemModel>                                                                        

#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "ui_c2dqt.h"

#include "TileProperyDelegate.h"
#include "Cocos2dWidget.h"

USING_NS_CC;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);  
      
    QHBoxLayout *mainLayout = new QHBoxLayout ;   
    QVBoxLayout *rightLayout = new QVBoxLayout;  
    QVBoxLayout *leftLayout = new QVBoxLayout;  
  
	QWidget *mainWindow = new QWidget();  
    QMainWindow * uiParent = new QMainWindow(); 

    Cocos2dWidget * pCocosWidget = new Cocos2dWidget();  
   
    Ui_c2dqtClass *ui = new Ui_c2dqtClass();    
    ui->setupUi(uiParent);  
	
    mainLayout->addWidget(pCocosWidget, 0, Qt::AlignTop);
    mainLayout->addWidget(uiParent);
    mainWindow->resize(700, 500);
    mainWindow->setLayout(mainLayout); 

    TileProperyDelegate* delegate = new TileProperyDelegate();
    ui->tableView->setItemDelegate(delegate);

    QStandardItemModel* mItemModel = new QStandardItemModel(20, 2);
    QStandardItem *nameit = new QStandardItem("name");  
    QStandardItem *numit = new QStandardItem("TileNum"); 
    QStandardItem *blockit = new QStandardItem("Block");  
    
    mItemModel->setItem(0, 0, nameit);
    mItemModel->setItem(1, 0, numit);
    mItemModel->setItem(2, 0, blockit);

    ui->tableView->setModel(mItemModel);

    QSize iconsize(200, 200);
    ui->listWidget->setIconSize(iconsize);
    ui->listWidget->setMinimumHeight(iconsize.height());
	QObject::connect(ui->listWidget, SIGNAL(currentRowChanged(int)), pCocosWidget, SLOT(curveChanged(int)));

	  QPixmap pix(iconsize);
       pix.load("mouse.png");
	QListWidgetItem *item = new QListWidgetItem;
	 item->setText("common");
	  item->setIcon(pix);
     ui->listWidget->addItem(item);

    for (int i = 1; i < 6; ++i){
        QListWidgetItem *item = new QListWidgetItem;
    
        QString imagename = QString("map%1").arg(i);
        QPixmap pix(iconsize);
        pix.load(imagename);

        item->setIcon(pix);
        item->setText(imagename);
        ui->listWidget->addItem(item);
    }

	pCocosWidget->initCocos2d();
    mainWindow->show();
    app.exec();

	cocos2d::CCDirector::sharedDirector()->end();
    cocos2d::CCDirector::sharedDirector()->mainLoop();

    return 0;
}
