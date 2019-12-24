#include "order_deleter.h"
#include "ui_order_deleter.h"


COrderDel::COrderDel(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::OrderWindow)
{
  m_pUi->setupUi(this);

  connect(this, &COrderDel::finished,
          this, &COrderDel::clearUi);
}

COrderDel::~COrderDel()
{
  delete m_pUi;
}

void COrderDel::showWindow(QTableView* tb_ptr,
                           std::tuple <
                                        std::vector<QString>, // order title
                                        std::vector<QString>, // order employeer
                                        std::vector<QString>, // order acceptance date
                                        std::vector<QString>  // order price
                                      > o_struct)
{
  // fill order titles
  std::for_each(std::get<0>(o_struct).begin(), std::get<0>(o_struct).end(), [this](QString& title) {
    m_pUi->o_title_cBox->addItem(title);
  });

  // fill order employeers
  std::for_each(std::get<1>(o_struct).begin(), std::get<1>(o_struct).end(), [this](QString& empl) {
    m_pUi->o_empl_cBox->addItem(empl);
  });

  // fill order acceptance date
  std::for_each(std::get<2>(o_struct).begin(), std::get<2>(o_struct).end(), [this](QString& a_date) {
    m_pUi->o_a_date_cBox->addItem(a_date);
  });

  // fill order prices
  std::for_each(std::get<3>(o_struct).begin(), std::get<3>(o_struct).end(), [this](QString& price) {
    m_pUi->o_price_cBox->addItem(price);
  });

  m_tb_ptr = tb_ptr;

  show();
}

void COrderDel::clearUi() {
  m_pUi->o_title_cBox->clear();
  m_pUi->o_empl_cBox->clear();
  m_pUi->o_a_date_cBox->clear();
  m_pUi->o_price_cBox->clear();
}

void COrderDel::on_accept_btn_clicked() {
  size_t i = 0;
  CQueryController query_ctrl(CQueryController(CODBCW::getInstance("", nullptr, &i)));

  try {
    QString ordersQ = QString("DELETE FROM orders "
                              "WHERE ( "
                                      "FK_component_id = (SELECT C.PK_component_id "
                                                         "FROM components C "
                                                         "WHERE C.title = '%1') AND "
                                      "FK_employee_id =  (SELECT E.PK_employee_id "
                                                         "FROM employees E "
                                                         "WHERE E.fullname = '%2') AND "
                                      "price = %3 AND "
                                      "acceptance_date = '%4');").arg(
                                      m_pUi->o_title_cBox->currentText(),
                                      m_pUi->o_empl_cBox->currentText(),
                                      m_pUi->o_price_cBox->currentText(),
                                      m_pUi->o_a_date_cBox->currentText());

    if (query_ctrl.executeSqlQuery(ordersQ))
    {
      bool isAffected = query_ctrl.getQuery().numRowsAffected() > 0;

      if (!isAffected) throw std::invalid_argument("Error, record not found!");

      query_ctrl.clear();

      if (query_ctrl.executeSqlQuery("SELECT C.title AS 'Title', E.fullname AS 'Employee ', "
                                     "O.acceptance_date AS 'Acceptance date', O.price AS 'Price' "
                                     "FROM orders O "
                                     "JOIN components C "
                                     "ON O.FK_component_id = C.PK_component_id "
                                     "JOIN employees E "
                                     "ON o.FK_employee_id = e.PK_employee_id;"))
      {
        QMessageBox::information(this, "Success!", "Order was successfilly deleted!");

        QSqlQueryModel* hOrdersQModel = new QSqlQueryModel();
        QSortFilterProxyModel* hOrdersFilterModel = new QSortFilterProxyModel();
        hOrdersQModel->setQuery(query_ctrl.getQuery());
        hOrdersFilterModel->setDynamicSortFilter(true);
        hOrdersFilterModel->setSourceModel(hOrdersQModel);

        m_tb_ptr->setModel(hOrdersQModel);
      }
      else
        throw std::invalid_argument("Error, can't execute SELECT FROM `orders` query!  LastError: [" +
                                    query_ctrl.getQuery().lastError().text().toStdString() + "]");
    }
    else
      throw std::invalid_argument("Error, can't execute DELETE FROM `orders` query!  LastError: [" +
                                  query_ctrl.getQuery().lastError().text().toStdString() + "]");
  } catch(std::invalid_argument& e) {
    QMessageBox::critical(this, e.what(), e.what());
    return;
  }
  catch(...) {
    QMessageBox::critical(this, "Error!", "COrderDel::on_accept_btn_clicked : Unexpected error! LastError: [" +
                          query_ctrl.getQuery().lastError().text() + "]");
    return;
  }
}
