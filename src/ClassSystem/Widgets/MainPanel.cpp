#include "MainPanel.h"

#include <qbrush.h>
#include <qdatetime.h>
#include <qpainter.h>
#include <qrandom.h>
#include <qsizepolicy.h>

MainPanel::MainPanel(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint) {
  setWidgetTransparent(this);
  setParentToDesktop(this);

  setStyleSheet(R"(

#labelDDDD, #labelDate, #mealStuListWid, #stuOnDutyListWid {
  color: #d2d0ce;
}
#sentenceLine, #stuLine, #topNoticeLine, #bottomNoticeLine {
  color: rgba(102, 113, 134 ,180);
}
#mealStuTitle, #stuOnDutyTitle, #noticesTitle {
  color: #edebe9;
}
#mealStuListWid, #stuOnDutyListWid, #noticesWid {
  background-color: transparent;
}
#labelTime {
  color:#d9d9d9;
}
)");

  // init header
  m_labelDDDD->setObjectName("labelDDDD");
  m_labelDate->setObjectName("labelDate");
  m_labelTime->setObjectName("labelTime");
  m_labelDDDD->setText(QDate::currentDate().toString("dddd"));
  m_labelDate->setText(QDate::currentDate().toString("MM-dd"));
  m_labelDate->setFont(qFont{.pointSize = 16}());
  m_labelDDDD->setFont(qFont{.pointSize = 16}());
  m_labelTime->setFont(qFont{"华文中宋", 36, QFont::Bold}());
  m_labelDate->setFont(qFont{.pointSize = 16}());
  m_labelDDDD->setFont(qFont{.pointSize = 16}());

  m_sentenceLabel->setStyleSheet("color: #e5c07b;");
  QFile sentenceFile(":/other/sentences.txt");
  if (sentenceFile.open(QFile::ReadOnly | QFile::Text)) {
    QStringList lines =
        QString::fromUtf8(sentenceFile.readAll()).remove('\r').split('\n');
    m_sentenceLabel->setText(
        lines.at(QRandomGenerator::global()->bounded(0, lines.size())));
  }
  m_sentenceLabel->setWordWrap(true);
  m_sentenceLabel->setFont(qFont{.family = "仿宋", .pointSize = 17}());

  // init notices
  m_noticesWid->setAttribute(Qt::WA_TransparentForMouseEvents);
  m_noticesWid->setFocusPolicy(Qt::NoFocus);
  m_noticesWid->setSizePolicy(QSizePolicy::MinimumExpanding,
                              QSizePolicy::Preferred);
  m_noticesWid->setAnimationMode(QAnimationStackedWidget::R2L);
  m_noticesWid->setAnimationDuration(500);
  for (const QVariant &v : m_data.notices) {
    const auto &[date, str] = v.value<Notice>();
    QTextBrowser *l = new QTextBrowser(this);
    l->setText(str);
    l->setStyleSheet("color: #d2d0ce;background-color: transparent");
    l->setFont(qFont{.pointSize = 20}());
    l->setAttribute(Qt::WA_TransparentForMouseEvents);
    l->setFocusPolicy(Qt::NoFocus);
    l->setFrameShape(QFrame::NoFrame);
    l->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    l->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_noticesLabels << l;
    m_noticesWid->addWidget(l);
  }
  m_noticesTitle->setObjectName("noticesTitle");
  m_noticesTitle->setFont(qFont{.pointSize = 28, .weight = QFont::Bold}());
  m_noticesTitle->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

  // init students carry meals
  m_mealStuLabel->setObjectName("mealStuListWid");
  m_mealStuLabel->setFont(
      qFont{.family = "'Consolas', 'MiSans'", .pointSize = 22}());
  auto mealStuToday = m_data.mealStu[dayToday()];
  for (const uint &id : mealStuToday) {
    m_mealStuLabel->setText(m_mealStuLabel->text() + "\n" +
                            m_data.idAndName(id));
  }
  m_mealStuLabel->setText(m_mealStuLabel->text().mid(1));

  m_mealStuTitle->setObjectName("mealStuTitle");
  m_mealStuTitle->setFont(qFont{.pointSize = 28, .weight = QFont::Bold}());

  // init students on duty
  m_stuOnDutyLabel->setSizePolicy(QSizePolicy::MinimumExpanding,
                                  QSizePolicy::MinimumExpanding);
  m_stuOnDutyLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
  m_stuOnDutyLabel->setObjectName("stuOnDutyListWid");
  m_stuOnDutyLabel->setFont(
      qFont{.family = "'Consolas', 'MiSans'", .pointSize = 22}());
  auto stuOnDutyToday = m_data.stuOnDuty[dayToday()];
  for (int i = 0; i < stuOnDutyToday.size(); ++i) {
    QList<uint> l = stuOnDutyToday[i];
    if (l.empty()) continue;
    QString displayStr =
        QString(
            R"(<font style="font-weight: 1000; font-size: 25pt; display: inline;">%1:</font>)")
            .arg(m_data.dutyJobs[i]);
    for (const uint &id : l) displayStr += " " + m_data.idAndName(id);
    m_stuOnDutyLabel->setText(m_stuOnDutyLabel->text() + "<br></br>" +
                              displayStr);
  }
  m_stuOnDutyLabel->setText(
      m_stuOnDutyLabel->text().mid(9));  // 移除第一个 `<br></br>`

  m_stuOnDutyTitle->setObjectName("stuOnDutyTitle");
  m_stuOnDutyTitle->setFont(qFont{.pointSize = 28, .weight = QFont::Bold}());

  // init lines
  m_sentenceLine->setFrameShape(QFrame::VLine);
  m_sentenceLine->setObjectName("sentenceLine");

  m_stuLine->setFrameShape(QFrame::VLine);
  m_stuLine->setObjectName("stuLine");

  m_topNoticeLine->setFrameShape(QFrame::HLine);
  m_topNoticeLine->setObjectName("topNoticeLine");

  m_bottomNoticeLine->setFrameShape(QFrame::HLine);
  m_bottomNoticeLine->setObjectName("bottomNoticeLine");

  // init layouts
  m_mainLayout->setMargin(10);
  m_mainLayout->setSpacing(10);
  m_mainLayout->addLayout(m_headerLayout, 0, 0, 1, 3, Qt::AlignLeft);
  m_mainLayout->addWidget(m_topNoticeLine, 1, 0, 1, 3);
  m_mainLayout->addWidget(m_noticesTitle, 2, 0, Qt::AlignVCenter);
  m_mainLayout->addWidget(m_noticesWid, 3, 0, 1, 3, Qt::AlignVCenter);
  m_mainLayout->addWidget(m_bottomNoticeLine, 4, 0, 1, 3);
  m_mainLayout->addLayout(m_mealStuLayout, 5, 0);
  m_mainLayout->addWidget(m_stuLine, 5, 1);
  m_mainLayout->addLayout(m_stuOnDutyLayout, 5, 2);

  m_headerLayout->addWidget(m_labelTime, 0, 0, 2, 1, Qt::AlignBottom);
  m_headerLayout->addWidget(m_labelDate, 0, 1, Qt::AlignBottom);
  m_headerLayout->addWidget(m_labelDDDD, 1, 1, Qt::AlignBottom);
  m_headerLayout->addWidget(m_sentenceLine, 0, 2, 2, 1);
  m_headerLayout->addWidget(m_sentenceLabel, 0, 3, 2, 1, Qt::AlignBottom);

  m_mealStuLayout->setAlignment(Qt::AlignTop);
  m_mealStuLayout->addWidget(m_mealStuTitle);
  m_mealStuLayout->addWidget(m_mealStuLabel, 0, Qt::AlignTop);

  m_stuOnDutyLayout->setAlignment(Qt::AlignTop);
  m_stuOnDutyLayout->addWidget(m_stuOnDutyTitle);
  m_stuOnDutyLayout->addWidget(m_stuOnDutyLabel, 0, Qt::AlignTop);

  //  init timers
  m_clockTimerId = startTimer(500);
  m_noticeTimerId = startTimer(5000);

  loadFromIni();
}

