#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QDebug>
#include <QtSql>
#include <QMessageBox>
#include "customers.h"
#include "pricelist.h"
#include "order_add.h"
#include "order_deleter.h"
#include "statistic.h"
#include "employees.h"
#include "export.h"
#include "search.h"
#include "PCSM_msg_box.h"
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
  void showStatistic();
  void showEmployees(QSortFilterProxyModel* res_qsfpm);
  void showCustomers(QSortFilterProxyModel* res_qsfpm);
  void showAddOrder(QTableView* ptb_orders,
                    std::vector<std::tuple<size_t, QString, double>> vCompsV,
                    std::map<QString, size_t> employees_map, std::map<QString, size_t> cust_map);
  void showDelOrder(QTableView* tb_ptr,
                    std::tuple <
                                   std::vector<QString>, // order title
                                   std::vector<QString>, // order employeer
                                   std::vector<QString>, // order acceptance date
                                   std::vector<QString>  // order price
                               > o_struct);
  void tbSendPriceList(QSortFilterProxyModel* hFModelComp,
                       QSortFilterProxyModel* hFModelCompType);
  void showExport(QTableView* orders_tb_ptr);

private slots:
  void on_price_btn_clicked();
  void on_add_order_btn_clicked();
  void on_statistic_btn_clicked();
  void on_employees_btn_clicked();
  void on_customers_btn_clicked();
  void on_del_order_btn_clicked();
  void on_export_btn_clicked();

private:
  Ui::MainWindow* m_pUi;
  CPriceList* m_pPriceListWindow;
  COrderAdd* m_pOrderAdd;
  COrderDel* m_pOrderDeleter;
  CStatistic* m_pStatistic;
  CEmployees* m_pEmployees;
  CCustomers* m_pCustomers;
  CODBCW* m_hDb;
  CExport* m_pExport;
  CSearch* m_pSearch;
  CQueryController* m_hQuery; // query handler
  QSqlQueryModel* m_hModel;
  QSortFilterProxyModel* m_hFilterModel;

  void initTbOrders();
};

#endif // MAINWINDOW_H
