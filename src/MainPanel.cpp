#include "MainPanel.h"

#include <qbrush.h>
#include <qbuffer.h>
#include <qdatetime.h>
#include <qhostaddress.h>
#include <qmessagebox.h>
#include <qpaintengine.h>
#include <qpainter.h>
#include <qprocess.h>
#include <qrandom.h>
#include <qsizepolicy.h>
#include <qthread.h>

#include "ClassData.h"
#include "UpdateDownloader.h"
#include "cs/socket.h"

MainPanel::MainPanel(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint) {
  cs::setWidgetTransparent(this);
  cs::setParentToDesktop(this);

  initSocket();
}

void MainPanel::initSocket() {
  connect(m_socket, &QTcpSocket::connected, this, &MainPanel::onConnected);
  connect(m_socket, &QTcpSocket::readyRead,
          std::bind(cs::socket::recvMsg, m_socket));

  auto &callbacks = cs::socket::callbacks;
  callbacks["ERROR"] = [this](const nlohmann::json &data) {
    const auto errorType = data["type"].get<std::string>();
    const auto errorStr = data["str"].get<std::string>();

    QMessageBox::critical(
        this, "ClassSystem 错误",
        QString::asprintf("服务器错误：%s", errorStr.c_str()));

    qCritical("Server Error[%s]: %s", errorType.c_str(), errorStr.c_str());
    QApplication::quit();
  };

  callbacks["DATA"] = [this](const nlohmann::json &data) {
    classData = ClassData::parse(data);
    m_menu->m_tableWindow.loadData();
  };

  callbacks["UPDATE"] = [this](const nlohmann::json &) {
    auto downloader = new UpdateDownloader();
    connect(this, &MainPanel::updatesAvailable, downloader,
            &UpdateDownloader::download);

    auto downloadThread = new QThread();
    downloadThread->start();
    downloader->moveToThread(downloadThread);

    emit updatesAvailable();
  };

  m_socket->connectToHost(cs::settings::serverHost,
                          cs::settings::tcpServerPort);

  QMessageBox msgBox("ClassSystem 提示", "正在连接到服务器...",
                     QMessageBox::NoIcon, 0, 0, 0, nullptr);
  msgBox.setWindowFlag(Qt::WindowCloseButtonHint, false);
  msgBox.setStandardButtons(QMessageBox::NoButton);
  cs::setParentToDesktop(&msgBox);
  msgBox.show();

  QEventLoop loop;
  QTimer timer;
  ushort seconds = 0;
  timer.start(1000);
  bool connected = false;
  connect(m_socket, &QTcpSocket::connected, [this, &loop, &connected] {
    loop.quit();
    connected = true;
    onConnected();
  });
  connect(
      &timer, &QTimer::timeout, [&seconds, connected, &timer, &loop, &msgBox] {
        ++seconds;
        if (seconds >= cs::settings::connectionTimeoutSeconds || connected) {
          timer.stop();
          loop.quit();
        }
        msgBox.setText(QString::asprintf("正在连接到服务器...%us", seconds));
      });
  loop.exec();

  if (!connected) {
    qCritical("Connect to server timeout(10s).");
    msgBox.close();
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle("ClassSystem 错误");
    msgBox.setText("无法连接到服务器，请检查网络连接！");
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.exec();
    exit(1);
  }
}

void MainPanel::onConnected() {
  nlohmann::json json = {{"class", cs::settings::className.toStdString()},
                         {"version", cs::config::projectVersion}};
  m_socket->write(json.dump().c_str());
  m_socket->flush();
}

void MainPanel::saveGeometry() {
  if (m_init) return;

  using namespace cs::settings;

  ini.beginGroup("Gui");
  ini.setValue("geometry", geometry());
  ini.endGroup();
}

bool MainPanel::nativeEvent(const QByteArray &eventType, void *message,
                            long *result) {
  MSG *msg = static_cast<MSG *>(message);
  if (msg->message == WM_NCHITTEST) {
    if (isMaximized()) {
      return false;
    }

    *result = 0;
    constexpr LONG borderWidth = 8;
    RECT winrect;
    GetWindowRect(reinterpret_cast<HWND>(winId()),
                  &winrect);  // must be short to correctly work with multiple
                              // monitors (negative coordinates)
    short x = msg->lParam & 0x0000FFFF;
    short y = (msg->lParam & 0xFFFF0000) >> 16;
    bool resizeWidth = minimumWidth() != maximumWidth();
    bool resizeHeight = minimumHeight() != maximumHeight();
    if (resizeWidth) {  // left border
      if (x >= winrect.left && x < winrect.left + borderWidth) {
        *result = HTLEFT;
      }  // right border
      if (x < winrect.right && x >= winrect.right - borderWidth) {
        *result = HTRIGHT;
      }
    }
    if (resizeHeight) {  // bottom border
      if (y < winrect.bottom && y >= winrect.bottom - borderWidth) {
        *result = HTBOTTOM;
      }  // top border
      if (y >= winrect.top && y < winrect.top + borderWidth) {
        *result = HTTOP;
      }
    }
    if (resizeWidth && resizeHeight) {  // bottom left corner
      if (x >= winrect.left && x < winrect.left + borderWidth &&
          y < winrect.bottom && y >= winrect.bottom - borderWidth) {
        *result = HTBOTTOMLEFT;
      }  // bottom right corner
      if (x < winrect.right && x >= winrect.right - borderWidth &&
          y < winrect.bottom && y >= winrect.bottom - borderWidth) {
        *result = HTBOTTOMRIGHT;
      }  // top left corner
      if (x >= winrect.left && x < winrect.left + borderWidth &&
          y >= winrect.top && y < winrect.top + borderWidth) {
        *result = HTTOPLEFT;
      }  // top right corner
      if (x < winrect.right && x >= winrect.right - borderWidth &&
          y >= winrect.top && y < winrect.top + borderWidth) {
        *result = HTTOPRIGHT;
      }
    }
    if (*result != 0) return true;

    QWidget *action = QApplication::widgetAt(QCursor::pos());
    if (action == this) {
      *result = HTCAPTION;
      return true;
    }
  }
  return false;
}

void MainPanel::paintEvent(QPaintEvent *) {
  if (m_init) {
    m_menu->show();

    // init geometry
    using namespace cs::settings;

    ini.beginGroup("Gui");
    setGeometry(ini.value("geometry").toRect());
    ini.endGroup();

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

void MainPanel::mousePressEvent(QMouseEvent *ev) {
  m_mouseStartPoint = pos() - ev->globalPos();
}

void MainPanel::mouseMoveEvent(QMouseEvent *ev) {
  auto boundedPos = [this](const QPoint &pos) {
    const QSize kScreenSize = QApplication::primaryScreen()->availableSize();
    const int kBoundedX = qBound(0, pos.x(), kScreenSize.width() - width());
    const int kBoundedY = qBound(0, pos.y(), kScreenSize.height() - height());
    return QPoint(kBoundedX, kBoundedY);
  };
  auto unbounded = ev->globalPos() + m_mouseStartPoint,
       bounded = boundedPos(unbounded);

  // 出界时重置鼠标初始点
  if (bounded != unbounded) m_mouseStartPoint = pos() - ev->globalPos();

  move(bounded);
}

void MainPanel::resizeEvent(QResizeEvent *) { saveGeometry(); }

void MainPanel::moveEvent(QMoveEvent *) { saveGeometry(); }

void MainPanel::closeEvent(QCloseEvent *ev) { QApplication::quit(); }