#include <string>
#include <iostream>
#include "tagger.hpp"
#include "debug.hpp"

int main(int argc, char** argv)
{
  using namespace std;
  using namespace tagger;

  store_t store;

  if(argc < 2){
    store.build(".tagger");
  }else{
    string dot_tagger;
    dot_tagger.append(argv[1]);
    if('/' != *(dot_tagger.end()-1))
      dot_tagger.append("/");
    dot_tagger.append(".tagger");
    store.build(dot_tagger);
  }
  
  // test: list doc and its tag
  auto const& doc_list = store.doc_tag_map.by<doc_t>();

  for(auto i = doc_list.begin(); i != doc_list.end(); ++i)
  {
    cout<<(i->first)<<":"<<(i->second)<<"\n";
  }

  // test: find doc by tag
  auto const& tag_list = store.doc_tag_map.by<tag_t>();

  string tag;
  cout<<"Input tag to find docs: ";
  cin>>tag;
  auto range = tag_list.equal_range(tag);

  while(range.first != range.second){
    cout<<range.first->second<<","; 
    ++range.first;
  }
  cout<<"\n";
}
