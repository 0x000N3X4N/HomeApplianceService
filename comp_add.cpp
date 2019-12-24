#include "comp_add.h"
#include "ui_comp_add.h"


CCompAdd::CCompAdd(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::ServiceAddWindow)
{
  m_pUi->setupUi(this);
  size_t i = 0;
  m_hQuery = new CQueryController(CQueryController(CODBCW::getInstance("", nullptr, &i)));


  QWidget::connect(this, &CCompAdd::finished,
          this, &CCompAdd::clearUi);

  m_pUi->release_date_dEdit->setCalendarPopup(true);
  m_pUi->release_date_dEdit->setLocale(QLocale::English);
}

CCompAdd::~CCompAdd() {
  delete m_pUi;
  delete m_hQuery;
}

void CCompAdd::clearUi() {
  CCompsTraits::get_comps().get()->clear();
  m_pUi->spec_lEdit->clear();
  m_pUi->component_name_lEdit->clear();
  m_pUi->component_type_comboBox->clear();
  m_pUi->price_dSpinBox->clear();
}

void CCompAdd::showWindow(QTableView* hTbPriceList) {
  m_hTbPriceList = hTbPriceList;
  CCompsTraits::get_comps().upd(*m_pUi->component_type_comboBox);
  this->show();
}

void CCompAdd::on_add_comp_btn_clicked() {
  auto it = CCompsTraits::get_comps().find(m_pUi->component_type_comboBox->currentText());

  QString sQuery = QString("INSERT INTO components "
                           "VALUES ('%1', '%2', '%3', '%4', '%5')")
                           .arg(QString::number((*it)->get_id()),
                                m_pUi->component_name_lEdit->text(),
                                m_pUi->spec_lEdit->text(),
                                QString::number(m_pUi->price_dSpinBox->value()),
                                QString::number(m_pUi->release_date_dEdit->date().year()));
qDebug() << sQuery;
  try {
    if (m_hQuery->executeSqlQuery(sQuery)) {
      QMessageBox::information(this, "Success!", "Component '" + m_pUi->component_name_lEdit->text()
                                                               + "' was successfully added!");
      m_hQuery->clear();
      sQuery = "SELECT title AS 'Title', component_type AS 'Component type',"
               " specifications AS 'Specifications', price AS 'Price', release_date AS 'Release date' "
               "FROM components "
               "JOIN components_type "
               "ON FK_type_code = PK_component_type_id;";
      m_hQuery->executeSqlQuery(sQuery);
      QSqlQueryModel* hModel = new QSqlQueryModel();
      QSortFilterProxyModel* hFilterModel = new QSortFilterProxyModel();
      hModel->setQuery(m_hQuery->getQuery());
      hFilterModel->setDynamicSortFilter(true);
      hFilterModel->setSourceModel(hModel);
      m_hTbPriceList->setModel(hFilterModel);
    } else
      throw std::invalid_argument("CCompAdd::on_add_service_btn_clicked : " +
                                  m_hQuery->getQuery().lastError().text().toStdString());
  }
  catch(std::invalid_argument& e) {
    QMessageBox::critical(this, "Error!", e.what());
  }
  catch(...) {
    QMessageBox::critical(this, "Error!", "CCompAdd::on_add_service_btn_clicked : Unexpected error! LastError: [" +
                          m_hQuery->getQuery().lastError().text() + "]");
  }
}
