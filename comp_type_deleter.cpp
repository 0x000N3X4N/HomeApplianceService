#include "comp_type_deleter.h"
#include "ui_comp_type_deleter.h"


CComp_TyDeleter::CComp_TyDeleter(QWidget* parent) :
  QDialog(parent),
  m_pUi(new Ui::comp_type_deleter)
{
  m_pUi->setupUi(this);
  m_pTbCompType = new QTableView();
}

CComp_TyDeleter::~CComp_TyDeleter() {
  delete m_pUi;
}

void CComp_TyDeleter::showWindow(QTableView* hTbCompType) {
  m_pTbCompType = hTbCompType;
}

void CComp_TyDeleter::on_delete_comp_type_btn_clicked() {

}
