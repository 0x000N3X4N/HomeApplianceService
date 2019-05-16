#ifndef EDITOR_H
#define EDITOR_H

#include <QDialog>
#include <QTableView>
#include "libs/xSql/handle.h"

namespace Ui {
  class EditorWindow;
}

class CEditor : public QDialog
{
  Q_OBJECT

public:
  explicit CEditor(QWidget *parent = nullptr);
  ~CEditor();

public slots:
  void showWindow(QTableView* p_tb_orders, Handle* hQuery);

private slots:
  void on_confirm_btn_clicked();
  void on_status_comboBox_currentIndexChanged(int index);

private:
  Ui::EditorWindow* m_pUi;
  Handle* m_hQuery;
  QTableView* m_pTbOrders;
};

#endif // EDITOR_H
