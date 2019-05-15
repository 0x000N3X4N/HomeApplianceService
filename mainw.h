#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QtSql>
#include <QMessageBox>
#include "pricelist.h"
#include "record.h"
#include "editor.h"
#include "libs/xSql/xSqlConnector.h"
#include "libs/xSql/handle.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

signals:
  void showPriceList();
  void showAddOrder();
  void showEditor(QTableView* tb_orders, Handle* hQuery);
  void tbSendPriceList(QSortFilterProxyModel* );
  void sendPriceList(QTableView* pTbOrders, Handle* hQuery,
                     std::shared_ptr<double[]> pPriceList,
                     size_t sizeOfPriceList);


private slots:
  void on_price_btn_clicked();
  void on_add_order_btn_clicked();
  void on_edit_btn_clicked();

private:
  Ui::MainWindow* m_pUi;
  PriceList* m_pPriceListWindow;
  CEditor* m_pEditor;
  Record* m_pRecord;
  XSqlDatabase* m_hDb;
  Handle* m_hQuery; // query handler
  QSqlQueryModel* m_hModel;
  QSortFilterProxyModel* m_hFilterModel;

  void initTbOrders();
};

#endif // MAINWINDOW_H
