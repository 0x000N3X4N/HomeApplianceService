#ifndef COMP_TYPE_DELETER_H
#define COMP_TYPE_DELETER_H


#include <QDialog>
#include <QTableView>
#include "PCSM_base_types.h"
#include "component.h"
#include "libs/ODBCConnector/query_controller.h"


namespace Ui {
  class comp_type_deleter;
}

class CComp_TyDeleter : public QDialog, public PCOM::CCompsTraits {
  Q_OBJECT

public:
  explicit CComp_TyDeleter(QWidget *parent = nullptr);
  ~CComp_TyDeleter();

public slots:
  void showWindow(QTableView* hTbCompType, QTableView* hTbComps);
  void on_delete_comp_type_btn_clicked();

private slots:
  void clearUi();

signals:
  void updOrders();

private:
  Ui::comp_type_deleter* m_pUi;
  QTableView* m_pTbCompType;
  QTableView* m_pTbComps;
  CQueryController* m_hQuery;
};


#endif // COMP_TYPE_DELETER_H
