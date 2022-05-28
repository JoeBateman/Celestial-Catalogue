#ifndef CSV_HEADER_H__
#define CSV_HEADER_H__
#include "objects.h"

const bool object_sorting(std::shared_ptr<celestial_object> i, std::shared_ptr<celestial_object> j);

const bool type_sorting(std::shared_ptr<celestial_object> i, std::shared_ptr<celestial_object> j);

const bool name_sorting(std::shared_ptr<celestial_object> i, std::shared_ptr<celestial_object> j);

const bool app_magnitude_sorting(std::shared_ptr<celestial_object> i, std::shared_ptr<celestial_object> j);

const bool distance_sorting(std::shared_ptr<celestial_object> i, std::shared_ptr<celestial_object> j);

const bool mass_sorting(std::shared_ptr<celestial_object> i, std::shared_ptr<celestial_object> j);

const bool declination_sorting(std::shared_ptr<celestial_object> i, std::shared_ptr<celestial_object> j);

const bool ra_sorting(std::shared_ptr<celestial_object> i, std::shared_ptr<celestial_object> j);

const void sort_object_vector(std::vector<std::shared_ptr<celestial_object>>& to_sort, std::string sort_by);

class data_storage{
  public:
    data_storage();
    data_storage(std::string csv_file);
    data_storage(std::vector<std::shared_ptr<celestial_object>>);
    ~data_storage();
    const void save_data_to_csv(std::string filename, int index);
    void operator+=(data_storage &);
    std::vector<std::shared_ptr<celestial_object>>* get_data_vector(int index){return &grouped_data[index];};
    std::vector<std::vector<std::shared_ptr<celestial_object>>>* edit_all_data(){return &grouped_data;}
  protected:
    std::vector<std::vector<std::shared_ptr<celestial_object>>> grouped_data;
};
#endif