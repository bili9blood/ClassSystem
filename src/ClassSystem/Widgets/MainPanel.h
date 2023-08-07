#pragma once

#include <qboxlayout.h>
#include <qevent.h>
#include <qlabel.h>
#include <qtimer.h>
class MainPanel : public QWidget {
  Q_OBJECT

 public:
  explicit MainPanel(QWidget *parent = nullptr);

 private:
  QGridLayout *m_layoutMain = new QGridLayout(this);
  QGridLayout *m_layoutDateTime = new QGridLayout;
  QVBoxLayout *m_layoutStudentsCarryMeals = new QVBoxLayout;
  QLabel *m_labelDate = new QLabel("00-00", this);
  QLabel *m_labelTime = new QLabel("00:00:00", this);
  QLabel *m_labelDDDD = new QLabel("星期八", this);
  QTimer m_timerHalfSeconds;
  constexpr static const char *m_kTimeFormat[2] = {"hh:mm:ss", "hh mm ss"};
  uint m_lastSecond;

  QPoint m_mouseStartPoint;
 private slots:
  void onHalfSecs();

 protected:
  void paintEvent(QPaintEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
};
