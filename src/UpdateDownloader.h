#pragma once

#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>

class UpdateDownloader : public QObject {
  Q_OBJECT
 public:
  explicit UpdateDownloader(QObject *parent = nullptr);

 signals:
  void progress(ushort progress);

 public slots:
  void download();

 private:
  QNetworkAccessManager m_nam;
  QFile m_latestFile = QFile("LATEST");
};