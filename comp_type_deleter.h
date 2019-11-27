#ifndef COMP_TYPE_DELETER_H
#define COMP_TYPE_DELETER_H


#include <QDialog>
#include <QTableView>
#include "comp_add.h"


namespace Ui {
  class comp_type_deleter;
}

class CComp_TyDeleter : public QDialog {
  Q_OBJECT

public:
  explicit CComp_TyDeleter(QWidget *parent = nullptr);
  ~CComp_TyDeleter();

public slots:
  void showWindow(QTableView* hTbCompType);
  void on_delete_comp_type_btn_clicked();

private:
  Ui::comp_type_deleter* m_pUi;
  QTableView* m_pTbCompType;
};


#endif // COMP_TYPE_DELETER_H
