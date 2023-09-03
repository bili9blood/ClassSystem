#include "MainPanel.h"

#include <qbrush.h>
#include <qbuffer.h>
#include <qdatetime.h>
#include <qmessagebox.h>
#include <qpainter.h>
#include <qrandom.h>
#include <qsizepolicy.h>

MainPanel::MainPanel(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint) {
  setWidgetTransparent(this);
  setParentToDesktop(this);

  setStyleSheet(R"(

#labelDDDD, #labelDate, #mealStuLabel, #stuOnDutyLabel {
  color: #d2d0ce;
}

QFrame {
  color: rgba(102, 113, 134 ,180);
}

.titleText, #mealStuTitle, #stuOnDutyTitle, #noticesTitle, #eventNameLabel, QLCDNumber {
  color: #edebe9;
}

#mealStuLabel, #stuOnDutyLabel, #noticesWid, #lessons {
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
  m_sentenceLabel->setSizePolicy(QSizePolicy::MinimumExpanding,
                                 QSizePolicy::Preferred);
  QFile sentenceFile(":/text/sentences.txt");
  if (sentenceFile.open(QFile::ReadOnly | QFile::Text)) {
    QStringList lines =
        QString::fromUtf8(sentenceFile.readAll()).remove('\r').split('\n');
    m_sentenceLabel->setText(
        lines.at(QRandomGenerator::global()->bounded(0, lines.size())));
  }
  m_sentenceLabel->setWordWrap(true);
  m_sentenceLabel->setFont(qFont{.family = "仿宋", .pointSize = 19}());

  // init lessons
  m_lessons->setObjectName("lessons");
  m_lessons->setAttribute(Qt::WA_TransparentForMouseEvents);
  m_lessons->setFocusPolicy(Qt::NoFocus);
  m_lessons->horizontalHeader()->setVisible(false);
  m_lessons->verticalHeader()->setVisible(false);
  m_lessons->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  m_lessons->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  m_lessons->setFrameShape(QFrame::NoFrame);
  m_lessons->setShowGrid(false);
  m_lessons->setSizePolicy(QSizePolicy::MinimumExpanding,
                           QSizePolicy::MinimumExpanding);
  m_lessons->setFixedWidth(370);
  m_lessons->setFont(qFont{.family = "华文中宋", .pointSize = 20}());
  m_lessons->setTextElideMode(Qt::ElideNone);

  // init notices
  m_noticesWid->setAttribute(Qt::WA_TransparentForMouseEvents);
  m_noticesWid->setFocusPolicy(Qt::NoFocus);
  m_noticesWid->setSizePolicy(QSizePolicy::MinimumExpanding,
                              QSizePolicy::Preferred);
  m_noticesWid->setAnimationMode(QAnimationStackedWidget::R2L);
  m_noticesWid->setAnimationDuration(500);

  m_noticesTitle->setObjectName("noticesTitle");
  m_noticesTitle->setFont(qFont{.pointSize = 28, .weight = QFont::Bold}());
  m_noticesTitle->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

  // init days left
  m_eventNameLabel->setObjectName("eventNameLabel");
  m_eventNameLabel->setFont(qFont{.pointSize = 20}());
  m_daysLeftDisplay->setFrameShape(QFrame::NoFrame);
  m_daysLeftDisplay->setSegmentStyle(QLCDNumber::Flat);

  // init students carry meals
  m_mealStuLabel->setObjectName("mealStuLabel");
  m_mealStuLabel->setFont(
      qFont{.family = "'Consolas', 'MiSans'", .pointSize = 22}());

  m_mealStuTitle->setObjectName("mealStuTitle");
  m_mealStuTitle->setFont(qFont{.pointSize = 28, .weight = QFont::Bold}());

  // init students on duty
  m_stuOnDutyLabel->setSizePolicy(QSizePolicy::MinimumExpanding,
                                  QSizePolicy::MinimumExpanding);
  m_stuOnDutyLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
  m_stuOnDutyLabel->setObjectName("stuOnDutyLabel");
  m_stuOnDutyLabel->setFont(
      qFont{.family = "'Consolas', 'MiSans'", .pointSize = 22}());

  m_stuOnDutyTitle->setObjectName("stuOnDutyTitle");
  m_stuOnDutyTitle->setFont(qFont{.pointSize = 28, .weight = QFont::Bold}());

  // init lines
  m_sentenceLine->setFrameShape(QFrame::VLine);
  m_titleLine->setFrameShape(QFrame::VLine);
  m_topNoticeLine->setFrameShape(QFrame::HLine);
  m_bottomNoticeLine->setFrameShape(QFrame::HLine);
  m_daysLeftLine->setFrameShape(QFrame::VLine);
  m_lessonsLine->setFrameShape(QFrame::VLine);
  m_stuLine->setFrameShape(QFrame::VLine);

  // init layouts
  m_mainLayout->setMargin(10);
  m_mainLayout->setSpacing(10);
  m_mainLayout->addLayout(m_headerLayout, 0, 0, 1, 6, Qt::AlignLeft);
  m_mainLayout->addWidget(m_topNoticeLine, 1, 0, 1, 6);

  m_mainLayout->addWidget(m_lessonsLine, 2, 4, 6, 1);
  m_mainLayout->addWidget(m_lessons, 2, 5, 6, 1);
  m_mainLayout->addLayout(m_noticesLayout, 3, 0, 1, 4, Qt::AlignTop);
  m_mainLayout->addWidget(m_bottomNoticeLine, 4, 0, 1, 4);
  m_mainLayout->addLayout(m_mealStuLayout, 5, 0);
  m_mainLayout->addWidget(m_stuLine, 5, 1);
  m_mainLayout->addLayout(m_stuOnDutyLayout, 5, 2);

  m_headerLayout->addWidget(m_labelTime, 0, 0, 2, 1, Qt::AlignVCenter);
  m_headerLayout->addWidget(m_labelDate, 0, 1, Qt::AlignBottom);
  m_headerLayout->addWidget(m_labelDDDD, 1, 1, Qt::AlignBottom);
  m_headerLayout->addWidget(m_sentenceLine, 0, 2, 2, 1);
  m_headerLayout->addWidget(m_sentenceLabel, 0, 3, 2, 1, Qt::AlignTop);
  m_headerLayout->addWidget(m_titleLine, 0, 4, 2, 1);
  m_headerLayout->addWidget(m_title, 0, 5, 2, 1, Qt::AlignRight);

  m_noticesLayout->setMargin(0);
  m_noticesLayout->setSpacing(2);
  m_noticesLayout->addWidget(m_noticesTitle, 0, 0, Qt::AlignVCenter);
  m_noticesLayout->addWidget(m_daysLeftLine, 0, 1, 3, 1);
  m_noticesLayout->addWidget(m_eventNameLabel, 0, 2);
  m_noticesLayout->addWidget(m_daysLeftDisplay, 1, 2, 2, 1);
  m_noticesLayout->addWidget(m_noticesWid, 1, 0, 2, 1);

  m_mealStuLayout->setAlignment(Qt::AlignTop);
  m_mealStuLayout->addWidget(m_mealStuTitle);
  m_mealStuLayout->addWidget(m_mealStuLabel, 0, Qt::AlignTop);

  m_stuOnDutyLayout->setAlignment(Qt::AlignTop);
  m_stuOnDutyLayout->addWidget(m_stuOnDutyTitle);
  m_stuOnDutyLayout->addWidget(m_stuOnDutyLabel, 0, Qt::AlignTop);

  QFile file("data.stm");

  if (!file.exists() || !ClassData::readFrom(&file, m_data)) {
    QMessageBox::critical(this, "ClassSystem",
                          "无法读取数据！<br/>程序将关闭。");
    exit(0);
  }
  file.close();

  reloadUi();
  loadFromIni();
  initLocalSocket();
}

