#ifndef SERVICE_DELETER_H
#define SERVICE_DELETER_H


#include <QDialog>
#include <QTableView>
#include <QMessageBox>
#include "PCSM_base_types.h"
#include "component.h"
#include "libs/ODBCConnector/query_controller.h"


namespace Ui {
  class CompDeleterWindow;
}


class CCompDeleter : public QDialog, public PCOM::CCompsTraits {
  Q_OBJECT

public:
  explicit CCompDeleter(QWidget *parent = nullptr);
  ~CCompDeleter();

public slots:
  void showWindow(QTableView* hTbPriceList);
  void on_accept_deleter_btn_clicked();

private:
  void clearUi();

  Ui::CompDeleterWindow* m_pUi;
  CQueryController* m_hQuery;
  QTableView* m_hTbPriceList;
};


#endif // SERVICE_DELETER_H
