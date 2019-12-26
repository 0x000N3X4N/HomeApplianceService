#ifndef CUSTOMERS_H
#define CUSTOMERS_H


#include <QWidget>
#include "customer_add.h"
#include "customer_deleter.h"
#include "PCSM_msg_box.h"
#include "libs/ODBCConnector/query_controller.h"


namespace Ui {
  class CustomersWindow;
}


class CCustomers : public QWidget {
  Q_OBJECT

public:
  explicit CCustomers(QWidget *parent = nullptr);
  CCustomerDel* getCustDel() const;
  ~CCustomers();

public slots:
  void showWindow(QSortFilterProxyModel* res_qsfpm);

private slots:
  void on_add_customer_btn_clicked();
  void on_delete_customer_btn_clicked();

signals:
  void showCustomerAdd(QTableView* cust_tb_ptr, std::map<QString, size_t> city_map);
  void showCustomerDel(QTableView* cust_tb_ptr, std::vector<QString> vCust);

private:
  Ui::CustomersWindow* m_pUi;
  CCustomerAdd* m_pCust_add;
  CCustomerDel* m_pCust_del;
};


#endif // CUSTOMERS_H
