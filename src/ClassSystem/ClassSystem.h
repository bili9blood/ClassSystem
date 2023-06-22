#pragma once
#include <QMainWindow>

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
};