#ifndef EMPLOYEE_DELETER_H
#define EMPLOYEE_DELETER_H


#include <QDialog>


namespace Ui {
  class EmployeeDeleterWindow;
}


class CEmployeeDel : public QDialog {
  Q_OBJECT

public:
  explicit CEmployeeDel(QWidget *parent = nullptr);
  ~CEmployeeDel();

public slots:
  void showWindow(QString* pFN, size_t sz);

private:
  Ui::EmployeeDeleterWindow* m_pUi;
};


#endif // EMPLOYEE_DELETER_H
