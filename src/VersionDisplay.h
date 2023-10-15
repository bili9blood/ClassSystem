#pragma once

#include <qboxlayout.h>
#include <qlabel.h>
#include <qwidget.h>

class VersionDisplay : public QWidget {
  Q_OBJECT
 public:
  explicit VersionDisplay(QWidget* parent = nullptr);

 public slots:
  void onProgress(ushort progress);

 private:
  QVBoxLayout* m_layout = new QVBoxLayout(this);
  QLabel* m_label = new QLabel(this);

  ushort m_progress = 101;  // 101代表不在下载更新
  const QString m_versionText = "V"_s + cs::config::projectVersion;

 protected:
  void paintEvent(QPaintEvent*) override;
};
