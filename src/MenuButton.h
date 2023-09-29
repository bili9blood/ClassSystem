#pragma once

#include <qboxlayout.h>
#include <qlabel.h>
#include <qwidget.h>

class MenuButton : public QWidget {
  Q_OBJECT
 public:
  explicit MenuButton(QPixmap icon = QPixmap(), QString text = "",
                      QWidget *parent = nullptr);

 signals:
  void clicked();

 private:
  QVBoxLayout *m_mainLayout = new QVBoxLayout(this);
  QLabel *m_iconLabel = new QLabel(this);
  QLabel *m_textLabel = new QLabel(this);

  enum class State { Normal, Hovered, Clicked } m_state;

 protected:
  bool event(QEvent *ev) override;
};