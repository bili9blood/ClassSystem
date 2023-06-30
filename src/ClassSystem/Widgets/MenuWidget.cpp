#include "MenuWidget.h"

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget{parent,
              Qt::WindowStaysOnTopHint | Qt::Tool | Qt::FramelessWindowHint} {
  SET_WIDGET_TRANSPARENT;
  mLayout->setMargin(0);
  mLayout->setSpacing(0);
  mLayout->addSpacerItem(spacer);
}

MenuWidget::~MenuWidget() = default;
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
  textLabel->setSizePolicy(QSizePolicy::Preferred,
                           QSizePolicy::MinimumExpanding);
  iconLabel->setAlignment(Qt::AlignCenter);
  textLabel->setAlignment(Qt::AlignCenter);
  textLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  textLabel->setFont(QFont("Inconsolata", 8));
  btn->installEventFilter(this);
  btn->setStyleSheet("background-color: white");
  layout->setMargin(5);
  layout->setSpacing(0);
  layout->addWidget(iconLabel);
  layout->addWidget(textLabel);
  // set transparent
  iconLabel->SET_WIDGET_TRANSPARENT;
  textLabel->SET_WIDGET_TRANSPARENT;
  btn->SET_WIDGET_TRANSPARENT;
  mLayout->insertWidget(0, btn);
  setFixedSize(80 * mBtns.count() + 40, 80);
  return *this;
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
  if (!flag) return false;
  if (ev->type() == QEvent::Enter || ev->type() == QEvent::MouseButtonRelease) {
    mTextLabels[i]->setFont(QFont("MiSans", 8, 100));
    return true;
  }
  if (ev->type() == QEvent::Leave) {
    mTextLabels[i]->setFont(QFont("MiSans", 8));
    return true;
  }
  if (ev->type() == QEvent::MouseButtonPress ||
      ev->type() == QEvent::MouseButtonDblClick) {
    emit clicked(i);
    return true;
  }
  return false;
}

void MenuWidget::paintEvent(QPaintEvent *ev) {
  for (auto i = size_t(); i < mIconLabels.size(); ++i)
    mIconLabels[i]->setPixmap(mPixs[i].scaledToHeight(
        mIconLabels[i]->height(), Qt::SmoothTransformation));
  QPainter painter(this);
  painter.setBrush(QColor(39, 174, 227));
  painter.setPen(Qt::transparent);
  painter.drawRoundedRect(rect(), 10, 10);
}