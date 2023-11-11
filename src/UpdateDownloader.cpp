#include "UpdateDownloader.h"

#include <qnetworkrequest.h>
#include <qprocess.h>

UpdateDownloader::UpdateDownloader(QObject *parent) : QObject(parent) {}

void UpdateDownloader::download() {
  QUrl url = "http://%1:%2/latest/LATEST"_s.arg(cs::settings::serverHost)
                 .arg(cs::settings::httpServerPort);
  auto reply = m_nam.get(QNetworkRequest(url));

  m_latestFile.open(QFile::WriteOnly | QFile::Truncate);

  connect(
      reply, &QNetworkReply::downloadProgress,
      [this](qint64 bytesReceived, qint64 bytesTotal) {
        emit progress(ushort((qreal)bytesReceived / (qreal)bytesTotal * 100.0));
      });

  connect(reply, &QNetworkReply::readyRead,
          [this, reply] { m_latestFile.write(reply->readAll()); });

  connect(reply, &QNetworkReply::finished, [this, reply] {
    while (!reply->atEnd()) m_latestFile.write(reply->readAll());

    if (reply->error() != QNetworkReply::NoError) [[unlikely]]
      return;

    QProcess::startDetached("Updater.exe",
                            {QString::number(qApp->applicationPid())});
  });
}