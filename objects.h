#ifndef OBJECT_HEADER_H__
#define OBJECT_HEADER_H__
#include<algorithm>
#include<boost/algorithm/string.hpp>
#include<cmath>
#include<filesystem>
#include<fstream>
#include<iomanip>
#include<iostream>
#include<map>
#include<sstream>
#include<string>
#include<vector>

// Abstract base class to classify 4 main types of object in the sky, galaxies, nebula, planets & stars.
class celestial_object{
    friend std::istream & operator>>(std::istream &is, celestial_object &obj);
    friend std::ostream & operator<<(std::ostream &os, celestial_object &obj);
    public:
        virtual ~celestial_object(){};
        const void show_location();
        void print_to_txt();
        // Virtual get functions, as data pointers are only defined in child classes
        const virtual std::string get_object()=0;
        const virtual std::string get_name()=0;
        const virtual std::string get_type()=0;
        const virtual double get_dec()=0;
        const virtual double get_ra()=0;
        const virtual double get_apparent_mag()=0;
        const virtual double get_distance()=0;
        const virtual double get_mass()=0;
        const virtual std::string get_fact()=0;
        // Returning a vector of object info for display as part of a list. Includes virtual function to call to 
        // return object-specific details, eg number of moons
        const virtual std::vector<std::string> get_all_info();
        const virtual std::vector<std::string> get_specific_info()=0;
        // ostream print of objects data for use in-app and printing to .txt files. Includes virtual function to call to 
        // return object-specific details, eg number of moons
        const void print_info();
        const virtual void print_specific_data()=0;

        virtual void set_name(std::string)=0;
        virtual void set_type(std::string)=0;
        virtual void set_dec(double)=0;
        virtual void set_ra(double)=0;
        virtual void set_apparent_mag(double)=0;
        virtual void set_distance(double)=0;
        virtual void set_mass(double)=0;
        virtual void set_object_fact(std::string)=0;
        virtual void show_specific_info(std::ostream &os)=0;
        virtual void set_specific_info(std::istream &is)=0;

    protected:
        // Data class framework, containing properties associated with all observed objects
        class celestial_data{
            public:
                celestial_data();
                celestial_data(std::string name, double app_mag, double dist, double M, double dec, double ra, std::string information);
                celestial_data(celestial_data const &d);
                virtual ~celestial_data()=0;
                std::string object;
                std::string name;
                double apparent_magnitude;
                double distance;
                double mass;
                double declination;
                double right_ascension;
                std::string object_fact;
        };
};
 
class planet    :   public celestial_object{
    public:
        planet(){data = new planet_data();data->planet_type = "n/a";};
        planet(std::string type, std::string name, double app_mag, double dist, double M, double dec, double ra, std::string information, int number_moons, int number_rings){
        data = new planet_data(type,name, app_mag, dist,M, dec, ra, information, number_moons, number_rings);
        };
        ~planet(){delete data;};
        const std::string get_name(){return data->name;};
        const std::string get_object(){return data->object;};
        const std::string get_type(){return data->planet_type;};
        const double get_dec(){return data->declination;};
        const double get_ra(){return data->right_ascension;};
        const double get_apparent_mag(){return data->apparent_magnitude;};
        const double get_distance(){return data->distance;};
        const double get_mass(){return data->mass;};
        const int get_moons(){return data->moons;};
        const int get_rings(){return data->rings;};
        const std::string get_fact(){return data->object_fact;};
        const std::vector<std::string> get_specific_info();
        void set_name(std::string name){data->name=name;};
        void set_type(std::string type){data->planet_type=type;};
        void set_dec(double dec){data->declination=dec;};
        void set_ra(double ra){data->right_ascension=ra;};
        void set_apparent_mag(double mag){data->apparent_magnitude=mag;};
        void set_distance(double distance){data->distance=distance;};
        void set_mass(double mass){data->mass=mass;};
        void set_moons(int moons){data->moons=moons;};
        void set_rings(int rings){data->rings=rings;};
        void set_object_fact(std::string fact){data->object_fact=fact;};
        void show_specific_info(std::ostream &os);
        void set_specific_info(std::istream &is);
        const void print_specific_data();
    
