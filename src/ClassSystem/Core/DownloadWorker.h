//
// Created by 35012 on 2023/7/3.
//

#ifndef CLASSSYSTEM_DOWNLOADWORKER_H
#define CLASSSYSTEM_DOWNLOADWORKER_H
#include <QProcess>
#include <QRunnable>
#include <QThread>

class DownloadWorker : public QObject, public QRunnable {
  Q_OBJECT
 public:
  explicit DownloadWorker(QString url, QString name, int id,
                          QObject *parent = nullptr);
  void run() override;
 signals:
  void getProgress(const int &id, const int &progress);
  void finishDownloading(const int &id);

 private:
  const QString mName;
  const QString mUrl;
  const int mId;

  QEventLoop mLoop;
};

#endif  // CLASSSYSTEM_DOWNLOADWORKER_H
