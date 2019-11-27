#ifndef COMPADD_H
#define COMPADD_H


#include <QDialog>
#include <QTableView>
#include <QMessageBox>
#include "PCSM_base_types.h"
#include "component.h"
#include "libs/ODBCConnector/query_controller.h"


namespace Ui {
  class ServiceAddWindow;
}

//TODO: component class
class CCompAdd : public QDialog, public CComponents {
  Q_OBJECT

public:
  explicit CCompAdd(QWidget *parent = nullptr);
  ~CCompAdd();

private slots:
  void clearUi();

public slots:
  void showWindow(QTableView* hTbPriceList);
  void on_add_service_btn_clicked();

private:
  void update_comp_items();

  Ui::ServiceAddWindow* m_pUi;
  CQueryController* m_hQuery;
  QTableView* m_hTbPriceList;
};


#endif // COMPADD_H
