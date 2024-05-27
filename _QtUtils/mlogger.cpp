#include "mlogger.h"

MLogger::MLogger()
{
    QString currentDate = QDate::currentDate().toString("yyyy-MM-dd");
    openLogFile(currentDate);
}

MLogger::~MLogger()
{
    if (logFile.isOpen()) {
        logFile.close();
    }
}

MLogger &MLogger::getInstance()
{
    static MLogger instance;
    return instance;
}

void MLogger::printLog(MLogger::LogLevel level, const QString &message, const char *file, int line)
{
    QMutexLocker locker(&mutex);
    QString logMessage = formatLogMessage(level, message, file, line);
    QString currentDate = QDate::currentDate().toString("yyyy-MM-dd");
    if (currentDate != lastLogDate) {
        openLogFile(currentDate);
    }
    QTextStream out(&logFile);
    out << logMessage << "\n";
    out.flush();
}

void MLogger::openLogFile(const QString &date)
{
    if (logFile.isOpen()) {
                logFile.close();
    }

    QString logDirPath = "./../logs";
    QDir logDir;
    if (!logDir.exists(logDirPath)) {
        logDir.mkpath(logDirPath);
    }

    QString logFileName = QString("%1/log_%2.log").arg(logDirPath).arg(date);
    logFile.setFileName(logFileName);
    if (!logFile.open(QIODevice::Append | QIODevice::Text)) {
        qWarning("Cannot open log file for writing");
    } else {
        lastLogDate = date;
    }
}

QString MLogger::formatLogMessage(MLogger::LogLevel level, const QString &message, const char *file, int line)
{
    QString levelStr;
    switch (level) {
        case DEBUG:   levelStr = "DEBUG"; break;
        case INFO:    levelStr = "INFO"; break;
        case WARNING: levelStr = "WARNING"; break;
        case ERROR:   levelStr = "ERROR"; break;
    }

    QString logMessage = QString("[%1] [%2] [%3:%4] %5")
                         .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
                         .arg(levelStr)
                         .arg(file)
                         .arg(line)
                         .arg(message);

    return logMessage;
}


