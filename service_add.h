#ifndef PRICEADD_H
#define PRICEADD_H


#include <QDialog>
#include <QTableView>
#include <QMessageBox>
#include <array>
#include "libs/ODBCConnector/query_controller.h"


namespace Ui {
  class ServiceAddWindow;
}

class CServiceAdd : public QDialog {
  Q_OBJECT

public:
  explicit CServiceAdd(QWidget *parent = nullptr);
  ~CServiceAdd();

public slots:
  void showWindow(QTableView* hTbPriceList);

private slots:
  void on_add_service_btn_clicked();

private:
  Ui::ServiceAddWindow* m_pUi;
  CQueryController* m_hQuery;
  QTableView* m_hTbPriceList;
  //TODO: webhook and fetch list of equipment from remote server
  std::array<QString, 20> m_listOfEquipment {
    "1",
    "2",
    "3"
  };
};

#endif // PRICEADD_H
