#pragma once

#include <qundostack.h>

#include "ClassData.h"
#include "MainWindow.h"

class MainWindow;

class ChangeDataCommand : public QUndoCommand {
 public:
  explicit ChangeDataCommand(ClassData::Data before, MainWindow *window,
                             QUndoCommand *parent = nullptr)
      : m_before(before),
        m_data(&window->m_data),
        m_after(window->m_data),
        m_window(window),
        QUndoCommand(parent) {}

  void undo() override {
    m_window->m_data = m_before;
    m_window->loadData();
    m_window->m_changed = true;
  }

  void redo() override {
    m_window->m_data = m_after;
    m_window->loadData();
    m_window->m_changed = true;
  }

 private:
  ClassData::Data *m_data;
  const ClassData::Data m_before;
  const ClassData::Data m_after;

  MainWindow *m_window;
};

class ChangeConfigCommand : public QUndoCommand {};