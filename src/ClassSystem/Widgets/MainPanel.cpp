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

#labelDDDD, #labelDate, #mealStuListWid, #stuOnDutyListWid, #sentenceLine, #stuLine {
  color: #d2d0ce;
}
#mealStuTitle, #stuOnDutyTitle{
  color: #edebe9;
}
#mealStuListWid, #stuOnDutyListWid {
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

  // init students carry meals
  m_mealStuLabel->setFrameShape(QFrame::NoFrame);
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
  m_stuOnDutyLabel->setFrameShape(QFrame::NoFrame);
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

  // init layouts
  m_mainLayout->setMargin(10);
  m_mainLayout->setSpacing(10);
  m_mainLayout->addLayout(m_headerLayout, 0, 0, 1, 3, Qt::AlignLeft);
  m_mainLayout->addLayout(m_mealStuLayout, 1, 0);
  m_mainLayout->addWidget(m_stuLine, 1, 1);
  m_mainLayout->addLayout(m_stuOnDutyLayout, 1, 2);

  m_headerLayout->addWidget(m_labelTime, 0, 0, 2, 1, Qt::AlignBottom);
  m_headerLayout->addWidget(m_labelDate, 0, 1, Qt::AlignBottom);
  m_headerLayout->addWidget(m_labelDDDD, 1, 1, Qt::AlignBottom);
  m_headerLayout->addWidget(m_sentenceLine, 0, 2, 2, 1);
  m_headerLayout->addWidget(m_sentenceLabel, 0, 3, 2, 1, Qt::AlignBottom);

  m_mealStuLayout->setAlignment(Qt::AlignTop);
  m_mealStuLayout->addWidget(m_mealStuTitle);
  m_mealStuLayout->addWidget(m_mealStuLabel);

  m_stuOnDutyLayout->setAlignment(Qt::AlignTop);
  m_stuOnDutyLayout->addWidget(m_stuOnDutyTitle);
  m_stuOnDutyLayout->addWidget(m_stuOnDutyLabel);

  //  init timer
  m_timer.setInterval(500);  // 0.5 secs
  connect(&m_timer, &QTimer::timeout, this, &MainPanel::onHalfSecs);
  m_timer.start();
}

void MainPanel::paintEvent(QPaintEvent *) {
  if (m_init) {
    // init geometry
    qDebug() << m_settings.value("geometry").toRect();
    setGeometry(m_settings.value("geometry").toRect());
    m_init = false;
  }
  QPainter painter(this);
  QLinearGradient ling(rect().topLeft(), rect().bottomRight());

  ling.setColorAt(0, {0, 12, 64, 225});
  ling.setColorAt(1, {96, 125, 139, 225});
  painter.setBrush(ling);
  painter.setPen(Qt::transparent);
  painter.drawRoundedRect(rect(), 20, 20);
}
void MainPanel::onHalfSecs() {
  m_labelTime->setText(QTime::currentTime().toString(
      kTimeFormat[m_formatWithColons = !m_formatWithColons]));
}

void MainPanel::mousePressEvent(QMouseEvent *ev) {
  m_mouseStartPoint = pos() - ev->globalPos();
}

void MainPanel::mouseMoveEvent(QMouseEvent *ev) {
  move(ev->globalPos() + m_mouseStartPoint);
}

bool MainPanel::nativeEvent(const QByteArray &, void *message, long *result) {
  MSG *msg = (MSG *)message;
  if (msg->message == WM_NCHITTEST && m_settings.value("resizable").toBool()) {
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
    else  // 其他部分不做处理，返回false，留给其他事件处理器处理
      return false;
    repaint();
    return true;
  }
  return false;
}

void MainPanel::resizeEvent(QResizeEvent *) {
  if (!m_init) m_settings.setValue("geometry", geometry());
}

void MainPanel::moveEvent(QMoveEvent *) {
  if (!m_init) m_settings.setValue("geometry", geometry());
}