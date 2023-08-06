#pragma once
#include <qapplication.h>
#include <qboxlayout.h>
#include <qevent.h>
#include <qlabel.h>
#include <qscreen.h>

class PopupMenu : public QWidget {
  Q_OBJECT
 public:
  explicit PopupMenu(QWidget *parent = nullptr);

 private:
  QLabel *m_popMenuLabel = new QLabel(this);
  QHBoxLayout *m_mainLayout = new QHBoxLayout(this);
  QHBoxLayout *m_btnLayout = new QHBoxLayout();

  int m_mouseStartYPos;
  bool m_init = true;

 protected:
  bool eventFilter(QObject *obj, QEvent *ev) override;
  void paintEvent(QPaintEvent *ev) override;
  void mousePressEvent(QMouseEvent *ev) override;
  void mouseMoveEvent(QMouseEvent *ev) override;
};