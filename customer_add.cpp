#include "customer_add.h"
#include "ui_customer_add.h"


CCustomerAdd::CCustomerAdd(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::CustomerAddWindow)
{
  m_pUi->setupUi(this);
  m_pUi->phone_le->setValidator(new QRegExpValidator(QRegExp("^[+][0-9]*")));

  connect(this, &CCustomerAdd::finished,
          this, &CCustomerAdd::clearUi);
}

CCustomerAdd::~CCustomerAdd()
{
  delete m_pUi;
  delete m_pCust_tb;
}

void CCustomerAdd::showWindow(QTableView* cust_tb_ptr,
                              std::map<QString, size_t> city_map) {
  m_city_map = city_map;

  for (auto it = city_map.begin(); it != city_map.end(); ++it)
    m_pUi->city_cBox->addItem(it->first);

  m_pCust_tb = cust_tb_ptr;
  show();
}

void CCustomerAdd::on_submit_btn_clicked() {
  try {
    std::size_t i = 0;
    CQueryController query(CQueryController(CODBCW::getInstance("", nullptr, &i)));

    auto it = m_city_map.find(m_pUi->city_cBox->currentText());
    auto street_qstr = m_pUi->street_le->text();
    QString street_query_qstr = QString("INSERT INTO street "
                                        "VALUES ('%1', '%2', '%3', '%4', '%5');")
                                        .arg(QString::number(it->second),
                                             street_qstr,
                                             QString::number(m_pUi->house_num_sBox->value()),
                                             QString::number(m_pUi->porch_sBox->value()),
                                             QString::number(m_pUi->floor_sBox->value()));

    if (query.executeSqlQuery(street_query_qstr)) {
      query.clear();
      QString getStreetIdQ = QString("SELECT PK_street_id FROM street WHERE [name] = '%1';")
                             .arg(street_qstr);

      if (query.executeSqlQuery(getStreetIdQ)) {
        size_t nStreet_id;

        while(query.next())
          nStreet_id = query.parse_value(0).toUInt();

        query.clear();

        QString cust_name_qstr = m_pUi->name_le->text();
        QString custQ = QString("INSERT INTO customers "
                                "VALUES ('%1', '%2', '%3')")
                                .arg(QString::number(nStreet_id),
                                     cust_name_qstr,
                                     m_pUi->phone_le->text());

        if (query.executeSqlQuery(custQ)) {
          query.clear();

          if (query.executeSqlQuery("SELECT name AS 'Name', phone AS 'Phone' "
                                    "FROM customers;"))
          {
            QSqlQueryModel* hCustQModel = new QSqlQueryModel();
            QSortFilterProxyModel* hCustQSModel = new QSortFilterProxyModel();
            hCustQModel->setQuery(query.getQuery());
            hCustQSModel->setDynamicSortFilter(true);
            hCustQSModel->setSourceModel(hCustQModel);
            m_pCust_tb->setModel(hCustQSModel);

            CMsgBox::show(QMessageBox::Information, this, "Success!", "Customer '" + cust_name_qstr + "' was succesfully added!");
          }
          else
            throw std::invalid_argument("Error, query for SELECT FROM 'customers' not executed! [" +
                                        query.getQuery().lastError().text().toStdString());
        }
        else
          throw std::invalid_argument("Error, query for 'custQ' not executed! [" +
                                      query.getQuery().lastError().text().toStdString());
      }
      else
        throw std::invalid_argument("Error, query for '"
                                    "getStreetIdQ' not executed! [" +
                                    query.getQuery().lastError().text().toStdString() + "]");
    }
    else
      throw std::invalid_argument("Error, query for INSERT street not executed! [" +
                                  query.getQuery().lastError().text().toStdString() + "]");
  }
  catch(std::invalid_argument& e) {
    CMsgBox::show(QMessageBox::Critical, this, e.what(), e.what());
    return;
  }
  catch(...) {
    CMsgBox::show(QMessageBox::Critical, this, "Error!", "CCustomerAdd::on_submit_btn_clicked : Unexpected error!");
  }
}

void CCustomerAdd::clearUi() {
  m_pUi->name_le->clear();
  m_pUi->phone_le->clear();
  m_pUi->street_le->clear();
  m_pUi->city_cBox->clear();
  m_pUi->house_num_sBox->setValue(0);
  m_pUi->porch_sBox->setValue(0);
  m_pUi->floor_sBox->setValue(0);
}
