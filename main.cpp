#include "mainwindow.h"

#include <QApplication>
#include "qsqloperator.h"
#include "mlogger.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SQLServerDatabase sqlservertest;
    sqlservertest.connect("127.0.0.1","Scale","sa","193cl193");
    QString res = QString::number(sqlservertest.isConnected());
    LOG_DEBUG(res);

    MainWindow w;
    w.show();
    return a.exec();
}
