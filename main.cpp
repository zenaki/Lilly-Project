#include "logindialog.h"
#include <QApplication>
//#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qApp->setStyleSheet( "QStatusBar::item { border: 0px}" ) ;
    LoginDialog l;
    l.show();
//    w.setGeometry(
//        QStyle::alignedRect(
//            Qt::LeftToRight,
//            Qt::AlignCenter,
//            w.size(),
//            qApp->desktop()->availableGeometry()
//        )
//    );

    return a.exec();
}
