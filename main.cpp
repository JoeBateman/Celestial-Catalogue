#include "windows.h"

int main(){
  data_storage default_data("data");

  std::vector<std::string> group_names{"All Data","Galaxies","Nebulae","Planets","Stars"};
  std::vector<std::unique_ptr<object_window>> main_window(group_names.size());

  quit_window quit("!quit", "Would you like to save the changes made? (y/n)\n", true);
  search_window search("!search", true);
  add_window add_an_object("!add", true);
  csv_window io_csv("!csv", true);
  

  // Final "true" arguement denotes that this window will be included in the menu bar
  base_window main_menu("!main", &main_window, default_data, true);
  for(int i{0}; i<group_names.size(); i++){
    // Object windows have a false menu arguement, but are put into the main_window vector.
    // All windows in this array will be able to be opened when main_menu.show_window() is called.
    std::unique_ptr<object_window> temp_ptr(new  object_window(group_names[i], i, false));
    main_window[i]=std::move(temp_ptr);
  }

  while(main_menu.get_running()){
  main_menu.show_window();
  };
  return 0;
}