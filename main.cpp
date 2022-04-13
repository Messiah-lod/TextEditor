
#include "texteditor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("fusion"));
    a.setWindowIcon(QIcon(":/Icons/Icons/main.png"));

    TextEditor w;
    w.setMinimumHeight(600);
    w.setMinimumWidth(800);
    w.show();

    return a.exec();
}
