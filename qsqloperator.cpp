#include "qsqloperator.h"
#include <QDebug>
QSqlOperator::QSqlOperator()
{

}

bool MySQLDatabase::connect(const QString &host, const QString &databaseName, const QString &username, const QString &password)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(host);
    db.setDatabaseName(databaseName);
    db.setUserName(username);
    db.setPassword(password);
    return db.open();
}

void MySQLDatabase::disconnect()
{
    db.close();
}

bool MySQLDatabase::isConnected() const
{
    return db.isOpen();
}

QSqlQuery MySQLDatabase::executeQuery(const QString &query)
{
    QSqlQuery q;
    if (!q.exec(query)) {
        qDebug() << "Query execution error:" << q.lastError();
    }
    return q;
}

bool SQLServerDatabase::connect(const QString &host, const QString &databaseName, const QString &username, const QString &password)
{
    db = QSqlDatabase::addDatabase("QODBC");
    QString dsn = QString("Driver={SQL Server};Server=%1;Database=%2;Uid=%3;Pwd=%4;")
                    .arg(host)
                    .arg(databaseName)
                    .arg(username)
                    .arg(password);
    db.setDatabaseName(dsn);
    return db.open();
}

void SQLServerDatabase::disconnect()
{
    db.close();
}

QSqlQuery SQLServerDatabase::executeQuery(const QString &query)
{
    QSqlQuery q;
    if (!q.exec(query)) {
        qDebug() << "Query execution error:" << q.lastError();
    }
    return q;
}

bool SQLServerDatabase::isConnected() const
{
    return db.isOpen();
}
