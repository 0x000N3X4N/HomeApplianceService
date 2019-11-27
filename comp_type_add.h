#ifndef COMP_TYPE_ADD_H
#define COMP_TYPE_ADD_H


#include <QDialog>
#include <QMessageBox>
#include <QTableView>
#include "libs/ODBCConnector/query_controller.h"
#include "libs/ODBCConnector/ODBCW.h"


namespace Ui {
  class comp_type_add_wnd;
}

class CComp_TyAdd : public QDialog {
  Q_OBJECT

public:
  explicit CComp_TyAdd(QWidget *parent = nullptr);
  ~CComp_TyAdd();

private slots:
  void on_add_type_btn_clicked();
  void clearUi();

public slots:
  void showWindow(QTableView* hTbCompTy);

private:
  Ui::comp_type_add_wnd* m_pUi;
  std::shared_ptr<CQueryController> m_query_sptr;
  QTableView* m_hTbCompTy;
};


#endif // COMP_TYPE_ADD_H
