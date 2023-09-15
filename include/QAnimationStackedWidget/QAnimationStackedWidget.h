#pragma once

#include <QDebug>
#include <QPauseAnimation>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QStackedWidget>

class QAnimationStackedWidget : public QStackedWidget {
  Q_OBJECT

 public:
  enum AnimationMode {
    L2R,
    R2L,
    U2D,
    D2U,
    FromL,
    FromR,
    FromU,
    FromD,
    Quit2L,
    Quit2R,
    Quit2U,
    Quit2D,
    AUTOScroll
  };
  QAnimationStackedWidget(QWidget* parent);
  ~QAnimationStackedWidget();
  // 设置动画时长
  void setAnimationDuration(int duration);
  // 设置动画模式，模式在enum AnimationMode中枚举
  void setAnimationMode(int mode);
 signals:
  // 动画完成时发出信号
  void sStackedWidgetAnimationFinished(int indexbefore, int indexcurrent);
 public slots:
  // 支持除AUTOScroll外的动画
  void StartStackedWidgetAnimation(int indexbefore, int indexafter);
  // 仅支持AUTOScroll动画，indexmask为遮罩层的index,septime为暂停时间
  void StartStackedWidgetMaskAnimation(int indexmask, int indexbefore,
                                       int indexafter, int septime);

 private:
  QPropertyAnimation* m_AnimationCurrent;
  QPropertyAnimation* m_AnimationNext;
  QPauseAnimation* m_PauseAnimation;
  QSequentialAnimationGroup* m_AnimationGroup;
  int m_AnimationDuration = 1000;
  double m_AnimationCurrentValue = 0;
  int m_AnimationMode = 0;
  int m_lndexBeforeAnimation = 0;
  int m_lndexAfterAnimation = 0;
  int m_IndexMask = 0;
  bool m_IsAnimation = false;
  bool m_IsGroupAnimation = false;
  QPointF rectL;
  QPointF rectR;
  QPointF rectU;
  QPointF rectD;
  QPointF rect;
  void startAnimationEngine();
 private slots:
  void OnAnimationFinished();
  void OnPauseAnimationFinished();
  void OnGroupAnimationFinished();
};
