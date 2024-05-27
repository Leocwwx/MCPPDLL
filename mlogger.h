#ifndef MLOGGER_H
#define MLOGGER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <QMutexLocker>
#include <QDir>

class MLogger
{
public:
enum LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };
    static MLogger& getInstance();
    void printLog(LogLevel level, const QString& message, const char* file, int line);

private:
    QFile logFile;
    QMutex mutex;
    QString lastLogDate;

private:
    MLogger();
    ~MLogger();
    void openLogFile(const QString& date);
    QString formatLogMessage(LogLevel level, const QString& message, const char* file, int line);

    MLogger(const MLogger&) = delete;     //禁用拷贝构造
    MLogger& operator=(const MLogger&) = delete;      //禁用赋值运算
};

#define LOG_DEBUG(message) MLogger::getInstance().printLog(MLogger::DEBUG, message, __FILE__, __LINE__)
#define LOG_INFO(message) MLogger::getInstance().printLog(MLogger::INFO, message, __FILE__, __LINE__)
#define LOG_WARNING(message) MLogger::getInstance().printLog(MLogger::WARNING, message, __FILE__, __LINE__)
#define LOG_ERROR(message) MLogger::getInstance().printLog(MLogger::ERROR, message, __FILE__, __LINE__)

#endif // MLOGGER_H
