#ifndef COMPONENT_H
#define COMPONENT_H


#include <QObject>
#include <QString>
#include <QComboBox>
#include <vector>
#include <memory>


namespace PCSM {

  namespace COMP {


    template <typename NameT = QString, typename IdT = size_t>
    struct comp;


    class CCompsTraits;


    template <typename NameT, typename IdT>
    struct comp : public QObject {
      comp(NameT _name, IdT _id) :
        m_name(_name),
        m_id(_id) { }

      IdT       get_id()   const   { return m_id;   }
      NameT     get_name() const   { return m_name; }

    protected:
      NameT m_name;
      IdT m_id;
    };


    template <typename _CompsTy = std::vector<std::shared_ptr<comp<>>>>
    class CComponents : public comp<> {
    public:
      typedef _CompsTy CompsT;
      typedef typename CompsT::const_iterator CompsT_cit;

      CComponents();

      CompsT* get()              const { return &_m_comps;  }
      void    set(CompsT _comps) const { _m_comps = _comps; }

    protected:
      CompsT _m_comps;
    };


    class CCompsTraits : public CComponents<> {
    public:
      bool rm(QString name) const {
        comps._m_comps.erase(find(name));
      }

      CompsT_cit find(QString name) const {
        auto it = _m_comps.begin();
        for(; it != _m_comps.end(); ++it)
          if ((*it)->get_name() == name) return it;

        return it;
      }

      void upd(QComboBox& _cmb_box_ptr) const {
        _cmb_box_ptr.clear();

        for (auto comp : comps._m_comps)
          _cmb_box_ptr.addItem(comp->get_name());

      }

    protected:
      static CCompsTraits comps;
    };


  } //PCSM::COMP


} // PCSM


namespace PCOM = PCSM::COMP;


#endif // COMPONENT_H
