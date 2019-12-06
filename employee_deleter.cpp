#include "employee_deleter.h"
#include "ui_employee_deleter.h"


CEmployeeDel::CEmployeeDel(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::EmployeeDeleterWindow)
{
  m_pUi->setupUi(this);
}

CEmployeeDel::~CEmployeeDel() {
  delete m_pUi;
}

void CEmployeeDel::showWindow(QString* pFN, size_t sz) {
  for (size_t i = 0; i < sz; ++i)
    m_pUi->fullname_cBox->addItem(pFN[i]);

  show();
}

void CEmployeeDel::on_submit_btn_clicked() {
  //TODO:
}
