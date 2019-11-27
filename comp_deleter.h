#ifndef SERVICE_DELETER_H
#define SERVICE_DELETER_H


#include <QDialog>
#include <QTableView>
#include <QMessageBox>
#include "PCSM_base_types.h"
#include "libs/ODBCConnector/query_controller.h"


namespace Ui {
  class CompDeleterWindow;
}

class CCompDeleter : public QDialog {
  Q_OBJECT

public:
  explicit CCompDeleter(QWidget *parent = nullptr);
  ~CCompDeleter();

public slots:
  void showWindow(QTableView* hTbPriceList, component_itemT& _map_comp_items);
  void on_accept_deleter_btn_clicked();

private:
  void upd_comp_items();
  void clearUi();

  Ui::CompDeleterWindow* m_pUi;
  CQueryController* m_hQuery;
  QTableView* m_hTbPriceList;
  component_itemT m_map_comp_items;
};


#endif // SERVICE_DELETER_H
