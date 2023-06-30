#pragma once
#include <QAction>
#include <QBitmap>
#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QSpacerItem>
#include <QWidget>

class MenuWidget : public QWidget {
  Q_OBJECT
 public:
  explicit MenuWidget(QWidget *parent = nullptr);
  ~MenuWidget() override;
  MenuWidget &addToMenu(const QString &text, const QPixmap &icon);
 signals:
  void clicked(size_t id);

 private:
  QList<QWidget *> mBtns{};
  QList<QVBoxLayout *> mLayouts;
  QList<QPixmap> mPixs;
  QList<QLabel *> mIconLabels;
  QList<QLabel *> mTextLabels;
  QHBoxLayout *mLayout = new QHBoxLayout(this);
  QSpacerItem *spacer = new QSpacerItem(20, 40);

 protected:
  bool eventFilter(QObject *obj, QEvent *ev) override;
  void paintEvent(QPaintEvent *ev) override;
};
