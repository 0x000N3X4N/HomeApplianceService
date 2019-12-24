#ifndef COMPARE_H
#define COMPARE_H


#include <QWidget>
#include <QDebug>
#include <QLabel>


namespace Ui {
  class CompareWindow;
}


class CCompare : public QWidget {
  Q_OBJECT

public:
  explicit CCompare(QWidget *parent = nullptr);
  ~CCompare() override;

public slots:
  void showWindow(std::map<QString, QStringList> cmp_item_map);
  void clearWidgets(QLayout* layout);

protected:
  void closeEvent(QCloseEvent *event) override;

private:
  Ui::CompareWindow* m_pUi;
};

#endif // COMPARE_H
