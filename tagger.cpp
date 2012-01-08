#include "tagger.hpp"
#include <fstream>
#include <boost/tokenizer.hpp>
#include "debug.hpp"

#define GO_REPARSE_IF_NEXT_SECTION(Line) \
  if('[' == Line[0]){ \
    state = detail::CONF_TOP; goto REPARSE; \
  }

namespace tagger{
  namespace detail{
    enum STATE {
      CONF_TOP = 0, 
      DOC_ROOT, 
      DOC, 
      CONCEPT_REL
    };
  } //namespace detail
  
  void
  store_t::build(std::string const& conf_file)
  {
    using namespace std;
    using namespace boost;

    ifstream conf(
      conf_file.c_str(), 
      ios::binary | ios::in);

    if(!conf.is_open()){
      CERR<<"Can't find .tagger file\n";
      return;
    }

    typedef tokenizer<char_separator<char> > tok_t;

    string line;
    detail::STATE state = detail::CONF_TOP;

    while(getline(conf, line)){
      if(!conf.fail() && line[0] != '#'){
      REPARSE:
        switch(state){

          case detail::CONF_TOP:
            if(line == "[doc-root]"){
              state = detail::DOC_ROOT;
            }else if(line == "[doc]"){
              state = detail::DOC;
            }else if(line == "[concept-rel]"){
              state = detail::CONCEPT_REL;
            }
            break;

          case detail::DOC_ROOT:
            GO_REPARSE_IF_NEXT_SECTION(line);
            doc_root = line;
            break;

          case detail::DOC:
            GO_REPARSE_IF_NEXT_SECTION(line);
            {
              tok_t tok(line, char_separator<char>(" \t"));
              tok_t::iterator i = tok.begin();
              if(tok.end() != i){
                string dname = *i;
                ++i;
                if(tok.end() != i){
                  string tags = *i;
                  tok_t tag_tok(tags, char_separator<char>(","));
                  for(tok_t::iterator j = tag_tok.begin();
                      j != tag_tok.end(); ++j)
                    doc_tag_map.insert(doc_tag_t(dname, *j));
                }
              }
            }
            break;

          case detail::CONCEPT_REL:
            GO_REPARSE_IF_NEXT_SECTION(line);
            {
              tok_t tok(line, char_separator<char>(" \t"));
              tok_t::iterator i = tok.begin();
              if(tok.end() != i){
                string concept = *i;
                ++i;
                if(tok.end() != i){
                  string tags = *i;
                  tok_t tag_tok(tags, char_separator<char>(","));
                  for(tok_t::iterator j = tag_tok.begin();
                      j != tag_tok.end(); ++j)
                    concept_tag_map.insert(concept_tag_t(concept, *j));
                }
              }
            }
            break;
        } // switch(state)
      } // if('#' != line[0])
    } // whild(getline())
  }

}// namespace tagger

