//
// Created by 35012 on 2023/7/2.
//

#ifndef CLASSSYSTEM_UPDATER_H
#define CLASSSYSTEM_UPDATER_H
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcess>
#include <QSharedPointer>
#include <QThread>
#include <QThreadPool>

#include "Core/DownloadWorker.h"
#include "Widgets/UpdateDialog.h"
class Updater : public QObject {
  Q_OBJECT
 public:
  explicit Updater(QObject *parent = nullptr);
 public slots:
  void check();
 signals:
  void checked();
  void getFileName(const QString &fileName);

 private:
  QNetworkAccessManager *mNam;
  QNetworkRequest mRequest;
  QProcess mProcess;
  int mCount = 0;
};

#endif  // CLASSSYSTEM_UPDATER_H
