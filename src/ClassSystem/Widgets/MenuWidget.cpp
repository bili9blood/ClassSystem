#include "MenuWidget.h"

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget{parent,
              Qt::WindowStaysOnTopHint | Qt::Tool | Qt::FramelessWindowHint} {
  setStyleSheet("background-color: white");
  mLayout->setMargin(0);
  mLayout->setSpacing(0);
}

MenuWidget::~MenuWidget() {}
MenuWidget &MenuWidget::addToMenu(const QString &text, const QPixmap &icon) {
  mBtns << new QWidget(this);
  mPixs << icon;
  auto btn = mBtns.back();
  mLayouts << new QVBoxLayout(btn);
  mTextLabels << new QLabel(text, btn);
  mIconLabels << new QLabel(btn);
  auto layout = mLayouts.back();
  auto textLabel = mTextLabels.back();
  auto iconLabel = mIconLabels.back();
  iconLabel->setPixmap(
      icon.scaledToHeight(iconLabel->height(), Qt::SmoothTransformation));
  textLabel->setSizePolicy(
      QSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding));
  iconLabel->setAlignment(Qt::AlignCenter);
  // iconLabel->setScaledContents(true);
  textLabel->setAlignment(Qt::AlignCenter);
  textLabel->setSizePolicy(
      QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
  btn->installEventFilter(this);
  btn->setStyleSheet("background-color: white");
  layout->setMargin(5);
  layout->setSpacing(0);
  layout->addWidget(iconLabel);
  layout->addWidget(textLabel);
  mLayout->addWidget(btn);
  return *this;
  resize(80 * mBtns.count(), 80);
}

bool MenuWidget::eventFilter(QObject *obj, QEvent *ev) {
  bool flag = false;
  auto i = size_t();
  for (; i < mBtns.size(); ++i) {
    if (mBtns[i] == obj) {
      flag = true;
      break;
    }
  }
  if (!flag)
    return false;
  auto btn = qobject_cast<QWidget *>(obj);
  if (ev->type() == QEvent::Enter || ev->type() == QEvent::MouseButtonRelease) {
    btn->setStyleSheet("background-color: grey");
    return true;
  }
  if (ev->type() == QEvent::Leave) {
    btn->setStyleSheet("background-color: white");
    return true;
  }
  if (ev->type() == QEvent::MouseButtonPress ||
      ev->type() == QEvent::MouseButtonDblClick) {
    btn->setStyleSheet("background-color: deepskyblue");
    emit clicked(i);
    return true;
  }
  return false;
}

void MenuWidget::paintEvent(QPaintEvent *ev) {
  for (auto i = size_t(); i < mIconLabels.size(); ++i) {
    mIconLabels[i]->setPixmap(mPixs[i].scaledToHeight(
        mIconLabels[i]->height(), Qt::SmoothTransformation));
  }
}