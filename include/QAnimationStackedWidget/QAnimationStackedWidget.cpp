#include "QAnimationStackedWidget.h"

QAnimationStackedWidget::QAnimationStackedWidget(QWidget *parent)
    : QStackedWidget(parent) {
  m_AnimationCurrent = new QPropertyAnimation(this, "pos");
  m_AnimationNext = new QPropertyAnimation(this, "pos");
  m_PauseAnimation = new QPauseAnimation(this);
  m_AnimationGroup = new QSequentialAnimationGroup(this);
  connect(m_AnimationNext, &QPropertyAnimation::finished, this,
          &QAnimationStackedWidget::OnAnimationFinished);
  connect(m_AnimationCurrent, &QPropertyAnimation::finished, this,
          &QAnimationStackedWidget::OnAnimationFinished);
}

QAnimationStackedWidget::~QAnimationStackedWidget() {
  if (m_AnimationCurrent) {
    m_AnimationCurrent->deleteLater();
    m_AnimationCurrent = Q_NULLPTR;
  }
  if (m_AnimationNext) {
    m_AnimationNext->deleteLater();
    m_AnimationNext = Q_NULLPTR;
  }
  if (m_AnimationGroup) {
    m_AnimationGroup->clear();
    m_AnimationGroup->deleteLater();
    m_AnimationGroup = Q_NULLPTR;
  }
  if (m_PauseAnimation) {
    m_PauseAnimation->deleteLater();
    m_PauseAnimation = Q_NULLPTR;
  }
}

void QAnimationStackedWidget::OnAnimationFinished() {
  m_IsAnimation = false;
  widget(m_lndexBeforeAnimation)->hide();
  widget(m_lndexAfterAnimation)->show();
  widget(m_lndexAfterAnimation)->raise();
  setCurrentIndex(m_lndexAfterAnimation);
  emit sStackedWidgetAnimationFinished(m_lndexBeforeAnimation,
                                       m_lndexAfterAnimation);
}

void QAnimationStackedWidget::OnPauseAnimationFinished() {
  widget(m_lndexBeforeAnimation)->hide();
  widget(m_lndexAfterAnimation)->show();
  widget(m_IndexMask)->show();
  widget(m_lndexAfterAnimation)->lower();
  widget(m_IndexMask)->raise();
}

void QAnimationStackedWidget::OnGroupAnimationFinished() {
  widget(m_IndexMask)->hide();
  widget(m_lndexBeforeAnimation)->hide();
  widget(m_lndexAfterAnimation)->show();
  setCurrentIndex(m_lndexAfterAnimation);
  m_IsGroupAnimation = false;
  emit sStackedWidgetAnimationFinished(m_lndexBeforeAnimation,
                                       m_lndexAfterAnimation);
}

void QAnimationStackedWidget::StartStackedWidgetAnimation(int indexbefore,
                                                          int indexafter) {
  rectL = QPointF(-width(), 0);
  rectR = QPointF(width(), 0);
  rectU = QPointF(0, -height());
  rectD = QPointF(0, height());
  if (m_IsAnimation || m_IsGroupAnimation) {
    return;
  }
  if (indexbefore >= count() || indexafter >= count() || indexbefore < 0 ||
      indexafter < 0 || indexafter == indexbefore) {
    return;
  }
  m_lndexBeforeAnimation = indexbefore;
  m_lndexAfterAnimation = indexafter;
  m_AnimationCurrent->setDuration(m_AnimationDuration);
  m_AnimationNext->setDuration(m_AnimationDuration);
  m_IsAnimation = true;
  startAnimationEngine();
}

void QAnimationStackedWidget::StartStackedWidgetMaskAnimation(int indexmask,
                                                              int indexbefore,
                                                              int indexafter,
                                                              int septime) {
  if (m_IsAnimation || m_IsGroupAnimation) {
    return;
  }
  m_IsGroupAnimation = true;
  m_IndexMask = indexmask;
  m_lndexBeforeAnimation = indexbefore;
  m_lndexAfterAnimation = indexafter;
  m_AnimationCurrent->setTargetObject(widget(indexmask));
  m_AnimationNext->setTargetObject(widget(indexmask));
  m_AnimationCurrent->setStartValue(rectL);
  m_AnimationCurrent->setEndValue(rect);
  m_AnimationNext->setStartValue(rect);
  m_AnimationNext->setEndValue(rectL);
  m_AnimationCurrent->setDuration(m_AnimationDuration);
  m_AnimationNext->setDuration(m_AnimationDuration);
  m_PauseAnimation->setDuration(septime);
  m_AnimationGroup->addAnimation(m_AnimationCurrent);
  m_AnimationGroup->addAnimation(m_PauseAnimation);
  m_AnimationGroup->addAnimation(m_AnimationNext);
  m_AnimationGroup->start();
  widget(indexmask)->show();
  widget(indexmask)->raise();
  widget(indexbefore)->show();
}

