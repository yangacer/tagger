#ifndef TAGGER_HPP_
#define TAGGER_HPP_

#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>
#include <string>
#include <map>

namespace tagger{
  namespace bm = boost::bimaps;
  
  struct tag_t{};
  struct doc_t{};

  typedef bm::bimap<
    bm::multiset_of< bm::tagged<std::string, doc_t> >,
    bm::multiset_of< bm::tagged<std::string, tag_t> >
    > doc_tag_map_t;

  typedef doc_tag_map_t::value_type doc_tag_t;

  typedef std::multimap<std::string, std::string> 
  concept_tag_map_t;
  
  typedef concept_tag_map_t::value_type concept_tag_t;

  struct store_t
  {
    void build(std::string const& conf_file);
    
    std::string doc_root;
    doc_tag_map_t doc_tag_map;
    concept_tag_map_t concept_tag_map;
  };  
}

#endif // header guard
