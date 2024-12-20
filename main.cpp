#include "src/ui/mainWindow.h"
#include <QApplication>
#include <QIcon>

void initSystemService();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":res/ico.png"));
    initSystemService();
    CMainWindow w;
    w.show();
    return a.exec();
}