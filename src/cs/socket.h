#pragma once

#include <qabstractsocket.h>

namespace cs::socket {
using callback_type = std::function<void(const nlohmann::json &)>;

// key: cmd, value: callbackfunc
inline QMap<QString, callback_type> callbacks;

namespace details {
inline bool complete = true;  // 解决TCP拆包，是否出现拆包
inline QByteArray buf;        // 解决TCP拆包，拆包的前半部分
inline int leftLen = 0;       // 解决TCP拆包，剩余长度

inline void doCallBack(const QByteArray &ba) {
  auto json = nlohmann::json::parse(ba.constData());
  auto cmd = QString::fromStdString(json["cmd"].get<std::string>());

  if (callbacks.contains(cmd)) callbacks[cmd](json["data"]);
}
}  // namespace details

inline void recvMsg(QAbstractSocket *socket) {
  using namespace details;

  if (!complete) {
    if (socket->bytesAvailable() < leftLen) return;

    doCallBack(buf + socket->read(leftLen));

    buf.clear();
    leftLen = 0;
    complete = true;
  }

  while (socket->bytesAvailable() >= sizeof(int)) {
    int len = ntohl(*(int *)socket->read(sizeof(int)).constData());
    if (socket->bytesAvailable() < len) {
      leftLen = len;
      buf = socket->readAll();
      return;
    }

    doCallBack(socket->read(len));
  }
}
}  // namespace cs::socket
