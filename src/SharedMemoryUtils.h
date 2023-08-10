#pragma once

#include <qbuffer.h>
#include <qdatastream.h>
#include <qsharedmemory.h>

#include <cstring>

namespace SharedMemoryUtils {

constexpr inline const char kMemoryKey[] = "CLASS-SYSTEM-ADMIN-KEY";
inline QSharedMemory memory(kMemoryKey);

template <typename T>
inline void __fromDataStream(QDataStream &in, T &t) {
  QVariant v;
  in >> v;
  t = v.value<T>();
}

template <typename T>
inline void __toDataStream(QDataStream &out, const T &t) {
  QVariant v;
  v.setValue(t);
  out << v;
}

template <typename... Arg>
inline bool read(Arg &...args) {
  memory.attach();

  QBuffer buffer;
  QDataStream in(&buffer);

  memory.lock();

  buffer.setData((char *)memory.constData(), memory.size());
  if (!buffer.open(QBuffer::ReadWrite)) {
    qDebug() << buffer.errorString();
    return false;
  }

  int tmp[] = {(__fromDataStream(in, args), 0)...};

  memory.unlock();
  memory.detach();
  return true;
}

template <typename... Arg>
inline bool write(const Arg &...args) {
  if (memory.isAttached() && !memory.detach()) return false;

  QBuffer buffer;
  buffer.open(QBuffer::ReadWrite);
  QDataStream out(&buffer);
  int tmp[] = {(__toDataStream(out, args), 0)...};
  if (!memory.create(buffer.size())) return false;
  char *to = (char *)memory.data();
  const char *from = buffer.data().data();
  memcpy(to, from, std::min(memory.size(), (int)buffer.size()));

  memory.unlock();
  qDebug() << "write:" << memory.errorString() << memory.size()
           << memory.data();
  return true;
}

}  // namespace SharedMemoryUtils