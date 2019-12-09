#ifndef CUSTOMERS_H
#define CUSTOMERS_H


#include <QWidget>
#include "customer_add.h"
#include "libs/ODBCConnector/query_controller.h"


namespace Ui {
  class CustomersWindow;
}


class CCustomers : public QWidget {
  Q_OBJECT

public:
  explicit CCustomers(QWidget *parent = nullptr);
  ~CCustomers();

public slots:
  void showWindow(QSortFilterProxyModel* res_qsfpm);

private slots:
  void on_add_customer_btn_clicked();

signals:
  void showCustomerAdd(QTableView* cust_tb_ptr, std::map<QString, size_t> city_map);

private:
  Ui::CustomersWindow* m_pUi;
  CCustomerAdd* m_pCust_add;
};


#endif // CUSTOMERS_H