    protected:
        class planet_data   :   public celestial_object::celestial_data{
            public:
                planet_data()   :   celestial_data(), moons{0}, rings{0}{object = "Planet";};
                planet_data(std::string type, std::string name, double app_mag, double dist, double M, double dec, double ra, std::string information, int number_moons, int number_rings)
                    :   celestial_data(name, app_mag, dist, M, dec, ra, information), moons{number_moons}, rings{number_rings}, planet_type{type}{object = "Planet";};
                std::string planet_type;
                int moons;
                int rings;
    };
        planet_data* data;
};

class galaxy    :   public celestial_object{
    public:
        galaxy(){data = new galaxy_data();data->galaxy_type = "n/a";};
        galaxy(std::string type, std::string name, double app_mag, double dist, double M, double dec, double ra, std::string information, std::string sub_type, double mass_fraction, int sats){
        data = new galaxy_data(type, name, app_mag, dist, M, dec, ra, information, sub_type, mass_fraction, sats);};
        ~galaxy(){delete data;};
        const std::string get_name(){return data->name;};
        const std::string get_object(){return data->object;}
        const std::string get_type(){return data->galaxy_type;};
        const double get_dec(){return data->declination;};
        const double get_ra(){return data->right_ascension;};
        const double get_apparent_mag(){return data->apparent_magnitude;};
        const double get_distance(){return data->distance;};
        const double get_mass(){return data->mass;};
        const std::string get_sub_type(){return data->galaxy_sub_type;};
        const double get_stellar_M_frac(){return data->stellar_mass_fraction;};
        const int get_satellites(){return data->number_of_satellites;};
        const std::string get_fact(){return data->object_fact;};
        const std::vector<std::string> get_specific_info();

        void set_name(std::string name){data->name=name;};
        void set_type(std::string type){data->galaxy_type=type;};
        void set_dec(double dec){data->declination=dec;};
        void set_ra(double ra){data->right_ascension=ra;};
        void set_apparent_mag(double mag){data->apparent_magnitude=mag;};
        void set_distance(double distance){data->distance=distance;};
        void set_mass(double mass){data->mass=mass;};
        void set_sub_type(std::string sub_type){data->galaxy_sub_type=sub_type;};
        void set_stellar_mass_fraction(double m_frac){data->stellar_mass_fraction=m_frac;};
        void set_satellites(int sats){data->number_of_satellites=sats;};
        void set_object_fact(std::string fact){data->object_fact=fact;};
        void show_specific_info(std::ostream &os);
        void set_specific_info(std::istream &is);
        const void print_specific_data();
    
    protected:
        class galaxy_data   :   public celestial_object::celestial_data{
            public:
                galaxy_data()   :   celestial_data(), stellar_mass_fraction{0}, number_of_satellites{0}{object = "Galaxy";};
                galaxy_data(std::string type, std::string name, double app_mag, double dist, double M, double dec, double ra, std::string information, std::string sub_type, double mass_fraction, int sats)
                    :   celestial_data(name, app_mag, dist, M, dec, ra, information), stellar_mass_fraction{mass_fraction}, number_of_satellites{sats}, galaxy_type{type}, galaxy_sub_type{sub_type}{object = "Galaxy";};
                std::string galaxy_type;
                std::string galaxy_sub_type;
                double stellar_mass_fraction;
                int number_of_satellites;
    };
        galaxy_data* data;
};

