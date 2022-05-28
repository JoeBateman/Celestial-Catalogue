#ifndef WINDOWS_TEMP_HEADER_H__
#define WINDOWS_TEMP_HEADER_H__
#include "csv_io.h"

// Abstract "window" class to be used as a base for the user interface
// Menu windows are window children defined in main(), and can be called from
// other windows to access the associated function.
class window{
  public:
    window(std::string window_name, bool add_to_menu)
      :name{window_name}{if(add_to_menu){
        menus.push_back(this);
      };
      };
    std::string get_name(){return name;}
    virtual void show_window()=0;
    const std::vector<window*> get_menu_windows(){return menus;}
    const bool get_running(){return running;};
    void set_running(bool is_running){running = is_running;};
    void print_menu();

  protected:
    static bool running;
    static std::vector<window*> menus;
    static data_storage* global_data;
    void get_menu(std::string);
    std::string initial_text;
    std::string name;
    virtual void get_user_input(){};
};

// Object windows are used to display celestial object data, retrived by a pointer to a vector.
// Users can print specific entry details to a .txt file, see where the object is in the night
// sky, and delete individual entries from the data (this is not reflected in data.csv unless
// the changes are saved by the user upon quitting).
class object_window : public window{
  public:
    object_window(std::string window_name, int index, bool add_to_menu)
      : window(window_name, add_to_menu), objects{global_data->get_data_vector(index)}{};
    object_window(std::string window_name, int index, std::string text, bool add_to_menu)
      : window(window_name, add_to_menu), objects{global_data->get_data_vector(index)}{initial_text=text;};
    object_window(std::string window_name, std::vector<std::shared_ptr<celestial_object>>* obj, bool add_to_menu)
      : window(window_name, add_to_menu), objects{obj}{};
    object_window(std::string window_name, std::vector<std::shared_ptr<celestial_object>>* obj, std::string text, bool add_to_menu)
      : window(window_name, add_to_menu), objects{obj}{initial_text=text;};
    //Objects window options
    void show_window();
  protected:
    std::vector<std::shared_ptr<celestial_object>>* objects;
    std::vector<std::string> object_labels{"Object","Type","Name","Apparent Magnitude", "Distance, km", "Mass, kg", "Declination, deg", "Right Ascension, hrs", "Object Specific Info"};
    void get_user_input();
    void interact_with_entry(int);
    void delete_entry(int);
    void sort_vector();
};

// Menu window type, used to hold other windows to be accessed by user.
class base_window  : public window{
  public:
    base_window(std::string name, std::vector<std::unique_ptr<object_window>>* children, data_storage &source_data, bool add_to_menu)
    : window(name,add_to_menu), child_windows{children}{
      if(global_data==nullptr){
        global_data = &source_data;
      };
    };
    base_window(std::string name, std::vector<std::unique_ptr<object_window>>* children, data_storage &source_data, std::string text, bool add_to_menu)
    : window(name, add_to_menu), child_windows{children}{
      initial_text=text;
      if(global_data==nullptr){
        global_data = &source_data;
      };
    };
    void show_window();
    void get_user_input();

  protected:
    std::vector<std::unique_ptr<object_window>>* child_windows;
};

// Window used to add objects to the data set manually, uses a template function to add an object based on user input
class add_window  : public window{
  public:
    add_window(std::string name, bool add_to_menu)
    : window(name, add_to_menu){};
    std::vector<std::string> obj_labels{"Galaxy","Nebula","Planet","Star"};
    void show_window();
    template <class obj_type> void add_object(obj_type*);
};

template <class obj_type> void add_window::add_object(obj_type* obj){
  std::cin>>*obj;
  std::shared_ptr<celestial_object> temp_object(obj);
  global_data->edit_all_data()->operator[](0).push_back(temp_object);
  for(int i {0};i<obj_labels.size(); i++){
    if(temp_object->get_object()==obj_labels[i]){
      global_data->edit_all_data()->operator[](i+1).push_back(temp_object);
    };
  }; 
}

// Window used to allow the user to save particular subsets of data, or add other data files
class csv_window  : public window{
  public:
    csv_window(std::string name, bool add_to_menu)
    : window(name, add_to_menu) {};
    std::vector<std::string> function_labels{"Add a CSV","Save to a CSV"};
    std::vector<std::string> vector_labels{"All data","Galaxies","Nebulae","Planets","Stars"};
    void show_window();
    void add_csv();
    void save_to_csv();
};

class search_window : public window{
  public:
    search_window(std::string name,  bool add_to_menu)
    : window(name, add_to_menu){};
    ~search_window(){};
    void show_window();
    void get_user_input();
  protected:
    std::vector<std::shared_ptr<celestial_object>> temp_vector;
};

// Window type used to allow the user to properly exit the program, and allows the user to save changes made to the core data file, data.csv
class quit_window  : public window{
  public:
    quit_window(std::string name, std::string text, bool add_to_menu)
    : window(name, add_to_menu){initial_text=text;};
    void show_window();
    void get_user_input();
};
#endif