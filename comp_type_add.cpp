#include "comp_type_add.h"
#include "ui_comp_type_add.h"


CComp_TyAdd::CComp_TyAdd(QWidget* parent) :
  QDialog(parent),
  m_pUi(new Ui::comp_type_add_wnd)
{
  m_pUi->setupUi(this);
  m_query_sptr = std::make_shared<CQueryController>(CODBCW::getInstance());

  connect(this, &CComp_TyAdd::finished,
          this, &CComp_TyAdd::clearUi);
}

CComp_TyAdd::~CComp_TyAdd() {
  delete m_pUi;
}

void CComp_TyAdd::on_add_type_btn_clicked() {
  try {
    QString cm_ty_qstr = m_pUi->product_type_lEdit->text();
    if (!m_query_sptr->executeSqlQuery(QString("INSERT INTO components_type VALUES('%1');")
                                     .arg(cm_ty_qstr)))
      throw std::invalid_argument("CComp_TyAdd::on_add_type_btn_clicked : " + m_query_sptr->getQuery().lastError().text().toStdString());

    QMessageBox::information(this, "Component success added!", "Component type '" + cm_ty_qstr + "' was successfully added!");
    m_query_sptr->clear();

    if (!m_query_sptr->executeSqlQuery("SELECT component_type AS 'Component type' FROM components_type;")) {
      throw std::invalid_argument("CComp_TyAdd::showWindow : " +
                                  m_query_sptr->getQuery().lastError().text().toStdString());
    }
    else {
      QSqlQueryModel* hModel = new QSqlQueryModel();
      QSortFilterProxyModel* hFilterModel = new QSortFilterProxyModel();
      hModel->setQuery(m_query_sptr->getQuery());
      hFilterModel->setDynamicSortFilter(true);
      hFilterModel->setSourceModel(hModel);
      m_hTbCompTy->setModel(hFilterModel);
    }
  }
  catch(std::invalid_argument& e) {
    QMessageBox::critical(this, "Error!", e.what());
  }
  catch(...) {
    QMessageBox::critical(this, "Error!", "CComp_TyAdd::on_add_type_btn_clicked : Unexpected error!");
  }
}

void CComp_TyAdd::clearUi() {
  m_pUi->product_type_lEdit->clear();
}

void CComp_TyAdd::showWindow(QTableView* hTbCompTy) {
  m_hTbCompTy = hTbCompTy;
  this->show();
}