void MainPanel::reloadUi() {
  /* ---------------------------- timers ---------------------------- */
  if (m_clockTimerId) killTimer(m_clockTimerId);
  m_clockTimerId = startTimer(500);

  if (m_noticeTimerId) killTimer(m_noticeTimerId);
  m_noticeTimerId = startTimer(5000);

  if (m_curLessonUpdateTimerId) killTimer(m_curLessonUpdateTimerId);
  m_curLessonUpdateTimerId = startTimer(1000);

  /* ---------------------------- lessons --------------------------- */

  m_lessons->clear();

  auto lessonsToday = m_data.lessons[dayToday()];
  for (int i = 0; i < m_data.lessonsTm.size(); ++i) {
    m_lessons->setItem(
        i, 0,
        new QTableWidgetItem("%1(%2-%3)"_s.arg(
            lessonsToday[i], m_data.lessonsTm[i].toString("hh:mm"),
            m_data.lessonsTm[i].addSecs(2400).toString("hh:mm"))));
  }

  /* ---------------------------- notices --------------------------- */

  // clear
  for (const auto &b : m_noticesTextBrowsers) {
    m_noticesTextBrowsers.removeOne(b);
    m_noticesWid->removeWidget(b);
    b->deleteLater();
  }

  for (const auto &[date, str, fontPtSize] : m_data.notices) {
    auto b = new QTextBrowser(this);
    b->setText(str);
    b->setStyleSheet("color: #d2d0ce;background-color: transparent");
    b->setFont(qFont{.pointSize = fontPtSize}());
    b->setAttribute(Qt::WA_TransparentForMouseEvents);
    b->setFocusPolicy(Qt::NoFocus);
    b->setFrameShape(QFrame::NoFrame);
    b->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    b->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_noticesTextBrowsers << b;
    m_noticesWid->addWidget(b);
  }

  /* --------------------------- days left -------------------------- */

  if (m_data.events.size()) {
    m_eventNameLabel->setText(
        "离%1剩余天数:"_s.arg(m_data.events.front().name));
    m_daysLeftDisplay->display(
        (int)QDate::currentDate().daysTo(m_data.events.front().date));
  } else {
    m_eventNameLabel->setText("无事件");
    m_daysLeftDisplay->display("");
  }

  /* --------------------- students carry meals --------------------- */

  auto mealStuToday = m_data.mealStu[dayToday()];
  QString mealStuStr;
  for (const uint &id : mealStuToday) {
    mealStuStr += "\n" + m_data.idAndName(id);
  }
  m_mealStuLabel->setText(mealStuStr.mid(1));

  /* ----------------------- students on duty ----------------------- */

  auto stuOnDutyToday = m_data.stuOnDuty[dayToday()];
  QString stuOnDutyStr;
  for (int i = 0; i < stuOnDutyToday.size(); ++i) {
    QList<uint> l = stuOnDutyToday[i];
    if (l.empty()) continue;
    QString displayStr =
        QString(
            R"(<font style="font-weight: 1000; font-size: 25pt; display: inline;">%1:</font>)")
            .arg(m_data.dutyJobs[i]);
    for (const uint &id : l) {
      if (!id) continue;
      displayStr += " " + m_data.idAndName(id);
    }
    stuOnDutyStr += "<br></br>" + displayStr;
  }
  m_stuOnDutyLabel->setText(stuOnDutyStr.mid(9));  // 移除第一个 `<br></br>`
}

