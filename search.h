#ifndef SEARCH_H
#define SEARCH_H


#include <QWidget>
#include <QStack>
#include "compare.h"
#include "libs/ODBCConnector/query_controller.h"


namespace Ui {
  class SearchWindow;
}

enum QT {
  NotFound,
  Param,
  NParam,
  ParamWildcard,
  NParamWildcard
};

namespace QueryParamsKeywords {
  constexpr auto PRICE = "price";
  constexpr auto YEAR = "year";
  constexpr auto COUNT = 2;
}

namespace CMP {
  constexpr auto MAX = 7;
}

namespace QPK = QueryParamsKeywords;

struct Params {
  QString price;
  QString year;
};

struct QueryParser {
  static std::tuple<std::map<QString, QString>, std::vector<QString>> parseParams(QString& query) {
    auto query_qstr = query;
    int off = 0, off_ = 0;
    std::map<QString, QString> params_map;
    std::vector<QString> undefined_params;

    while (true) {
      off = query_qstr.indexOf('[');
      off_ = query_qstr.indexOf(']');
      if (off == -1 && off_ == -1) break;
      QString param = query_qstr.mid(off + 1, off_ - off - 1);
      auto QStrList = param.split(':');

      if (!(param.contains(QPK::PRICE) || param.contains(QPK::YEAR)))
        undefined_params.push_back(param);
      else if (!QStrList.empty() && (QStrList[0] == QPK::PRICE || QStrList[0] == QPK::YEAR))
        params_map[QStrList[0]] = QStrList[1];

      query_qstr.remove(off, off_ - off + 1);
    }

    return std::make_tuple(params_map, undefined_params);
  }
};

struct QueryTyper {
private:
  QString extractBrackets(QString& in) {
      QString backets_str;

      for (auto it = in.begin(); it != in.end(); it++)
          if (*it == '[' || *it == ']') backets_str.push_back(*it);

      return backets_str;
  }

  bool isBalanced(QString& brackets_str) {
      QStack<QChar> s;
      QChar c;

      for (auto it = brackets_str.begin(); it != brackets_str.end(); it++)
      {
          if (*it == '[')
              s.push(*it);
          else if (s.empty()) return false;

          if (*it == ']') {
              c = s.top();

              s.pop();
              continue;
          }
      }

      return s.empty();
  }

public:
  std::pair<std::tuple<std::map<QString, QString>, std::vector<QString>>, QT> fetch(QString& query) {
    auto brackets_query = extractBrackets(query);
    bool bBalanced = isBalanced(brackets_query);

    if (!bBalanced)
      throw std::invalid_argument("Error, brackets are not balanced!");

    auto params_container = QueryParser::parseParams(query);
    auto cp_query = query;

    if (brackets_query != "" && bBalanced && !std::get<0>(params_container).empty()) {
      int off = cp_query.indexOf('[');
      cp_query.remove(off, cp_query.length() - off);

      return std::make_pair(params_container, cp_query.contains('*') ? QT::ParamWildcard : QT::Param);
    }
    else if (std::get<0>(params_container).empty()) {
      return std::make_pair(params_container, cp_query.contains('*') ? QT::NParamWildcard : QT::NParam);
    }
    else
      return std::make_pair(params_container, QT::NotFound);
  }
};


class CSearch : public QWidget {
  Q_OBJECT

public:
  explicit CSearch(QWidget *parent = nullptr);
  ~CSearch();

private slots:
  void on_search_btn_clicked();
  void on_cmp_btn_clicked();

signals:
  void showCompareWindow(std::map<QString, QStringList> cmp_item_map);

private:
  bool isOnlyDigits(std::string str);
  QString parse_search_qeury(QString query_title, QStringList price,
                             QStringList year);

  CCompare* m_hCmp;
  Ui::SearchWindow* m_pUi;
};


#endif // SEARCH_H
