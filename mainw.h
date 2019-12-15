#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QDebug>
#include <QtSql>
#include <QMessageBox>
#include "customers.h"
#include "pricelist.h"
#include "order_add.h"
#include "editor.h"
#include "statistic.h"
#include "employees.h"
#include "libs/ODBCConnector/ODBCW.h"
#include "libs/ODBCConnector/query_controller.h"


namespace Ui {
  class MainWindow;
}


class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

signals:
  void showPriceList();
  void showEditor(QTableView* ptb_orders, CQueryController* hQuery);
  void showStatistic();
  void showEmployees(QSortFilterProxyModel* res_qsfpm);
  void showCustomers(QSortFilterProxyModel* res_qsfpm);
  void showAddOrder(QTableView* ptb_orders,
                    std::vector<std::tuple<size_t, QString, double>> vCompsV,
                    std::map<QString, size_t> employees_map, std::map<QString, size_t> cust_map);
  void tbSendPriceList(QSortFilterProxyModel* hFModelComp,
                       QSortFilterProxyModel* hFModelCompType);


private slots:
  void on_price_btn_clicked();
  void on_add_order_btn_clicked();
  void on_edit_btn_clicked();
  void on_statistic_btn_clicked();
  void on_employees_btn_clicked();
  void on_customers_btn_clicked();

private:
  Ui::MainWindow* m_pUi;
  CPriceList* m_pPriceListWindow;
  CEditor* m_pEditor;
  COrderAdd* m_pOrderAdd;
  CStatistic* m_pStatistic;
  CEmployees* m_pEmployees;
  CCustomers* m_pCustomers;
  CODBCW* m_hDb;
  CQueryController* m_hQuery; // query handler
  QSqlQueryModel* m_hModel;
  QSortFilterProxyModel* m_hFilterModel;

  void initTbOrders();
};

#endif // MAINWINDOW_H
