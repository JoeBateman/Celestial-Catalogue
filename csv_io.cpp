#include"csv_io.h"

const bool object_sorting(std::shared_ptr<celestial_object> i, std::shared_ptr<celestial_object> j){
    int order{i->get_object().compare((j->get_object()))};
    bool i_before_j;
    if(order < 0){i_before_j=true;}
    else{i_before_j=false;};
    return i_before_j;
}; 

const bool type_sorting(std::shared_ptr<celestial_object> i, std::shared_ptr<celestial_object> j){
    int order{i->get_type().compare(j->get_type())};
    bool i_before_j;
    if(order < 0){i_before_j=true;}
    else{i_before_j=false;};
    return i_before_j;
}; 

const bool name_sorting(std::shared_ptr<celestial_object> i, std::shared_ptr<celestial_object> j){
    int order{i->get_name().compare(j->get_name())};
    bool i_before_j;
    if(order < 0){i_before_j=true;}
    else{i_before_j=false;};
    return i_before_j;
}; 

const bool app_magnitude_sorting(std::shared_ptr<celestial_object> i, std::shared_ptr<celestial_object> j){
    return (i->get_apparent_mag() < j->get_apparent_mag());
};

const bool distance_sorting(std::shared_ptr<celestial_object> i, std::shared_ptr<celestial_object> j){
    return (i->get_distance() < j->get_distance());
};

const bool mass_sorting(std::shared_ptr<celestial_object> i, std::shared_ptr<celestial_object> j){
    return (i->get_mass() < j->get_mass());
};

const bool declination_sorting(std::shared_ptr<celestial_object> i, std::shared_ptr<celestial_object> j){
    return (i->get_dec() < j->get_dec());
};

const bool ra_sorting(std::shared_ptr<celestial_object> i, std::shared_ptr<celestial_object> j){
    return (i->get_ra() < j->get_ra());
};

const void sort_object_vector(std::vector<std::shared_ptr<celestial_object>>& to_sort, std::string sort_by){
  bool success{false};
  
  typedef const bool (*func_ptr)(std::shared_ptr<celestial_object>, std::shared_ptr<celestial_object>);
  std::map<std::string, func_ptr> sorting_dictionary = {
    {"Object", object_sorting},
    {"Type", type_sorting},
    {"Name", name_sorting},
    {"Apparent Magnitude", app_magnitude_sorting},
    {"Distance, km", distance_sorting},
    {"Mass, kg", mass_sorting},
    {"Declination, deg", declination_sorting},
    {"Right Ascension, hrs", ra_sorting}
  };
  
  for(auto& it  : sorting_dictionary)
    if(it.first==sort_by){
      std::sort(to_sort.begin(),to_sort.end(), it.second);
      success = true;
    }
  if(!success) std::cout<<"Error: did not recognise sorting option. Returning unsorted vector...\n";
}

data_storage::data_storage(){
  /*
  if(!shared_data){
    for(auto& i: grouped_data[0]){
      delete i;
    };
  };
  */
}

data_storage::data_storage(std::string csv_file){
  try{
    std::cout<<"Opening "<<csv_file<<".csv...\n";
    int line_counter{1};
    std::string line;
    std::vector<std::shared_ptr<celestial_object>> data;
    std::vector<std::shared_ptr<celestial_object>> galaxies{};
    std::vector<std::shared_ptr<celestial_object>> nebulae{};
    std::vector<std::shared_ptr<celestial_object>> planets{};
    std::vector<std::shared_ptr<celestial_object>> stars{};
    std::fstream object_stream{(csv_file+".csv").c_str()};
    
    if(!object_stream.good()){
          std::string error_message{"Error: "+csv_file+" could not be opened.\n"};
          throw std::runtime_error(error_message);}
    while(std::getline(object_stream, line)){ 
      std::size_t found = line.find("%");
      if(found!= std::string::npos){line_counter++;continue;}
      int position;
      std::string delim{","};
      std::vector<std::string> words;
      while((position=line.find(delim)) != std::string::npos){
        words.push_back(line.substr(0,position));
        line.std::string::erase(0, position+delim.std::string::length());
      };
      words.push_back(line);
      try{
        std::string object{words[0]};
        std::string sub_object{words[1]};
        std::string name{words[2]};
        double apparent_magnitude{std::stod(words[3])};
        double distance{std::stod(words[4])};
        double mass{std::stod(words[5])};
        double declination{std::stod(words[6])};
        double right_ascension{std::stod(words[7])};
        std::string additional_information{words[8]};
        if(object=="Planet"){
          std::shared_ptr<celestial_object> temp {new planet(sub_object, name, apparent_magnitude, distance, mass, declination, right_ascension, additional_information, std::stoi(words[9]), std::stoi(words[10]))};
          data.push_back(temp);
          planets.push_back(temp);
        }
        else if(object=="Galaxy"){
          std::shared_ptr<celestial_object> temp {new galaxy(sub_object, name, apparent_magnitude, distance, mass, declination, right_ascension, additional_information, words[9], std::stod(words[10]), std::stoi(words[11]))};
          data.push_back(temp);
          galaxies.push_back(temp);
        }
        else if(object=="Nebula"){
          std::shared_ptr<celestial_object> temp {new nebula(sub_object, name, apparent_magnitude, distance, mass, declination, right_ascension, additional_information)};
          data.push_back(temp);
          nebulae.push_back(temp);
        }
        else if(object=="Star"){
          std::shared_ptr<celestial_object> temp {new star(sub_object, name, apparent_magnitude, distance, mass, declination, right_ascension, additional_information, std::stod(words[9]), words[10])};
          data.push_back(temp);
          stars.push_back(temp);
        }
        else{std::cout<<"Unable to determine object on line "<<line_counter<<std::endl;};
      }
      catch(...){std::cerr<<"Error. Line "<<line_counter<<" was unable to be read. Please check it is correctly formatted.\n";};
      line_counter++;
      };
    object_stream.close();
    grouped_data.push_back(data);
    grouped_data.push_back(galaxies);
    grouped_data.push_back(nebulae);
    grouped_data.push_back(planets);
    grouped_data.push_back(stars);
    sort_object_vector(grouped_data[0], "Object");
    }
    catch(std::runtime_error error){std::cout<<error.what()<<"\nBuilding a default object\n";
    grouped_data.clear();
    grouped_data.resize(5);
    for(auto& it: grouped_data){
      it.resize(0);
    }
  };
}

