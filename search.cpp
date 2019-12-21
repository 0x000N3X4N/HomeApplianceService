#include "search.h"
#include "ui_search.h"


CSearch::CSearch(QWidget *parent) :
  QWidget(parent),
  m_pUi(new Ui::SearchWindow)
{
  m_pUi->setupUi(this);
}

CSearch::~CSearch()
{
  delete m_pUi;
}

void CSearch::on_search_btn_clicked() {

}
