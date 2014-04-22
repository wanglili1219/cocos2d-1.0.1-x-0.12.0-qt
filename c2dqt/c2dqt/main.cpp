#include "c2dqt.h"
#include <QtGui/QApplication>
#include <QtGui/QHBoxLayout>
#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"

#include "Cocos2dWidget.h"


USING_NS_CC;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);  
      
    QHBoxLayout *mainLayout = new QHBoxLayout ;   
    QVBoxLayout *rightLayout = new QVBoxLayout;  
    QVBoxLayout *leftLayout = new QVBoxLayout;  
  
    Cocos2dWidget * pCocosWidget = new Cocos2dWidget();  

    QWidget *testMainWindow = new QWidget();  
    QMainWindow * pButtonWidget = new QMainWindow();  
    QPushButton helloButton("click me");
    QPushButton testbtn1("test1");
    QPushButton testbtn2("test2");
    QPushButton testbtn3("test3");
      
    testMainWindow->setGeometry(QRect(1024 * 0.5, 768 * 0.5,1024, 768));  
    rightLayout->addWidget(pButtonWidget,0,Qt::AlignBottom);  

    rightLayout->setDirection(QBoxLayout::TopToBottom);  
    leftLayout->addWidget(pCocosWidget,1,Qt::AlignTop);  
    leftLayout->addWidget(&helloButton,1,Qt::AlignCenter); 
    leftLayout->addWidget(&testbtn1,1,Qt::AlignCenter);  
    leftLayout->addWidget(&testbtn2,1,Qt::AlignCenter);  
    leftLayout->addWidget(&testbtn3,1,Qt::AlignCenter);  


    mainLayout->addLayout(leftLayout);  
    mainLayout->addLayout(rightLayout);  
    testMainWindow->setLayout(mainLayout);  

    pCocosWidget->initCocos2d();
    testMainWindow->show();  
      
    QObject::connect(&helloButton, SIGNAL(clicked()),&a,SLOT(quit()));  
    QObject::connect(&testbtn1, SIGNAL(clicked()), pCocosWidget,SLOT(translate()));  
    QObject::connect(&testbtn2, SIGNAL(clicked()), pCocosWidget,SLOT(particleDemo()));  
    QObject::connect(&testbtn3, SIGNAL(clicked()), pCocosWidget,SLOT(effectDemo()));  
    a.exec();  

	cocos2d::CCDirector::sharedDirector()->end();
    cocos2d::CCDirector::sharedDirector()->mainLoop();
    return 0;
        
}