class nebula    :   public celestial_object{
    public:
        nebula(){data = new nebula_data();data->nebula_type = "n/a";};
        nebula(std::string type, std::string name, double app_mag, double dist, double M, double dec, double ra, std::string information){
        data = new nebula_data(type, name, app_mag, dist, M, dec, ra, information);};
        ~nebula(){delete data;};
        const std::string get_name(){return data->name;};
        const std::string get_object(){return data->object;};
        const std::string get_type(){return data->nebula_type;};
        const double get_dec(){return data->declination;};
        const double get_ra(){return data->right_ascension;};
        const double get_apparent_mag(){return data->apparent_magnitude;};
        const double get_distance(){return data->distance;};
        const double get_mass(){return data->mass;};
        const std::string get_fact(){return data->object_fact;};
        const std::vector<std::string> get_specific_info(){std::vector<std::string> additional_info(0); return additional_info;};
    
        void set_name(std::string name){data->name=name;};
        void set_type(std::string type){data->nebula_type=type;};
        void set_dec(double dec){data->declination=dec;};
        void set_ra(double ra){data->right_ascension=ra;};
        void set_apparent_mag(double mag){data->apparent_magnitude=mag;};
        void set_distance(double distance){data->distance=distance;};
        void set_mass(double mass){data->mass=mass;};
        void set_object_fact(std::string fact){data->object_fact=fact;};
        void show_specific_info(std::ostream &os){};
        void set_specific_info(std::istream &is){};
        const void print_specific_data();

    protected:
        class nebula_data   :   public celestial_object::celestial_data{
            public:
                nebula_data()   :   celestial_data(){object = "Nebula";};
                nebula_data(std::string type, std::string name, double app_mag, double dist, double M, double dec, double ra, std::string information)
                    :   celestial_data(name, app_mag, dist, M, dec, ra, information), nebula_type{type}{object = "Nebula";};
                std::string nebula_type;
    };
        nebula_data* data;
};

class star    :   public celestial_object{
    public:
        star(){data = new star_data();data->star_type = "n/a";};
        star(std::string type, std::string name, double app_mag, double dist, double M, double dec, double ra, std::string information, double lum, std::string s_class){
        data = new star_data(type, name, app_mag, dist, M, dec, ra, information, lum, s_class);};
        ~star(){delete data;};
        const std::string get_name(){return data->name;};
        const std::string get_object(){return data->object;};
        const std::string get_type(){return data->star_type;};
        const double get_dec(){return data->declination;};
        const double get_ra(){return data->right_ascension;};
        const double get_apparent_mag(){return data->apparent_magnitude;};
        const double get_distance(){return data->distance;};
        const double get_mass(){return data->mass;};
        const double get_luminosity(){return data->luminosity;};
        const std::string get_spectral_class(){return data->spectral_class;};
        const std::string get_fact(){return data->object_fact;};
        const std::vector<std::string> get_specific_info();
    
        void set_name(std::string name){data->name=name;};
        void set_type(std::string type){data->star_type=type;};
        void set_dec(double dec){data->declination=dec;};
        void set_ra(double ra){data->right_ascension=ra;};
        void set_apparent_mag(double mag){data->apparent_magnitude=mag;};
        void set_distance(double distance){data->distance=distance;};
        void set_mass(double mass){data->mass=mass;};
        void set_luminosity(double L){data->luminosity=L;};
        void set_spectral_class(std::string s_class){data->spectral_class=s_class;};
        void set_object_fact(std::string fact){data->object_fact=fact;};
        void show_specific_info(std::ostream &os);
        void set_specific_info(std::istream &is);
        const void print_specific_data();

    protected:
        class star_data   :   public celestial_object::celestial_data{
            public:
                star_data()   :   celestial_data(){object = "Star";};
                star_data(std::string type, std::string name, double app_mag, double dist, double M, double dec, double ra, std::string information, double lum, std::string s_class)
                    :   celestial_data(name, app_mag, dist, M, dec, ra, information), star_type{type}, luminosity{lum}, spectral_class{s_class} {object = "Star";};
                std::string star_type;
                double luminosity;
                std::string spectral_class;
    };
        star_data* data;
};
#endif