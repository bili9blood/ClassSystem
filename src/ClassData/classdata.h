#ifndef CLASSDATA_H
#define CLASSDATA_H

#include <QDataStream>

class ClassData : public QObject {
  Q_OBJECT
 public:
  explicit ClassData(QIODevice* device = new QFile("data.stm"),
                     QObject* parent = nullptr);
  ~ClassData();
  QVector<QVector<QVector<int>>> onDuty;
  QVector<QVector<int>> fan;
  QVector<QString> notices;
  int numOfStudents, lastGroup;
  QDate lastWorked;
  QHash<int, QString> students;
  inline QString idAndName(int id) {
    return QString::number(id) + students.value(id);
  }
  QVector<QVector<QString>> lessons = QVector(5, QVector(8, QString()));
  QVector<QTime> timeLessonsStart = QVector(8, QTime());
  QString eventName;
  QDate eventDate;
  inline QIODevice* device() { return mDevice; }
  void setDevice(QIODevice* newDevice) { mDevice = newDevice; }
 public slots:
  void load();
  void save();

 private:
  QPointer<QIODevice> mDevice;
  QDataStream dataStream;
  inline void testData() {
    onDuty = {{{2, 0}, {11, 0}, {37, 49}, {13, 33}, {40, 52}, {7, 15}},
              {{36, 0}, {42, 0}, {22, 8}, {31, 12}, {19, 10}, {21, 0}},
              {{23, 0}, {41, 0}, {1, 51}, {35, 38}, {47, 46}, {16, 0}},
              {{39, 0}, {43, 0}, {18, 48}, {44, 50}, {34, 14}, {4, 54}},
              {{20, 0}, {3, 0}, {6, 17}, {32, 9}, {53, 45}, {5, 0}}};
    fan = {{1, 2, 3},    {23, 5, 6},   {7, 8, 10},   {11, 12, 13}, {22, 15, 16},
           {17, 20, 19}, {18, 21, 14}, {4, 31, 32},  {33, 34, 35}, {36, 37, 38},
           {39, 40, 44}, {45, 54, 52}, {48, 50, 51}, {53, 46, 47}};
    notices = {
        "NONE__<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" "
        "\"http://www.w3.org/TR/REC-html40/"
        "strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" "
        "/><style type=\"text/css\">\np, li { white-space: pre-wrap; "
        "}\n</style></head><body style=\" font-family:'SimSun'; "
        "font-size:9pt; font-weight:400; font-style:normal;\">\n<p "
        "style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; "
        "margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span "
        "style=\" font-size:18pt; "
        "font-weight:600;\">中午抬饭的同学务必在12:35前将饭抬下！ "
        "</span></p></body></html>"};
    students = {{1, "陈皓林"},  {2, "陈梓畅"},  {3, "冯梓墨"},  {4, "高任"},
                {5, "郭俊晞"},  {6, "金正"},    {7, "黎玮峰"},  {8, "李诚涛"},
                {9, "李力衡"},  {10, "李子韬"}, {11, "梁智超"}, {12, "马鸿键"},
                {13, "马铄滨"}, {14, "舒楷迪"}, {15, "谭琪臻"}, {16, "韦睿"},
                {17, "文昱翀"}, {18, "向俊熙"}, {19, "余承孝"}, {20, "余炯晟"},
                {21, "钟杰龙"}, {22, "朱科晓"}, {23, "祝邦哲"}, {31, "曾潼潼"},
                {32, "陈心琳"}, {33, "陈雅琳"}, {34, "陈奕妃"}, {35, "陈誉嘉"},
                {36, "邓嘉瑜"}, {37, "黄楚涵"}, {38, "黄熙童"}, {39, "黄依琳"},
                {40, "李涤非"}, {41, "李宛芸"}, {42, "潘楚滢"}, {43, "潘可可"},
                {44, "彭紫玉"}, {45, "吴卓滢"}, {46, "肖祉恩"}, {47, "杨靖瑶"},
                {48, "姚可睿"}, {49, "姚瑶"},   {50, "张思琦"}, {51, "张秀秀"},
                {52, "张梓璇"}, {53, "郑茵予"}, {54, "邹雨轩"}};
    lessons = {
        {"班会", "语文", "英语", "美术", "政治", "地理", "数学", "体育"},
        {"数学", "语文", "生物", "英语", "历史", "英语", "信息", "劳动"},
        {"阅览", "政治", "体育", "音乐", "语文", "数学", "综合", "地理"},
        {"英语", "英语", "历史", "语文", "语文", "数学", "生物", "体育"},
        {"英语", "心理", "校本", "数学", "数学", "语文", "校本", "语文"}};
    timeLessonsStart = {QTime(8, 0),   QTime(8, 50),  QTime(9, 40),
                        QTime(10, 40), QTime(11, 35), QTime(14, 10),
                        QTime(15, 05), QTime(15, 55)};
    numOfStudents = 47;
    lastGroup = 7;
    lastWorked = {2023, 6, 14};
    eventName = "期末考试";
    eventDate = {2023, 6, 27};
    save();
  }
};

#endif  // CLASSDATA_H
