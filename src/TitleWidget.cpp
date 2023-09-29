#include "TitleWidget.h"

#include <qpainter.h>

TitleWidget::TitleWidget(QString title, QWidget *parent) : QWidget(parent) {
  setAttribute(Qt::WA_TransparentForMouseEvents);
  setStyleSheet("background-color: transparent");

  // init labels
  m_logoLabel->setPixmap(
      QPixmap(":/img/logo.png")
          .scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
  m_textLabel->setText(title);
  m_textLabel->setFont(cs::font{.family = "zihun181hao-feichibiaotiti",
                                .pointSize = cs::settings::mediumFontSize}());
  m_textLabel->setProperty("class", "titleText");

  // init layouts
  m_mainLayout->setMargin(5);
  m_mainLayout->addWidget(m_logoLabel);
  m_mainLayout->addWidget(m_textLabel);
}
