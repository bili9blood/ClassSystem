//
// Created by 35012 on 2023/7/2.
//

#include "Updater.h"
Updater::Updater(QObject* parent) : QObject(parent) {
  mNam = new QNetworkAccessManager(this);
  QSslConfiguration conf = mRequest.sslConfiguration();
  conf.setPeerVerifyMode(QSslSocket::VerifyNone);
  conf.setProtocol(QSsl::TlsV1SslV3);
  mRequest.setSslConfiguration(conf);
  mProcess.setProgram("wget.exe");
  mProcess.setProcessChannelMode(QProcess::MergedChannels);
  QThreadPool::globalInstance()->setMaxThreadCount(QThread::idealThreadCount());
}
void Updater::check() {
  mRequest.setUrl(giteeGetLatestReleaseApiUrl);
  auto reply = mNam->get(mRequest);
  QEventLoop loop;
  connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
  loop.exec();
  int statusCode =
      reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
  if (statusCode < 200 || statusCode >= 300 ||
      reply->error() != QNetworkReply::NoError) {
    QMessageBox::warning(nullptr, "自动更新",
                         QString("班级系统自动更新程序无法获取远程版本信息，自"
                                 "动更新中止。\n\n错误原因：%1 %2")
                             .arg(statusCode)
                             .arg(reply->errorString()),
                         QMessageBox::Ok);
    qDebug() << reply->readAll();
    emit checked();
    return;
  }
  auto jsonErr = QJsonParseError();
  auto replyJson = QJsonDocument::fromJson(reply->readAll(), &jsonErr);
  if (jsonErr.error != QJsonParseError::NoError) {
    QMessageBox::warning(nullptr, "自动更新",
                         "班级系统自动更新程序无法解析远程版本信息，自动更新中"
                         "止。\n\n错误原因：" +
                             jsonErr.errorString(),
                         QMessageBox::Ok);
    emit checked();
    return;
  }
  auto remoteVersion = replyJson.object().value("tag_name").toString();
  if (remoteVersion <= projectVersion) {
    // everything up-to-date
    emit checked();
    return;
  }
  if (!QFile::exists("wget.exe")) {
    QMessageBox::warning(nullptr, "自动更新",
                         "班级系统自动更新程序无法找到wget.exe，自动更新中止。",
                         QMessageBox::Ok);
    emit checked();
    return;
  }
  QDir::current().mkdir("tmp");
  auto assets = replyJson.object().value("assets").toArray();
  UpdateDialog dlg;
  QList<DownloadWorker*> workers;
  qDebug() << "main:"
           << "thread:" << QThread::currentThread();
  for (const auto val : assets) {
    if (val.toObject().keys().contains("name")) {
      ++mCount;
      auto url = val.toObject().value("browser_download_url").toString();
      auto name = val.toObject().value("name").toString();
      int id;
      dlg.appendFile(name, &id);
      qDebug() << "id:" << id;
      auto w = new DownloadWorker(url, name, id);
      connect(w, &DownloadWorker::getProgress, &dlg,
              &UpdateDialog::updateProgress);
      connect(w, &DownloadWorker::finishDownloading, &dlg,
              &UpdateDialog::removeFile);
      connect(w, &DownloadWorker::finishDownloading, [this] { --mCount; });
      QThreadPool::globalInstance()->start(w);
      workers << w;
    }
  }
  while (true) {
    if (mCount == 0) break;
    QApplication::processEvents();
  }
  dlg.close();
  // decode file name and move
  auto tmpDir = QDir("tmp");
  auto files = tmpDir.entryList(QDir::Files);
  for (const auto& i : files) {
    auto dest = QDir(QDir::currentPath() + "/tmp");
    auto n = i;
    while (n.contains("--")) {
      dest.mkpath(n.left(n.indexOf("--")));
      dest.cd(n.left(n.indexOf("--")));
      n = n.remove(0, n.indexOf("--") + 2);
    }
    auto file = QFile("tmp/" + i);
    file.copy(dest.absolutePath() + "/" + n) && file.remove();
  }
  QApplication::quit();
  system(
      R"(powershell Copy-Item -Path "tmp/*" -Destination "./" -Recurse -force;Remove-Item "tmp" -Recurse -Force)");
}
