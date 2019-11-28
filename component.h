#ifndef COMPONENT_H
#define COMPONENT_H


#include <QObject>
#include <QString>
#include <vector>
#include <memory>


template <typename NameT = QString, typename IdT = size_t>
struct comp;


class CCompsTraits;


template <typename NameT, typename IdT>
struct comp : public QObject {
  IdT       get_id()   const   { return id;   }
  NameT     get_name() const   { return name; }
protected:
  NameT name;
  IdT id;
};


template <typename _CompsTy = std::vector<std::shared_ptr<comp<>>>>
class CComponents : public comp<> {
public:
  typedef _CompsTy CompsT;
  typedef typename CompsT::iterator CompsT_it;

  CComponents();

  CompsT* get()              const { return &comps;  }
  void    set(CompsT _comps) const { comps = _comps; }

protected:
  static CompsT comps;
};


class CCompsTraits : public CComponents<> {
public:
  bool rm(QString name) const;
  CompsT_it find(QString name) const {
    auto it = comps.begin();
    for(; it != comps.end(); ++it)
      if ((*it)->get_name() == name) return it;

    return it;
  }
};


#endif // COMPONENT_H
