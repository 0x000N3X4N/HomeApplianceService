#ifndef COMPARE_H
#define COMPARE_H


#include <QWidget>
#include <QLabel>


namespace Ui {
  class CompareWindow;
}


class CCompare : public QWidget {
  Q_OBJECT

public:
  explicit CCompare(QWidget *parent = nullptr);
  ~CCompare();

public slots:
  void showWindow(std::map<QString, QStringList> cmp_item_map);

private:
  Ui::CompareWindow* m_pUi;
};

#endif // COMPARE_H
