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


class CCompAdd : public QDialog, public PCOM::CCompsTraits {
  Q_OBJECT

public:
  explicit CCompAdd(QWidget *parent = nullptr);
  ~CCompAdd();

private slots:
  void clearUi();
  void on_add_comp_btn_clicked();

public slots:
  void showWindow(QTableView* hTbPriceList);

private:
  Ui::ServiceAddWindow* m_pUi;
  CQueryController* m_hQuery;
  QTableView* m_hTbPriceList;
};


#endif // COMPADD_H