void MainPanel::initLocalSocket() {
  m_socket->setServerName(kServerName);
  auto reconnectLater = [this]() {
    QTimer::singleShot(1000,
                       [this] { m_socket->connectToServer(kServerName); });
  };
  connect(m_socket, &QLocalSocket::readyRead, this, &MainPanel::onReadyRead);
  connect(m_socket, &QLocalSocket::connected, this, &MainPanel::onConnected);
  connect(m_socket, &QLocalSocket::errorOccurred,
          [this, reconnectLater](const QLocalSocket::LocalSocketError &err) {
            if (err != QLocalSocket::ServerNotFoundError ||
                err != QLocalSocket::ConnectionRefusedError) {
              qDebug() << m_socket->errorString();
            }
          });
  connect(m_socket, &QLocalSocket::disconnected, reconnectLater);
  m_socket->connectToServer(kServerName);
}

void MainPanel::onReadyRead() {
  QBuffer b;
  b.setData(m_socket->readAll());
  b.open(QBuffer::ReadWrite);
  QDataStream ds(&b);
  MsgType ty;
  ds >> ty;
  switch (ty) {
    case MsgType::Request: {
      QBuffer writeBuf;
      writeBuf.open(QBuffer::WriteOnly);
      ClassData::writeTo(m_data, &writeBuf);
      m_socket->write(writeBuf.data());
      break;
    }
    case MsgType::Save: {
      ClassData::readFrom(&b, m_data);
      QFile file("data.stm");
      ClassData::writeTo(m_data, &file);
      reloadUi();
      m_menu->m_tableWindow.reloadUi();
      break;
    }
  }
}

void MainPanel::onConnected() {}

bool MainPanel::nativeEvent(const QByteArray &, void *message, long *result) {
#ifdef _WIN32
  auto msg = static_cast<MSG *>(message);
  if (msg->message == WM_NCHITTEST) {  // resize
    if (!m_resizable) return false;

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
#endif  //_WIN32

  return false;
}

void MainPanel::paintEvent(QPaintEvent *) {
  if (m_init) {
    m_menu->show();
    // init geometry
    setGeometry(m_settings.value("geometry").toRect());
    m_init = false;
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
  if (!m_init) m_settings.setValue("geometry", geometry());
}

void MainPanel::moveEvent(QMoveEvent *) {
  if (!m_init) m_settings.setValue("geometry", geometry());
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
  if (ev->timerId() == m_curLessonUpdateTimerId) {
    const QTime kCur = QTime::currentTime();
    auto color = [](bool b) {
      return b ? QColor(229, 192, 123) : QColor(206, 210, 206);
    };

    for (int i = 0; i < m_data.lessonsTm.size(); ++i) {
      auto tm = m_data.lessonsTm[i];
      m_lessons->item(i, 0)->setForeground(
          color(kCur >= tm.addSecs(-600) && kCur <= tm.addSecs(2400)));
    }
  }
}