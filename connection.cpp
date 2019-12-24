#include "connection.h"
#include "ui_connection.h"


CConnection::CConnection(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::ConnectionWindow)
{
  m_pUi->setupUi(this);
  QString host_qstr = QHostInfo::localHostName();

  connect(this, &CConnection::showMainWindow,
          this, [this](){m_hMain->show();});

  m_pUi->srv_name_le->setText(host_qstr);
}

CConnection::~CConnection()
{
  delete m_pUi;
}

void CConnection::on_accept_btn_clicked() {
  std::size_t isOpen = 0;
  m_hQuery = new CQueryController(CODBCW::getInstance(m_pUi->srv_name_le->text(),
                                                      m_pUi->status_val_l, &isOpen));
  m_hMain = new MainWindow();
  if (isOpen) {
    emit showMainWindow();
    hide();
  }
}
