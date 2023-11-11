#pragma once
#include <qboxlayout.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qwidget.h>

class TitleWidget : public QWidget {
  Q_OBJECT
 public:
  explicit TitleWidget(QString title, QWidget *parent = nullptr);

 private:
  QHBoxLayout *m_mainLayout = new QHBoxLayout(this);

  QLabel *m_logoLabel = new QLabel(this);
  QLabel *m_textLabel = new QLabel(this);
};