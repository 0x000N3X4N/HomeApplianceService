#ifndef COMP_TYPE_ADD_H
#define COMP_TYPE_ADD_H


#include <QDialog>


namespace Ui {
  class comp_type_add_wnd;
}

class CComp_TyAdd : public QDialog {
  Q_OBJECT

public:
  explicit CComp_TyAdd(QWidget *parent = nullptr);
  ~CComp_TyAdd();

private:
  Ui::comp_type_add_wnd* m_pUi;
};


#endif // COMP_TYPE_ADD_H