bool MainPanel::nativeEvent(const QByteArray &, void *message, long *result) {
  if (!m_resizable) return false;
  MSG *msg = (MSG *)message;
  if (msg->message == WM_NCHITTEST) {
    int xPos = GET_X_LPARAM(msg->lParam) - frameGeometry().x();
    int yPos = GET_Y_LPARAM(msg->lParam) - frameGeometry().y();
    if (xPos < kPadding && yPos < kPadding)  // 左上角
      *result = HTTOPLEFT;
    else if (xPos >= width() - kPadding && yPos < kPadding)  // 右上角
      *result = HTTOPRIGHT;
    else if (xPos < kPadding && yPos >= height() - kPadding)  // 左下角
      *result = HTBOTTOMLEFT;
    else if (xPos >= width() - kPadding &&
             yPos >= height() - kPadding)  // 右下角
      *result = HTBOTTOMRIGHT;
    else if (xPos < kPadding)  // 左边
      *result = HTLEFT;
    else if (xPos >= width() - kPadding)  // 右边
      *result = HTRIGHT;
    else if (yPos < kPadding)  // 上边
      *result = HTTOP;
    else if (yPos >= height() - kPadding)  // 下边
      *result = HTBOTTOM;
    else
      return false;
    return true;
  }
  return false;
}

void MainPanel::paintEvent(QPaintEvent *) {
  if (m_init) {
    // init geometry
    setGeometry(m_settings.value("geometry").toRect());
    m_init = false;
  }
  if (geometry() != m_settings.value("geometry").toRect()) {
    m_settings.setValue("geometry", geometry());
  }
  QPainter painter(this);
  QLinearGradient ling(rect().topLeft(), rect().bottomRight());

  ling.setColorAt(0, {0, 12, 64, 205});
  ling.setColorAt(1, {96, 125, 139, 205});
  painter.setBrush(ling);
  painter.setPen(Qt::transparent);
  painter.drawRoundedRect(rect(), 20, 20);
}

void MainPanel::loadFromIni() {
  m_resizable = m_settings.value("resizable").toBool();
}

void MainPanel::mousePressEvent(QMouseEvent *ev) {
  m_mouseStartPoint = pos() - ev->globalPos();
}

void MainPanel::mouseMoveEvent(QMouseEvent *ev) {
  move(ev->globalPos() + m_mouseStartPoint);
}

void MainPanel::resizeEvent(QResizeEvent *) {
  if (geometry() != m_settings.value("geometry").toRect()) {
    m_settings.setValue("geometry", geometry());
  }
}

void MainPanel::moveEvent(QMoveEvent *) {
  if (geometry() != m_settings.value("geometry").toRect()) {
    m_settings.setValue("geometry", geometry());
  }
}

void MainPanel::timerEvent(QTimerEvent *ev) {
  if (ev->timerId() == m_clockTimerId) {
    m_labelTime->setText(QTime::currentTime().toString(
        kTimeFormat[m_formatWithColons = !m_formatWithColons]));
  }
  if (ev->timerId() == m_noticeTimerId) {
    m_noticesWid->StartStackedWidgetAnimation(
        m_noticesWid->currentIndex(),
        (m_noticesWid->currentIndex() + 1) % m_noticesWid->count());
  }
}