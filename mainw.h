#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QDebug>
#include <QtSql>
#include <QMessageBox>
#include "pricelist.h"
#include "record.h"
#include "editor.h"
#include "statistic.h"
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
  void showAddOrder();
  void showEditor(QTableView* tb_orders, CQueryController* hQuery);
  void showStatistic();
  void tbSendPriceList(QSortFilterProxyModel* );
  void sendPriceList(QTableView* pTbOrders, CQueryController* hQuery,
                     std::shared_ptr<double[]> pPriceList,
                     std::vector<QString> aEquipmentName);


private slots:
  void on_price_btn_clicked();
  void on_add_order_btn_clicked();
  void on_edit_btn_clicked();
  void on_statistic_btn_clicked();

private:
  Ui::MainWindow* m_pUi;
  CPriceList* m_pPriceListWindow;
  CEditor* m_pEditor;
  CRecord* m_pRecord;
  CStatistic* m_pStatistic;
  CODBCW* m_hDb;
  CQueryController* m_hQuery; // query handler
  QSqlQueryModel* m_hModel;
  QSortFilterProxyModel* m_hFilterModel;

  void initTbOrders();
};

#endif // MAINWINDOW_H
