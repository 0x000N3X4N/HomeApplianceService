#ifndef EMPLOYEES_H
#define EMPLOYEES_H


#include <QWidget>
#include <QTableView>
#include "PCSM_msg_box.h"
#include "libs/ODBCConnector/query_controller.h"
#include "employee_add.h"
#include "employee_deleter.h"


namespace Ui {
  class EmployeesWindow;
}


class CEmployees : public QWidget {
  Q_OBJECT

public:
  explicit CEmployees(QWidget *parent = nullptr);
  CEmployeeDel* getEmplDel() const;
  ~CEmployees();

signals:
  void showEmplAdd(QTableView* hTbEmployees);
  void showEmplDel(QString* pFN, size_t sz, QTableView* tb_ptr);

public slots:
  void showWindow(QSortFilterProxyModel* query_qsfpm_ptr);

private slots:
  void on_add_employee_btn_clicked();
  void on_delete_employee_btn_clicked();

private:
  Ui::EmployeesWindow* m_pUi;
  CEmployeeAdd* m_pEmpl_add;
  CEmployeeDel* m_pEmpl_del;
};


#endif // EMPLOYEES_H
