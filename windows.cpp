#include"windows.h"

void clear_terminal(){
#ifdef _WIN32
    system("cls");
#elif linux
    system("clear");
#elif __APPLE__
    system("clear");
#endif
}

data_storage* window::global_data{nullptr};

std::vector<window*> window::menus{};

bool window::running{true};

void window::print_menu(){
  std::cout<<"Menu commands are: ";
  for(auto& it: menus){
    std::cout<<it->get_name()<<", ";
  };
}

void window::get_menu(std::string input){
  bool good_input{false};
  while(!good_input){
    for(int i{};i<menus.size();i++){
      if(input==menus[i]->name){good_input = true; menus[i]->show_window();}
      };
    if(!good_input){
    std::cout<<"That menu command was not understood. Please try again.\n";
    std::cin.clear();
    std::cin>>input;
    };
  };
}

void object_window::show_window(){
   //clear_terminal();
  print_menu();
  std::cout<<std::endl<<initial_text;
  std::cout<<std::endl;
  std::cout<<std::setw(5)<< std::right<<"|";
  for(auto& it: object_labels){
    std::cout<<std::setw(22) << std::right <<it<<"|";
  }
  std::cout<<std::endl;
  if(objects->empty()){std::cout<<"This selection is currently empty.\n";}
  else{int num{1};
    for(auto& i:  *objects){
      if(i!=nullptr){
        std::cout<<std::setw(3)<< std::right <<num<<".|";
        for(auto& j: i->get_all_info()){
          std::cout<<std::setw(22) << std::right <<j<<"|";
        };
        std::cout<<std::endl;
        num++;
      };
    };
    std::cout<<std::setw(3)<< std::right <<num<<". Sort this selection.\n";
    std::cout<<std::setw(3)<< std::right <<num+1<<". Save this selection to a new CSV file.\n";
  };
  get_user_input();
}

void object_window::get_user_input(){
  std::string input;
  bool good_input{false};
  while(!good_input){
    std::cout<<"Enter the associated entry number for additional details, or use the menu commands to go back. "<<std::endl;
    input.clear();
    std::cin>>input;
    bool is_int{input.find_first_not_of("0123456789") == std::string::npos};
    if(is_int){
      int num = std::stoi(input);
      num--;
      if(num<objects->size()&&num>=0){
        interact_with_entry(num);
        good_input=true;
      }
      else if(num==objects->size()&&num!=0){
        sort_vector();
        good_input=true;
      }
      else if(num==objects->size()+1&&num!=1){
        try{
          data_storage temp(*objects);
          temp.save_data_to_csv(name, 0);

          
        }
        catch(std::runtime_error){std::cerr<<"Unable to save this file."<<std::endl;}
        good_input=true;
        show_window(); 
      }
      else{std::cout<<"Sorry, that number wasn't within the range of entries.\nPlease re-input your command.";};
    }
    else if(input[0]=='!'){
      good_input=true;
      get_menu(input);
    }
    else std::cout<<"That input was not understood.\n";
  };
}

void object_window::interact_with_entry(int entry_num){
  std::cout<<"What would you like to do with this data?\n1. See entry details.\n2. Delete this entry.\n";
  std::string input;
  bool good_input{false};
  while(!good_input){
    std::cout<<"Enter an option number.\n";
    input.clear();
    std::cin>>input;
    bool is_int{input.find_first_not_of("0123456789") == std::string::npos};
    if(is_int){
      int num = std::stoi(input);
      num--;
      if(num==0){
        objects->operator[](entry_num)->print_to_txt();
        good_input=true;
      }
      else if(num==1){
        delete_entry(entry_num);
        good_input=true;
      }
      else{std::cout<<"Sorry, that number wasn't within the range of options.\nPlease re-input your command.\n";continue;};
      show_window();
    }
    else std::cout<<"That input was not understood.\n";
  };
};

