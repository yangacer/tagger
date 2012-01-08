#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>
#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>
#include <map>
#include <boost/tokenizer.hpp>
#include "debug.hpp"

#define GO_REPARSE_IF_NEXT_SECTION(Line) \
  if('[' == Line[0]){ state = tagger::CONF_TOP; goto REPARSE; }

namespace tagger {
  enum STATE {
    CONF_TOP = 0, 
    DOC_ROOT, 
    DOC, 
    CONCEPT_REL
  };
}

int main(int argc, char** argv)
{
  using namespace std;
  using namespace boost;
  using namespace boost::bimaps;

  ifstream conf;

  if(argc < 2){
    conf.open(".tagger", ios::binary | ios::in);
  }else{
    string dot_tagger;
    dot_tagger.append(argv[1]);
    if('/' != *(dot_tagger.end()-1))
      dot_tagger.append("/");
    dot_tagger.append(".tagger");
    conf.open(dot_tagger.c_str(), ios::binary | ios::in);
  }

  if(!conf.is_open()){
    CERR<<"Can't find .tagger file\n";
    return 1;
  }

  // configure according to .tagger
  typedef bimap<
    multiset_of<string>,
    multiset_of<string> 
    > doc_tag_map_t;
  
  typedef doc_tag_map_t::value_type doc_tag_t;
  typedef tokenizer<char_separator<char> > tok_t;

  typedef multimap<string, string> concept_tag_map_t;
  typedef concept_tag_map_t::value_type concept_tag_t;

  string line;
  string doc_root;
  doc_tag_map_t doc_tag_map;
  concept_tag_map_t concept_tag_map;

  tagger::STATE state = tagger::CONF_TOP;

  while(getline(conf, line)){
    if(!conf.fail() && line[0] != '#'){
      REPARSE:
      switch(state){

        case tagger::CONF_TOP:
          if(line == "[doc-root]"){
            state = tagger::DOC_ROOT;
          }else if(line == "[doc]"){
            state = tagger::DOC;
          }else if(line == "[concept-rel]"){
            state = tagger::CONCEPT_REL;
          }
          break;

        case tagger::DOC_ROOT:
          GO_REPARSE_IF_NEXT_SECTION(line);
          doc_root = line;
          break;

        case tagger::DOC:
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

        case tagger::CONCEPT_REL:
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
      }
    }
  }
  
  // list doc and its tag
  for(doc_tag_map_t::left_iterator i = doc_tag_map.left.begin();
    i != doc_tag_map.left.end(); ++i)
  {
    cout<<(i->first)<<":"<<(i->second)<<"\n";
  }

  // test: find doc by tag
  string tag;
  cout<<"Input tag to find docs: ";
  cin>>tag;
  doc_tag_map_t::right_range_type range = 
    doc_tag_map.right.equal_range(tag);

  while(range.first != range.second){
    cout<<range.first->second<<","; 
    ++range.first;
  }
  cout<<"\n";
}
