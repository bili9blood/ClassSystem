#pragma once

#include <qundostack.h>

#include "ClassData.h"
#include "MainWindow.h"

class MainWindow;

class ChangeDataCommand : public QUndoCommand {
 public:
  explicit ChangeDataCommand(ClassData::Data before, MainWindow *window,
                             const QString &_text,
                             QUndoCommand *parent = nullptr)
      : m_before(before),
        m_after(window->m_data),
        m_window(window),
        QUndoCommand(parent) {
    setText(_text);
  }

  void undo() override {
    m_window->m_data = m_before;
    m_window->loadData();
    m_window->update();
    m_window->m_changed = true;
  }

  void redo() override {
    m_window->m_data = m_after;
    m_window->loadData();
    m_window->update();
    m_window->m_changed = true;
  }

 private:
  const ClassData::Data m_before;
  const ClassData::Data m_after;

  MainWindow *m_window;
};
