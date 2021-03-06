#ifndef CUSTOMER_DELETER_H
#define CUSTOMER_DELETER_H


#include <QDialog>
#include <QTableView>
#include "PCSM_msg_box.h"
#include "libs/ODBCConnector/query_controller.h"


namespace Ui {
  class CustomerDeleterWIndow;
}


class CCustomerDel : public QDialog
{
  Q_OBJECT

public:
  explicit CCustomerDel(QWidget *parent = nullptr);
  ~CCustomerDel();

public slots:
  void showWindow(QTableView* cust_tb_ptr, std::vector<QString> vCust);

private slots:
  void clearUi();
  void on_submit_btn_clicked();

signals:
  void updOrders();

private:
  void updateUi();
  void fillFN();

  Ui::CustomerDeleterWIndow* m_pUi;
  std::vector<QString> m_vCust;
  QTableView* m_pCustTb;
};


#endif // CUSTOMER_DELETER_H
