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
  void showWindow();

private slots:
  void on_name_comboBox_currentIndexChanged(int index);
  void on_accept_btn_clicked();
  void on_status_comboBox_currentIndexChanged(int index);
//  void on_guarantee_dial_valueChanged(int value);
//  void on_guarantee_dial_sliderReleased();
//  void on_RecordWindow_finished(int result);

private:
  Ui::OrderAddWindow* m_pUi;
  CQueryController* m_hQuery;
  struct priceListHandler;
  priceListHandler* m_pPriceListHandler;
  std::map<std::vector<QString>,
           std::shared_ptr<double[]>> m_priceListStructure;
  std::map<std::vector<QString>,
           std::shared_ptr<double[]>>::iterator m_map_it;
  QTableView* m_pTbOrders;
  double m_total;
  double m_prevTotal;
  int m_guarantee;
};

#endif // RECORD_H
