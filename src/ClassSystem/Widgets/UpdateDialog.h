//
// Created by 35012 on 2023/7/3.
//

#ifndef CLASSSYSTEM_UPDATEDIALOG_H
#define CLASSSYSTEM_UPDATEDIALOG_H
#include <QDialog>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QThread>
#include <QWinTaskbarButton>
#include <QWinTaskbarProgress>
class UpdateDialog : public QDialog {
  Q_OBJECT
 public:
  explicit UpdateDialog(QWidget *parent = nullptr);
 public slots:
  void appendFile(const QString &fileName, int *id);
  void updateProgress(const int &id, const int &progress);
  void removeFile(const int &id);

 private:
  QGridLayout *mLayout;
  QList<QLabel *> mLabels;
  QList<QProgressBar *> mProgressBars;
  QWinTaskbarProgress *mTaskBarProgress;
  QWinTaskbarButton *mTaskbarBtn = new QWinTaskbarButton(this);

  QLabel *mLabelTot;
  QProgressBar *mProgressBarTot;
  QFrame *mFrameTot;
};

#endif  // CLASSSYSTEM_UPDATEDIALOG_H
