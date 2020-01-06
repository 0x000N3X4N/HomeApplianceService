#ifndef EMPLOYEE_DELETER_H
#define EMPLOYEE_DELETER_H


#include <QDialog>
#include <QTableView>
#include "PCSM_msg_box.h"
#include "libs/ODBCConnector/query_controller.h"


namespace Ui {
  class EmployeeDeleterWindow;
}


class CEmployeeDel : public QDialog {
  Q_OBJECT

public:
  explicit CEmployeeDel(QWidget *parent = nullptr);
  ~CEmployeeDel();

public slots:
  void showWindow(QString* pFN, size_t sz, QTableView* tb_ptr);

private slots:
  void on_submit_btn_clicked();
  void clearUi();

signals:
  void updOrders();

private:
  Ui::EmployeeDeleterWindow* m_pUi;
  QTableView* m_tb_ptr;
};


#endif // EMPLOYEE_DELETER_H
