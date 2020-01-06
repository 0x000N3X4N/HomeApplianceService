#ifndef EMPLOYEE_ADD_H
#define EMPLOYEE_ADD_H


#include <QDialog>
#include <QTableView>
#include "PCSM_msg_box.h"
#include "libs/ODBCConnector/query_controller.h"


namespace Ui {
  class EmployeeAddWindow;
}


class CEmployeeAdd : public QDialog {
  Q_OBJECT

public:
  explicit CEmployeeAdd(QWidget *parent = nullptr);
  ~CEmployeeAdd();

public slots:
  void showWindow(QTableView* employees_tb_ptr);

private slots:
  void on_submit_btn_clicked();
  void clearUi();

private:
  Ui::EmployeeAddWindow* m_pUi;
  QTableView* m_pEmployeesTb;
};


#endif // EMPLOYEE_ADD_H