data_storage::data_storage(std::vector<std::shared_ptr<celestial_object>> vector){
    grouped_data.push_back(vector);
    std::vector<std::shared_ptr<celestial_object>> galaxies{};
    std::vector<std::shared_ptr<celestial_object>> nebulae{};
    std::vector<std::shared_ptr<celestial_object>> planets{};
    std::vector<std::shared_ptr<celestial_object>> stars{};
    
    if(!grouped_data[0].empty()){
      sort_object_vector(grouped_data[0], "Object");
      int i{0};
      int j{0};

      for(auto& it: vector){
        if(i==0){i++; continue;}
        if(it->get_object()==vector[i-1]->get_object()){i++; continue;}
        else{
          if(vector[i-1]->get_object()=="Galaxy"){galaxies.insert(galaxies.end(), vector.begin()+j, vector.begin()+i);j=i;i++;continue;}
          else if(vector[i-1]->get_object()=="Nebula"){nebulae.insert(nebulae.end(), vector.begin()+j, vector.begin()+i);j=i;i++;continue;}
          else if(vector[i-1]->get_object()=="Planet"){planets.insert(planets.end(), vector.begin()+j, vector.begin()+i);j=i;i++;continue;}
          else if(vector[i-1]->get_object()=="Star"){stars.insert(stars.end(), vector.begin()+j, vector.begin()+i);j=i;i++;continue;}
          std::cout<<"Error: Unrecognised object. Returning ungrouped vector...\n";
        };
        i++;
      };
      if(vector[j]->get_object()=="Galaxy"){galaxies.insert(galaxies.end(), vector.begin()+j, vector.end());}
      else if(vector[j]->get_object()=="Nebula"){nebulae.insert(nebulae.end(), vector.begin()+j, vector.end());}
      else if(vector[j]->get_object()=="Planet"){planets.insert(planets.end(), vector.begin()+j, vector.end());}
      else if(vector[j]->get_object()=="Star"){stars.insert(stars.end(), vector.begin()+j, vector.end());};
    }
    grouped_data.push_back(galaxies);
    grouped_data.push_back(nebulae);
    grouped_data.push_back(planets);
    grouped_data.push_back(stars);
}

data_storage::~data_storage(){
  for(auto& i: grouped_data){
    for(auto& j: i){
      j.reset();
    };
  };
}

void data_storage::operator+=(data_storage &to_add){
  for(int i{}; i<grouped_data.size();i++){
    grouped_data[i].insert(grouped_data[i].end(), to_add.grouped_data[i].begin(),to_add.grouped_data[i].end());
  };
};

const void data_storage::save_data_to_csv(std::string filename, int i){
  std::ofstream  new_file;
  bool file_exists{true};
  int num{1};
  std::string name = filename;
  while(file_exists){
      if(std::filesystem::exists(name+".csv")){
      name = filename + "("+std::to_string(num)+")";
      num++;
      }
      else{file_exists=false;};
  }
  new_file.open(name+".csv");
  if(!new_file.good()){
    new_file.close(); 
    std::string error_message{"Error: "+name+".csv could not be saved.\n"};
    throw std::runtime_error(error_message);
  };
  new_file<<"%Object,Sub-Object,Name,Magnitude,Distance,Mass,Declination,Right Ascension,";
  new_file<<"Additional information,Object Specific Traits,"<<std::endl;
  for(auto&it : grouped_data[i]){new_file<<*it<<std::endl;};
  new_file.close();
}