// Resets and erases the celestial_object shared pointer in global_data[0], global_data[object_type], and (*objects), deleting the underlying data befeorehand
// to prevent memory leak.
void object_window::delete_entry(int entry_num){
  std::cout<<"Are you sure you want to delete "<<objects->operator[](entry_num)->get_name()<<"? (y/n)\n";
  std::string input;
  bool good_input{false};
  while(!good_input){
    std::cin.clear();
    input.clear();
    std::cin>>input;
    if(input=="y"){
      good_input=true;
      
      std::string name{objects->operator[](entry_num)->get_name()};
      double distance{objects->operator[](entry_num)->get_distance()};
      std::string object{objects->operator[](entry_num)->get_object()};
      std::vector<std::string> object_names{"Galaxy", "Nebula", "Planet", "Star"};
      int data_index;
      for(int i{1}; i<=object_names.size(); i++){if(object==object_names[i-1]){data_index=i;};};
      
      std::vector<std::vector<std::shared_ptr<celestial_object>>>* all_data = global_data->edit_all_data();
      auto found_all = std::find_if(all_data->operator[](0).begin(), all_data->operator[](0).end(), [&name](std::shared_ptr<celestial_object> obj) {return obj->get_name() == name;});
      auto found_specific = std::find_if(all_data->operator[](data_index).begin(), all_data->operator[](data_index).end(), [&name](std::shared_ptr<celestial_object> obj) {return obj->get_name() == name;});
      
      if(found_all!=all_data->operator[](0).end()&&(*found_all)->get_distance()==distance&&(*found_all)->get_object()==object&&found_specific!=all_data->operator[](data_index).end()&&
      (*found_specific)->get_distance()==distance&&(*found_specific)->get_object()==object){
        long long unsigned int obj_size{objects->size()};
        int index_all = std::distance(all_data->operator[](0).begin(), found_all);
        int index_specific = std::distance(all_data->operator[](data_index).begin(), found_specific);
        // Performing a check on whether objects points to a global data vector. If not, the entry is also erased from objects.
        if(obj_size!=all_data->operator[](0).size()&&obj_size!=all_data->operator[](data_index).size()&&entry_num!=index_all&&entry_num!=index_specific){
          (*objects)[entry_num].reset();
          objects->erase(objects->begin()+entry_num);
        };
        (*all_data)[0][index_all].reset();
        all_data->operator[](0).erase(all_data->operator[](0).begin()+index_all);
        (*all_data)[data_index][index_specific].reset();
        all_data->operator[](data_index).erase(all_data->operator[](data_index).begin()+index_specific);
      };
    }
    else if(input=="n"){good_input=true;}
    else{std::cout<<"That input was not understood. Are you sure you want to delete "<<objects->operator[](entry_num)->get_name()<<"? (y/n)\n";}
  };
}

void object_window::sort_vector(){
  std::cout<<"How would you like this data to be sorted by?\n";
  for(int i{1};i<object_labels.size();i++){
      std::cout<<std::setw(3)<< std::right <<i<<". "<<object_labels[i-1]<<std::endl;
  };

  std::string input;
  bool good_input{false};
  while(!good_input){
    std::cout<<"Please select the method by which to sort the data.\nEnter the option number to continue.\n";
    input.clear();
    std::cin>>input;
    bool is_int{input.find_first_not_of("0123456789") == std::string::npos};
    if(is_int){
      int num = std::stoi(input);
      num--;
      if(num<object_labels.size()-1&&num>=0){
        good_input=true;
        sort_object_vector(*objects, object_labels[num]);
      }
      else{std::cout<<"Sorry, that number wasn't within the range of number of objects.\nPlease re-input your command.";continue;};
    }
    else std::cout<<"That input was not understood.\n";
  };
  show_window();
}

void base_window::show_window(){
   //clear_terminal();
  std::cout<<"Menu commands are: ";
  for(auto& it: menus){
    std::cout<<it->get_name()<<",";
  };
  std::cout<<std::endl<<initial_text;
  int num{1};
  for(auto& it: *child_windows){
    std::cout<<std::setw(5)<< std::right<<num<<". "<<std::setw(8) << std::right <<it->get_name()<<std::endl;
    num++;
    }
  get_user_input();
}

void base_window::get_user_input(){
  std::string input;
  bool good_input{false};
  while(!good_input){
    std::cout<<"Enter the option number to continue, or use the menu commands."<<std::endl;
    input.clear();
    std::cin>>input;
    bool is_int{input.find_first_not_of("0123456789") == std::string::npos};
    if(is_int){
      int num = std::stoi(input);
      num--;
      if(num<(*child_windows).size()&&num>=0){
        good_input=true;
        (*child_windows)[num]->show_window();
      }
      else{std::cout<<"Sorry, that number wasn't within the range of number of objects.\nPlease re-input your command.";continue;};
    }
    else if(input[0]=='!'){
      good_input=true;
      get_menu(input);
    }
    else std::cout<<"That input was not understood.\n";
  };
}

