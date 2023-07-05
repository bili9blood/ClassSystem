//
// Created by 35012 on 2023/7/3.
//

#include "DownloadWorker.h"

#include <utility>

DownloadWorker::DownloadWorker(QString url, QString name, int id,
                               QObject *parent)
    : QObject(parent),
      QRunnable(),
      mName(std::move(name)),
      mUrl(std::move(url)),
      mId(id) {
  setAutoDelete(true);
}

void DownloadWorker::run() {
  qDebug() << "id:" << mId << "thread:" << QThread::currentThread();
  auto process = new QProcess;
  process->setProgram("wget.exe");
  process->setProcessChannelMode(QProcess::MergedChannels);
  process->setArguments({"-P", "tmp", mUrl});
  connect(process, &QProcess::readyRead, [process, this] {
    QString out = process->readAll();
    if (out.contains('%')) {
      QString progress;
      for (int i = out.lastIndexOf('%') - 1; out[i] >= '0' && out[i] <= '9';
           --i)
        progress.insert(0, out[i]);
      emit getProgress(mId, progress.toInt());
    }
  });
  connect(process,
          QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
          [this] { emit finishDownloading(mId); });
  process->start(QProcess::ReadOnly);
  QEventLoop loop;
  connect(this, &DownloadWorker::finishDownloading, &loop, &QEventLoop::quit);
  loop.exec();
}
