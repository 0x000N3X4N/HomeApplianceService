#include "employee_add.h"
#include "ui_employee_add.h"


CEmployeeAdd::CEmployeeAdd(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::EmployeeAddWindow)
{
  m_pUi->setupUi(this);


  connect(this, &CEmployeeAdd::finished,
          this, &CEmployeeAdd::clearUi);
}

CEmployeeAdd::~CEmployeeAdd()
{
  delete m_pUi;
}

void CEmployeeAdd::showWindow(QTableView* employees_tb_ptr) {
  m_pEmployeesTb = employees_tb_ptr;
  show();
}

void CEmployeeAdd::on_submit_btn_clicked() {
  try {
    CQueryController query_ctrl(CODBCW::getInstance());
    QString empl_name_qstr = m_pUi->fullname_le->text();
    QString query_qstr = QString("INSERT INTO employees"
                         " VALUES ('%1', '%2', '%3', '%4');").arg(empl_name_qstr,
                                                                m_pUi->post_le->text(),
                                                                QString::number(m_pUi->price_dsBox->value()),
                                                                QString::number(m_pUi->working_hours_sBox->value()));

    if (query_ctrl.executeSqlQuery(query_qstr)) {
      QMessageBox::information(this, "Success!", "Employee '" + empl_name_qstr + "' was succesfully added!");

      query_ctrl.clear();

      if (query_ctrl.executeSqlQuery("SELECT fullname AS 'Fullname', post AS 'Post', "
                                     "salary AS 'Salary', working_hours AS 'Working hours' "
                                     "FROM employees;"))
      {
        QSqlQueryModel* hEmpQModel = new QSqlQueryModel();
        QSortFilterProxyModel* hEmpFilterModel = new QSortFilterProxyModel();
        hEmpQModel->setQuery(query_ctrl.getQuery());
        hEmpFilterModel->setDynamicSortFilter(true);
        hEmpFilterModel->setSourceModel(hEmpQModel);
        m_pEmployeesTb->setModel(hEmpFilterModel);
      }
      else
        throw std::invalid_argument("Error, query for select employees not executed!");

      clearUi();
    }
    else
      throw std::invalid_argument("Error, query for employees not executed!");
  }
  catch(std::invalid_argument& e) {
    QMessageBox::critical(this, e.what(), e.what());
    return;
  }
  catch(...) {
    QMessageBox::critical(this, "Error!", "CEmployeeAdd::on_submit_btn_clicked : Unexpected error!");
  }
}

void CEmployeeAdd::clearUi() {
  m_pUi->fullname_le->clear();
  m_pUi->post_le->clear();
  m_pUi->price_dsBox->setValue(0);
  m_pUi->working_hours_sBox->setValue(8);
}
