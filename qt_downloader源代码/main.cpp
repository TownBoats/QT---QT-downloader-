#include "widget.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("Qthttp下载器");
    w.setWindowIcon(QIcon(":/images/Download.png"));
    w.show();
    return a.exec();
}
