#pragma once
#include <qapplication.h>
#include <qboxlayout.h>
#include <qdesktopwidget.h>
#include <qlabel.h>
#include <qscreen.h>

class PopupMenu : public QWidget {
  Q_OBJECT
 public:
  explicit PopupMenu(QWidget *parent = nullptr);

 private:
  QDesktopWidget m_desktop;
  QLabel *m_popMenuLabel = new QLabel(this);
  QHBoxLayout *m_mainLayout = new QHBoxLayout(this);
  QPoint m_startPoint;
 private slots:
  void onMenuClicked(size_t menuID);

 protected:
  bool eventFilter(QObject *obj, QEvent *ev) override;
  void paintEvent(QPaintEvent *ev) override;
};