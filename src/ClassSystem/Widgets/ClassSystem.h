#pragma once
#include <QMainWindow>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QScreen>

namespace Ui {
class ClassSystem;
}

class ClassSystem : public QMainWindow {
  Q_OBJECT
public:
  explicit ClassSystem(QWidget *parent = nullptr);
  ~ClassSystem();

private:
  Ui::ClassSystem *ui;

protected:
  void paintEvent(QPaintEvent *ev);
  void mouseMoveEvent(QMouseEvent *ev);
};