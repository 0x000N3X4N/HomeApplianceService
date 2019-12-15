#ifndef RECORD_H
#define RECORD_H


#include <QDialog>
#include <QDebug>
#include <QTableView>
#include <QMessageBox>
#include <memory>
#include "libs/ODBCConnector/query_controller.h"


namespace Ui {
  class OrderAddWindow;
}


class COrderAdd : public QDialog {
  Q_OBJECT

public:
  explicit COrderAdd(QWidget* parent = nullptr);
  ~COrderAdd();

public slots:
  void showWindow(QTableView* ptb_orders,
                  std::vector<std::tuple<size_t, QString, double>> vCompsV,
                  std::map<QString, size_t> employees_map, std::map<QString, size_t> cust_map);

private slots:
  void on_accept_btn_clicked();
  void clearForm();
  void on_comp_cBox_currentIndexChanged(int index);
  void on_comp_cnt_dial_valueChanged(int value);

private:
  double fetch_price(QString key_qstr);
  void fillInParams();

  Ui::OrderAddWindow* m_pUi;
  std::vector<std::tuple<size_t, QString, double>> m_vCompsV;
  std::map<QString, size_t> m_employees_map;
  std::map<QString, size_t> m_cust_map;
  CQueryController* m_hQuery;
  QTableView* m_pTbOrders;
  double m_total;
};


#endif // RECORD_H