void QAnimationStackedWidget::startAnimationEngine() {
  m_AnimationCurrent->setTargetObject(widget(m_lndexBeforeAnimation));
  m_AnimationNext->setTargetObject(widget(m_lndexAfterAnimation));
  widget(m_lndexBeforeAnimation)->show();
  widget(m_lndexAfterAnimation)->show();
  switch (m_AnimationMode) {
    case R2L:
      m_AnimationCurrent->setStartValue(rect);
      m_AnimationCurrent->setEndValue(rectL);
      m_AnimationNext->setStartValue(rectR);
      m_AnimationNext->setEndValue(rect);
      m_AnimationCurrent->start();
      m_AnimationNext->start();
      break;
    case L2R:
      m_AnimationCurrent->setStartValue(rect);
      m_AnimationCurrent->setEndValue(rectR);
      m_AnimationNext->setStartValue(rectL);
      m_AnimationNext->setEndValue(rect);
      m_AnimationCurrent->start();
      m_AnimationNext->start();
      break;
    case U2D:
      m_AnimationCurrent->setStartValue(rect);
      m_AnimationCurrent->setEndValue(rectD);
      m_AnimationNext->setStartValue(rectU);
      m_AnimationNext->setEndValue(rect);
      m_AnimationCurrent->start();
      m_AnimationNext->start();
      break;
    case D2U:
      m_AnimationCurrent->setStartValue(rect);
      m_AnimationCurrent->setEndValue(rectU);
      m_AnimationNext->setStartValue(rectD);
      m_AnimationNext->setEndValue(rect);
      m_AnimationCurrent->start();
      m_AnimationNext->start();
      break;
    case FromD:
      m_AnimationNext->setStartValue(rectD);
      m_AnimationNext->setEndValue(rect);
      m_AnimationNext->start();
      widget(m_lndexAfterAnimation)->raise();
      break;
    case FromU:
      m_AnimationNext->setStartValue(rectU);
      m_AnimationNext->setEndValue(rect);
      m_AnimationNext->start();
      widget(m_lndexAfterAnimation)->raise();
      break;
    case FromL:
      m_AnimationNext->setStartValue(rectL);
      m_AnimationNext->setEndValue(rect);
      m_AnimationNext->start();
      widget(m_lndexAfterAnimation)->raise();
      break;
    case FromR:
      m_AnimationNext->setStartValue(rectR);
      m_AnimationNext->setEndValue(rect);
      m_AnimationNext->start();
      widget(m_lndexAfterAnimation)->raise();
      break;
    case Quit2D:
      m_AnimationCurrent->setStartValue(rect);
      m_AnimationCurrent->setEndValue(rectD);
      m_AnimationCurrent->start();
      m_AnimationNext->setStartValue(rect);
      m_AnimationNext->setEndValue(rect);
      m_AnimationNext->start();
      widget(m_lndexBeforeAnimation)->raise();
      break;
    case Quit2U:
      m_AnimationCurrent->setStartValue(rect);
      m_AnimationCurrent->setEndValue(rectU);
      m_AnimationCurrent->start();
      m_AnimationNext->setStartValue(rect);
      m_AnimationNext->setEndValue(rect);
      m_AnimationNext->start();
      widget(m_lndexBeforeAnimation)->raise();
      break;
    case Quit2L:
      m_AnimationCurrent->setStartValue(rect);
      m_AnimationCurrent->setEndValue(rectL);
      m_AnimationCurrent->start();
      m_AnimationNext->setStartValue(rect);
      m_AnimationNext->setEndValue(rect);
      m_AnimationNext->start();
      widget(m_lndexBeforeAnimation)->raise();
      break;
    case Quit2R:
      m_AnimationCurrent->setStartValue(rect);
      m_AnimationCurrent->setEndValue(rectR);
      m_AnimationCurrent->start();
      m_AnimationNext->setStartValue(rect);
      m_AnimationNext->setEndValue(rect);
      m_AnimationNext->start();
      widget(m_lndexBeforeAnimation)->raise();
      break;
    default:
      break;
  }
}

void QAnimationStackedWidget::setAnimationMode(int mode) {
  m_AnimationMode = mode;
  rectL = QPointF(-width(), 0);
  rectR = QPointF(width(), 0);
  rectU = QPointF(0, -height());
  rectD = QPointF(0, height());
  rect = QPointF(0, 0);
  switch (mode) {
    case AUTOScroll:
      disconnect(m_AnimationNext, &QPropertyAnimation::finished, this,
                 &QAnimationStackedWidget::OnAnimationFinished);
      disconnect(m_AnimationCurrent, &QPropertyAnimation::finished, this,
                 &QAnimationStackedWidget::OnAnimationFinished);
      connect(m_PauseAnimation, &QPauseAnimation::finished, this,
              &QAnimationStackedWidget::OnPauseAnimationFinished);
      connect(m_AnimationGroup, &QSequentialAnimationGroup::finished, this,
              &QAnimationStackedWidget::OnGroupAnimationFinished);
      break;
    default:
      break;
  }
}

void QAnimationStackedWidget::setAnimationDuration(int duration) {
  m_AnimationCurrent->setDuration(duration);
}
