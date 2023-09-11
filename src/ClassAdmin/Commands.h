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
      : QUndoCommand(parent),
        m_before(before),
        m_after(window->m_data),
        m_window(window) {
    setText(_text);
  }

  void undo() override {
    if (m_window->m_doingChange) return;

    m_window->m_data = m_before;
    m_window->loadData();
    m_window->update();
    m_window->m_changed = true;
  }

  void redo() override {
    if (m_window->m_doingChange) return;

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