void add_window::show_window(){
   //clear_terminal();
  print_menu();
  std::cout<<"Please select which type of object you would like to add:\n";
  std::cout<<std::endl;
  int num{1};
  for(auto& it: obj_labels){
    std::cout<<std::setw(5)<< std::right<<num<<".  "<<it<<std::endl;
    num++;
  };


  int obj_num;
  std::string input;
  bool good_input{false};
  while(!good_input){
    std::cout<<"Enter the option number to continue, or use the menu commands."<<std::endl;
    input.clear();
    std::cin>>input;
    bool is_int{input.find_first_not_of("0123456789") == std::string::npos};
    if(is_int){
      obj_num = std::stoi(input);
      obj_num--;
      std::cin.clear();
      
      std::map<int, celestial_object*> type{
        {0, new galaxy},
        {1, new nebula},
        {2, new planet},
        {3, new star}
      };
      bool success{false};
      for(auto& it: type){
        if(it.first == obj_num){
          add_object(it.second);
          success = true;
        }
      }
      if(!success){std::cout<<"Sorry, that number wasn't within range.\nPlease re-input your command.";continue;}
      good_input=true;
    }
    else if(input[0]=='!'){
      good_input=true;
      get_menu(input);
    }
    else std::cout<<"That input was not understood.\n";
  };
};

void csv_window::show_window(){
   //clear_terminal();
  print_menu();
  
  std::cout<<std::endl;
  int num{1};
  for(auto& it: function_labels){
  std::cout<<std::setw(5)<< std::right<<num<<".  "<<it<<std::endl;
  num++;}

  std::string input;
  bool good_input{false};
  while(!good_input){
    std::cout<<"Enter the option number to continue, or use the menu commands."<<std::endl;
    input.clear();
    std::cin>>input;
    bool is_int{input.find_first_not_of("0123456789") == std::string::npos};
    if(is_int){
      int num = std::stoi(input);
      num--;
      std::cin.clear();
      if(num==0){
        good_input=true;
        add_csv();}
      else if(num==1){
        good_input=true;
        save_to_csv();}
      else{std::cout<<"Sorry, that number wasn't within range.\nPlease re-input your command.";continue;};
    }
    else if(input[0]=='!'){
      good_input=true;
      get_menu(input);
    }
    else std::cout<<"That input was not understood.\n";
  };
};

void csv_window::add_csv(){
  std::vector<std::shared_ptr<celestial_object>> new_data;
  bool good_input{false};
  std::cin.ignore(1,'\n');
  while(!good_input){
    std::string csv_name;
    std::cin.clear();
    std::cout<<"Please enter the name of the csv file to add:\n";
    std::getline(std::cin, csv_name);
    if(std::filesystem::exists(csv_name+".csv")){
      data_storage new_data(csv_name);
      (*global_data)+=new_data;
      good_input=true;}
    else std::cout<<"That file does not exist. Please try again.\n";
  };
}

// Allows the user to save all the data or a specific object type to a .csv file, the name of which is defined by the user.
// The user is warned if they are going to overwrite an existing .csv file, but the core data.csv is not permitted to be 
// overwritten from here.
void csv_window::save_to_csv(){
  std::cout<<"Menu commands are: ";
  for(auto& it:menus){
    std::cout<<it->get_name()<<", ";
  };
  std::cout<<std::endl;
  int num{1};
  for(auto& it: vector_labels){
    std::cout<<std::setw(5)<< std::right<<num<<".  "<<it<<std::endl;
    num++;
  }
  std::string input;
  bool good_input{false};
  
    while(!good_input){
    std::cout<<"Enter the option number to continue, or use the menu commands.\n"<<std::endl;
    input.clear();
    std::cin>>input;
    bool is_int{input.find_first_not_of("0123456789") == std::string::npos};
    if(is_int){
      int num = std::stoi(input);
      num--;
      if(num<vector_labels.size()&&num>=0){
        good_input=true;
        bool good_name{false};
        bool data_overwrite{false};
        while(!good_name){
          std::string csv_name;
          char y_n;
          std::cout<<"Please enter the name of the csv file to save to:\n";
          if(!data_overwrite){std::cin.ignore(1,'\n');};
          std::getline(std::cin, csv_name);
          if(csv_name=="data"){std::cout<<"Sorry, the core data file cannot be overriden.\n";data_overwrite=true;continue;}
          else{
            std::cin.clear();
            std::cout<<"Are you happy with the name: "<<csv_name<<"? (y/n)";
            std::cin>>y_n;
          };
          if(y_n=='y'){good_name=true;global_data->save_data_to_csv(csv_name, num);};
          data_overwrite=false;
        }
      }
      else{std::cout<<"Sorry, that number wasn't within the range of number of objects.\nPlease re-input your command.";continue;};
    }
    else if(input[0]=='!'){
      good_input=true;
      get_menu(input);
    }
    else std::cout<<"That input was not understood.\n";
  };
}

