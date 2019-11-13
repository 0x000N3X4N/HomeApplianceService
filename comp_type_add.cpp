#include "comp_type_add.h"
#include "ui_comp_type_add.h"


CComp_TyAdd::CComp_TyAdd(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::CComp_TyAdd)
{
  m_pUi->setupUi(this);
}

CComp_TyAdd::~CComp_TyAdd() {
  delete m_pUi;
}
