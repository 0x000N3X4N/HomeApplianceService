#include "employee_deleter.h"
#include "ui_employee_deleter.h"


CEmployeeDel::CEmployeeDel(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::EmployeeDeleterWindow)
{
  m_tb_ptr = new QTableView();

  connect(this, &CEmployeeDel::finished,
          this, &CEmployeeDel::clearUi);

  m_pUi->setupUi(this);
}

CEmployeeDel::~CEmployeeDel() {
  delete m_pUi;
  delete m_tb_ptr;
}

void CEmployeeDel::showWindow(QString* pFN, size_t sz, QTableView* tb_ptr) {
  for (size_t i = 0; i < sz; ++i)
    m_pUi->fullname_cBox->addItem(pFN[i]);

  m_tb_ptr = tb_ptr;
  show();
}

void CEmployeeDel::on_submit_btn_clicked() {
  size_t i = 0;
  CQueryController query(CQueryController(CODBCW::getInstance("", nullptr, &i)));

  try {
    QString query_qstr = QString("DELETE FROM employees WHERE fullname = '%1';").arg(m_pUi->fullname_cBox->currentText());

    if (query.executeSqlQuery(query_qstr)) {
      CMsgBox::show(QMessageBox::Information, this, "Success!", "Employee '" + m_pUi->fullname_cBox->currentText() +
                    "' was succesfully deleted!");

      query.clear();

      if (query.executeSqlQuery("SELECT fullname AS 'Fullname', post AS 'Post', "
                                "salary AS 'Salary', working_hours AS 'Working hours' "
                                "FROM employees;")) {
        QSqlQueryModel* hEmpQModel = new QSqlQueryModel();
        QSortFilterProxyModel* hEmpFilterModel = new QSortFilterProxyModel();
        hEmpQModel->setQuery(query.getQuery());
        hEmpFilterModel->setDynamicSortFilter(true);
        hEmpFilterModel->setSourceModel(hEmpQModel);
        m_tb_ptr->setModel(hEmpFilterModel);

        emit updOrders();
      }
      else
        throw std::invalid_argument("CEmployeeDel::on_submit_btn_clicked : Error execute SELECT query! LastError: [" +
                                    query.getQuery().lastError().text().toStdString() + "]");
    }
    else
      throw std::invalid_argument("CEmployeeDel::on_submit_btn_clicked : Error execute DELETE query! LastError: [" +
                                  query.getQuery().lastError().text().toStdString() + "]");

    clearUi();
  }
  catch(std::invalid_argument& e) {
    CMsgBox::show(QMessageBox::Critical, this, "Error!", e.what());
    return;
  }
  catch(...) {
    CMsgBox::show(QMessageBox::Critical, this, "Error!", "CEmployeeDel::on_submit_btn_clicked : Unexpected error! LastError: [" +
                  query.getQuery().lastError().text() + "]");
  }
}

void CEmployeeDel::clearUi() {
  m_pUi->fullname_cBox->clear();
}
