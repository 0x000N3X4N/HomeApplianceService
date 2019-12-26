#include "employees.h"
#include "ui_employees.h"


CEmployees::CEmployees(QWidget *parent) :
  QWidget(parent),
  m_pUi(new Ui::EmployeesWindow) {
  m_pEmpl_add = new CEmployeeAdd();
  m_pEmpl_del = new CEmployeeDel();

  connect (this, &CEmployees::showEmplAdd,
           m_pEmpl_add, &CEmployeeAdd::showWindow);
  connect (this, &CEmployees::showEmplDel,
           m_pEmpl_del, &CEmployeeDel::showWindow);

  m_pUi->setupUi(this);
  m_pUi->employees_tb->horizontalHeader()
                     ->setSectionResizeMode(QHeaderView::Stretch);
  m_pUi->employees_tb->setSortingEnabled(true);
}

CEmployeeDel* CEmployees::getEmplDel() const { return m_pEmpl_del; }

CEmployees::~CEmployees() {
  delete m_pUi;
  delete m_pEmpl_add;
  delete m_pEmpl_del;
}

void CEmployees::showWindow(QSortFilterProxyModel* query_qsfpm_ptr) {
  show();
  m_pUi->employees_tb->setModel(query_qsfpm_ptr);
}

void CEmployees::on_add_employee_btn_clicked() {
  emit showEmplAdd(m_pUi->employees_tb);
}

void CEmployees::on_delete_employee_btn_clicked() {
  size_t i = 0;
  CQueryController query(CQueryController(CODBCW::getInstance("", nullptr, &i)));

  try {

    if (query.executeSqlQuery("SELECT fullname FROM employees;")) {
      size_t ecx = 0;
      QString* pFN_qstr = new QString[20000];

      while (query.next()) {
        pFN_qstr[ecx] = query.parse_value(0).toString();
        ++ecx;
      }

      emit showEmplDel(pFN_qstr, ecx, m_pUi->employees_tb);
    }
    else
      throw std::invalid_argument("Error, query for select employees not executed! LastError: [" +
                                  query.getQuery().lastError().text().toStdString() + "]");
  }
  catch(std::invalid_argument& e) {
    CMsgBox::show(QMessageBox::Critical, this, "Error!", e.what());
    return;
  }
  catch(...) {
    CMsgBox::show(QMessageBox::Critical, this, "Error!", "CEmployees::on_delete_employee_btn_clicked : Unexpected error! LastError: [" +
                  query.getQuery().lastError().text() + "]");
  }
}
