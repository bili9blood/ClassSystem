//
// Created by 35012 on 2023/7/3.
//

#include "UpdateDialog.h"
UpdateDialog::UpdateDialog(QWidget *parent) : QDialog(parent) {
  setWindowFlag(Qt::WindowCloseButtonHint, false);
  setWindowFlag(Qt::WindowMinimizeButtonHint);
  setWindowFlag(Qt::WindowStaysOnTopHint);
  setWindowTitle("班级系统 ClassSystem 自动更新");
  mLayout = new QGridLayout(this);
  qDebug() << "ui:"
           << "thread:" << QThread::currentThread();
  mLabelTot = new QLabel("总进度：", this);
  mProgressBarTot = new QProgressBar(this);
  mProgressBarTot->setRange(0, 100);
  mProgressBarTot->setMinimumWidth(300);
  mFrameTot = new QFrame(this);
  mFrameTot->setFrameShape(QFrame::HLine);
  mLayout->addWidget(mLabelTot, 0, 0);
  mLayout->addWidget(mProgressBarTot, 0, 1);
  mLayout->addWidget(mFrameTot, 1, 0, 1, 2);
  show();
  mTaskbarBtn->setWindow(windowHandle());
  mTaskBarProgress = mTaskbarBtn->progress();

  mTaskBarProgress->setRange(0, 100);
  mTaskBarProgress->setVisible(true);
}
void UpdateDialog::appendFile(const QString &fileName, int *id) {
  *id = mLabels.size();  // same as mProgressBars.size()
  auto label = new QLabel(fileName, this);
  auto progressBar = new QProgressBar(this);
  progressBar->setRange(0, 100);
  progressBar->setValue(0);
  mLayout->addWidget(label, *id + 2, 0);
  progressBar->setMinimumWidth(300);
  mLayout->addWidget(progressBar, *id + 2, 1);
  mLabels << label;
  mProgressBars << progressBar;
}
void UpdateDialog::updateProgress(const int &id, const int &progress) {
  mProgressBars[id]->setValue(progress);
  int sum = 0;
  for (const auto p : mProgressBars) sum += p->value();
  sum /= mLabels.size();
  mTaskBarProgress->setValue(sum);
  mProgressBarTot->setValue(sum);
}
void UpdateDialog::removeFile(const int &id) {
  mProgressBars[id]->setValue(100);
  mLabels[id]->setDisabled(true);
  mProgressBars[id]->setDisabled(true);
}
