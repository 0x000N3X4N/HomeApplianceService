#include "comp_type_add.h"
#include "ui_comp_type_add.h"


CComp_TyAdd::CComp_TyAdd(QWidget* parent) :
  QDialog(parent),
  m_pUi(new Ui::comp_type_add_wnd)
{
  m_pUi->setupUi(this);
}

CComp_TyAdd::~CComp_TyAdd() {
  delete m_pUi;
}

void CComp_TyAdd::on_add_type_btn_clicked() {
  try {
    auto query_sptr = std::make_shared<CQueryController>(CODBCW::getInstance());
    QString cm_ty_qstr = m_pUi->product_type_lEdit->text();
    if (!query_sptr->executeSqlQuery(QString("INSERT INTO components_type VALUES('%1');")
                                .arg(cm_ty_qstr)))
      throw std::invalid_argument("CComp_TyAdd::on_add_type_btn_clicked : " + query_sptr->getQuery().lastError().text().toStdString());

    QMessageBox::information(this, "Component success added!", "Component type '" + cm_ty_qstr + "' was successfully added!");
  }
  catch(std::invalid_argument& e) {
    QMessageBox::critical(this, "Error!", e.what());
  }
  catch(...) {
    QMessageBox::critical(this, "Error!", "CComp_TyAdd::on_add_type_btn_clicked : Unexpected error!");
  }
}
