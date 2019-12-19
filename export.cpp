#include "export.h"
#include "ui_export.h"


CExport::CExport(QWidget *parent) :
  QDialog(parent),
  m_pUi(new Ui::ExportWindow)
{
  m_pUi->setupUi(this);
  m_pUi->type_cBox->addItem("*.CSV");
}

CExport::~CExport() {
  delete m_pUi;
}

void CExport::showWindow(QTableView* orders_tb_ptr) {
  m_orders_tb = orders_tb_ptr;
  show();
}

void CExport::saveAsCSV() {
  QFile f(m_pUi->path_le->text());

  if (f.open(QIODevice::WriteOnly)) {
    QTextStream ts(&f);
    QStringList strList;

    strList << "\" \"";

    for(int c = 0; c < m_orders_tb->horizontalHeader()->count(); ++c)
        strList << "\"" + m_orders_tb->model()->headerData(c, Qt::Horizontal).toString()+"\"";

    ts << strList.join( ";" )+"\n";

    for(int r = 0; r < m_orders_tb->verticalHeader()->count(); ++r) {
        strList.clear();
        strList << "\"" + m_orders_tb->model()->headerData(r, Qt::Vertical).toString()+"\"";

        for(int c = 0; c < m_orders_tb->horizontalHeader()->count(); ++c)
            strList << "\"" + m_orders_tb->model()->data(m_orders_tb->model()->index(r, c), Qt::DisplayRole).toString()+"\"";

        ts << strList.join( ";" )+"\n";
    }

    f.close();
  }
}

void CExport::on_export_btn_clicked() {
  saveAsCSV();
}
