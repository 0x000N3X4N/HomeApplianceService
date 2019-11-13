#ifndef COMP_TYPE_ADD_H
#define COMP_TYPE_ADD_H


#include <QDialog>


namespace Ui {
  class CComp_TyAdd;
}

class CComp_TyAdd : public QDialog {
  Q_OBJECT

public:
  explicit CComp_TyAdd(QWidget *parent = nullptr);
  ~CComp_TyAdd();

private:
  Ui::CComp_TyAdd* m_pUi;
};

#endif // COMP_TYPE_ADD_H
