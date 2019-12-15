#ifndef ORDER_DEL_H
#define ORDER_DEL_H


#include <QDialog>


namespace Ui {
  class OrderWindow;
}


class COrderDel : public QDialog {
  Q_OBJECT

public:
  explicit COrderDel(QWidget *parent = nullptr);
  ~COrderDel();

private:
  Ui::OrderWindow* m_pUi;
};

#endif // ORDER_DEL_H
