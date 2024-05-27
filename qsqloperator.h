#ifndef QSQLOPERATOR_H
#define QSQLOPERATOR_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class QSqlOperator
{
public:
    QSqlOperator();
    virtual bool connect(const QString& host,const QString& databaseName,const QString& username, const QString& password) = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() const = 0;
    virtual QSqlQuery executeQuery(const QString& query) = 0;
public:
    QSqlDatabase db;
};

class MySQLDatabase : public QSqlOperator {
public:
    bool connect(const QString& host, const QString& databaseName, const QString& username, const QString& password) override;

    void disconnect() override ;

    bool isConnected() const override ;

    QSqlQuery executeQuery(const QString& query) override ;
};

class SQLServerDatabase : public QSqlOperator {
public:
    bool connect(const QString& host, const QString& databaseName, const QString& username, const QString& password) override ;

    void disconnect() override ;

    bool isConnected() const override ;

    QSqlQuery executeQuery(const QString& query) override ;
};

#endif // QSQLOPERATOR_H
