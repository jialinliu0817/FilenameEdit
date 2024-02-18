#include "filenameedit.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FilenameEdit w;
    w.show();
    return a.exec();
}
