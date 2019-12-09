#ifndef CUSTOMER_DELETER_H
#define CUSTOMER_DELETER_H


#include <QDialog>


namespace Ui {
  class CustomerDeleterWIndow;
}


class CCustomerDel : public QDialog
{
  Q_OBJECT

public:
  explicit CCustomerDel(QWidget *parent = nullptr);
  ~CCustomerDel();

private:
  Ui::CustomerDeleterWIndow* m_pUi;
};


#endif // CUSTOMER_DELETER_H