void search_window::show_window(){
  print_menu();
  std::cout<<"Please enter a search term, or use the menu commands.\n";
  get_user_input();
}

void search_window::get_user_input(){
  std::string search_term;
  std::vector<std::shared_ptr<celestial_object>> temp_copy_all = *global_data->get_data_vector(0);
  std::vector<std::shared_ptr<celestial_object>> found_entries;
  bool good_input{false};
  while(!good_input){
  std::cin.clear();
  std::getline(std::cin,search_term);
  if(search_term.size()!=0){good_input=true;}
  else{std::cout<<"Please enter a search term, or use the menu commands.\n";};
  };

  if(search_term[0]=='!'){get_menu(search_term);}
  else{
    bool searching{true};
    while(searching){
      
      auto found_name = std::find_if(temp_copy_all.begin(), temp_copy_all.end(), [&search_term](std::shared_ptr<celestial_object> obj) {return boost::algorithm::contains(obj->get_name(), search_term);});
      auto found_type = std::find_if(temp_copy_all.begin(), temp_copy_all.end(), [&search_term](std::shared_ptr<celestial_object> obj) {return boost::algorithm::contains(obj->get_type(), search_term);});
      auto found_object = std::find_if(temp_copy_all.begin(), temp_copy_all.end(), [&search_term](std::shared_ptr<celestial_object> obj) {return boost::algorithm::contains(obj->get_object(), search_term);});

      if(found_name!=temp_copy_all.end()){
        int index = std::distance(temp_copy_all.begin(), found_name);
        found_entries.push_back(temp_copy_all[index]);
        temp_copy_all.erase(temp_copy_all.begin()+index);
      }
      else if(found_type!=temp_copy_all.end()){
        int index = std::distance(temp_copy_all.begin(), found_type);
        found_entries.push_back(temp_copy_all[index]);
        temp_copy_all.erase(temp_copy_all.begin()+index);
      }
      else if(found_object!=temp_copy_all.end()){
        int index = std::distance(temp_copy_all.begin(), found_object);
        found_entries.push_back(temp_copy_all[index]);
        temp_copy_all.erase(temp_copy_all.begin()+index);
      }
      else{searching=false;};
      };
    std::cout<<"Number of results: "<<found_entries.size()<<std::endl;
    object_window search_results("Search results "+search_term, &found_entries, "Search term: "+search_term, false);
    search_results.show_window();
  };
};    

void quit_window::show_window(){
   //clear_terminal();

  print_menu();

  std::cout<<std::endl<<initial_text;
  get_user_input();
}

void quit_window::get_user_input(){
  bool good_input{false};
  while(!good_input){
    std::cout<<"Please enter 'y' or 'n', or use the menu commands."<<std::endl;
    std::string input;
    std::cin>>input;
    if(input=="y"){
      std::ofstream  new_file{"data.csv"};
      if(!new_file.is_open()){
        new_file.close(); 
        std::cout<<"Error: "<<name<<".csv could not be saved. Please use the menu commands to return, or use !quit to try again.\n";
        print_menu();
        input.clear();
        std::cout<<std::endl;
        std::cin>>input;
        get_menu(input);
      }
      else{
      new_file<<"%Object,Sub-Object,Name,Magnitude,Distance,Mass,Declination,Right Ascension,Additional information,Object Specific Traits,"<<std::endl;
      for(auto&it : (*global_data->edit_all_data())[0]){new_file<<*it<<std::endl;};
      new_file.close();
      good_input=true;menus[0]->set_running(false);}
    }
    else if(input=="n"){good_input=true;menus[0]->set_running(false);}
    else if(input[0]=='!'){good_input=true;get_menu(input);}
    else std::cout<<"That input was not understood.\n";
    std::cin.clear();
  };
}