#include "editor.h"
#include "ui_editor.h"

CEditor::CEditor(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::EditorWindow)
{
  m_pUi->setupUi(this);
  m_pUi->complete_dateEdit->setCalendarPopup(true);
  m_pUi->complete_dateEdit->setDate(QDate::currentDate());
  m_pUi->status_comboBox->addItem("Open");
  m_pUi->status_comboBox->addItem("Started");
  m_pUi->status_comboBox->addItem("Closed");
}

CEditor::~CEditor() { delete m_pUi; }

void CEditor::showWindow(QTableView* p_tb_orders, Handle* hQuery) {
  show();
  m_pTbOrders = p_tb_orders;
  m_hQuery = hQuery;
}

void CEditor::on_confirm_btn_clicked() {
  QString query;
  if (m_pUi->complete_dateEdit->isEnabled())
    query = QString("UPDATE `home_appliance_service`.`order` SET "
                    "completion_date = '%1', status = '%2', completed_surname = '%3'"
                    "WHERE id_order = '%4'").arg(
                      m_pUi->complete_dateEdit->date().toString("yyyy-MM-dd"),
                      m_pUi->status_comboBox->currentText(),
                      m_pUi->complete_order_name_lEdit->text(),
                      m_pUi->id_order_value_spinBox->text());
  else
    query = QString("UPDATE `home_appliance_service`.`order` SET "
                    "status = '%1' WHERE id_order = '%2'")
                    .arg(m_pUi->status_comboBox->currentText(),
                         m_pUi->id_order_value_spinBox->text());

  m_hQuery->executeSqlQuery(query);
  m_hQuery->clear();

  m_hQuery->executeSqlQuery(
          "SELECT id_order AS 'Id', name AS 'Name', type AS 'Type', "
          "acceptance_date AS 'Accept_date', completion_date AS 'Complete_date', "
          "total AS 'Total', status AS 'Status', completed_surname AS 'Surname' "
          "FROM home_appliance_service.order"
        );

  QSqlQueryModel* hModel = new QSqlQueryModel();
  QSortFilterProxyModel* hFilterModel = new QSortFilterProxyModel();
  hModel->setQuery(m_hQuery->getQuery());
  hFilterModel->setSourceModel(hModel);
  m_pTbOrders->setModel(hFilterModel);
}

void CEditor::on_status_comboBox_currentIndexChanged(int index) {
  if (index == 2) {
    m_pUi->complete_dateEdit->setEnabled(true);
    m_pUi->complete_order_name_lEdit->setEnabled(true);
  } else {
    m_pUi->complete_dateEdit->setEnabled(false);
    m_pUi->complete_order_name_lEdit->setEnabled(false);
  }
}
