#include "objects.h"

celestial_object::celestial_data::celestial_data()  =   default;
celestial_object::celestial_data::celestial_data(std::string name, double app_mag, double dist, double M, double dec, double ra, std::string information)
    :   name{name}, declination{dec}, right_ascension{ra}, apparent_magnitude{app_mag}, distance{dist}, mass{M}   {
        if(!information.empty()){object_fact = information;}
        else object_fact = "No additional information given.";
    }
celestial_object::celestial_data::celestial_data(celestial_data const &d)
    :   name{d.name}, declination{d.declination}, right_ascension{d.right_ascension}, apparent_magnitude{d.apparent_magnitude}, distance{d.distance}, mass{d.mass}  {}
celestial_object::celestial_data::~celestial_data(){};

// Shows the position of the object in the night sky, using declination & right ascension defined at J2000,
// by using system() to go to a webpage showing the part of the sky of interest. The exact form of the system() string
// input is dependent on the OS of the machine.

const void celestial_object::show_location(){
    #ifdef _WIN32
    std::string url_1{"https://www.fourmilab.ch/cgi-bin/Yourtel?lat="};
    std::string url_2{"^&ns=North^&lon="};
    std::string url_3 = "'fov=30^&date=1^&utc=2000-01-01";
    std::string url = url_1 + std::to_string(get_dec()) + url_2 + std::to_string((get_ra()/24.)*360) + url_3;
    std::cout<<"Opening in default browser: "<<url<<std::endl;
    std::string start{"start "+url};
    system(start.c_str());
    #elif  __APPLE__
    std::string url_1{"https://www.fourmilab.ch/cgi-bin/Yourtel?lat="};
    std::string url_2{"'&'ns=North'&'lon="};
    std::string url_3 = "'&'fov=30'&'date=1'&'utc=2000-01-01";
    std::string url = url_1 + std::to_string(get_dec()) + url_2 + std::to_string((get_ra()/24.)*360) + url_3;
    std::cout<<"Opening in default browser: "<<url<<std::endl;
    std::string open{"open "+url};
    system(open.c_str());
    #elif linux
    std::string url_1{"https://www.fourmilab.ch/cgi-bin/Yourtel?lat="};
    std::string url_2{"'&'ns=North'&'lon="};
    std::string url_3 = "'&'fov=30'&'date=1'&'utc=2000-01-01";
    std::string url = url_1 + std::to_string(get_dec()) + url_2 + std::to_string((get_ra()/24.)*360) + url_3;
    std::cout<<"Opening in default browser: "<<url<<std::endl;
    std::string open{"xdg-open "+url};
    system(open.c_str());
    #else 
    std::string url_1{"https://www.fourmilab.ch/cgi-bin/Yourtel?lat="};
    std::string url_2{"'&'ns=North'&'lon="};
    std::string url_3 = "'&'fov=30'&'date=1'&'utc=2000-01-01";
    std::string url = url_1 + std::to_string(get_dec()) + url_2 + std::to_string((get_ra()/24.)*360) + url_3;
    std::cout<<"Unable to determine OS. Please visit "<<url<<" to see the area of sky around "<<get_name()<<".\n";
    #endif
    
}

const std::vector<std::string> celestial_object::get_all_info(){
    std::vector<std::string> info;
    info.push_back(get_object());
    info.push_back(get_type());
    info.push_back(get_name());
    std::ostringstream ap_mag;
    ap_mag<<get_apparent_mag();
    info.push_back(ap_mag.str());
    std::ostringstream distance;
    distance<<get_distance();
    info.push_back(distance.str());
    std::ostringstream mass;
    mass<<get_mass();
    info.push_back(mass.str());
    std::ostringstream declination;
    declination<<get_dec();
    info.push_back(declination.str());
    std::ostringstream right_ascension;
    right_ascension<<get_ra();
    info.push_back(right_ascension.str());
    // Calls virtual get_specific_info function to retrive extra, object specific, information
    std::vector additional_info = get_specific_info();
    info.insert(info.end(), additional_info.begin(), additional_info.end());
    return info;
}

std::istream & operator>>(std::istream &is, celestial_object &obj){
    // Calls virtual add_data function so user information can be formatted for the type of object being input
    bool good_input{false};
    bool has_looped{false};
    while(!good_input){
        if(has_looped){std::cout<<"Bad input, please try again.\n";};
        is.clear();
        double temp_double;
        int temp_int;
        std::string temp_string;
        std::cout<<"What type of object is this?\n";
        is.ignore(2,'\n');
        std::getline(is, temp_string);
        obj.set_type(temp_string);     
        temp_string = "";
        std::cout<<"What is the name of the object?\n";
        std::getline(is, temp_string);
        obj.set_name(temp_string);
        std::cout<<"What is its declination in degrees?\n";
        is>>temp_double;
        if(!is){has_looped=true; continue;};
        obj.set_dec(temp_double);
        std::cout<<"What is its right ascension in hours?\n";
        is>>temp_double;
        if(!is){has_looped=true; continue;};
        obj.set_ra(temp_double);
        std::cout<<"What is its apparent magnitude?\n";
        is>>temp_double;
        if(!is){has_looped=true; continue;};
        obj.set_apparent_mag(temp_double);
        std::cout<<"What is its distance from earth in km?\n";
        is>>temp_double;
        if(!is){has_looped=true; continue;};
        obj.set_distance(temp_double);
        std::cout<<"What is its mass in kg?\n";
        is>>temp_double;
        if(!is){has_looped=true; continue;};
        obj.set_mass(temp_double);
        obj.set_specific_info(is);
        std::cout<<"Any additional information/facts? (y/n)\n";
        bool good_y_n{false};
        char y_n;
        while(!good_y_n){
            is.clear();
            is.ignore(2,'\n');
            is>>y_n;
            if(y_n=='y'){good_y_n=true; 
                do{
                    std::cout<<"Please enter additional information (note that commas cannot be used).";
                    is.ignore(2,'\n');
                    temp_string = "";
                    std::getline(is, temp_string);
                }
                while(temp_string.find(',') != std::string::npos);
                obj.set_object_fact(temp_string);
            } 
            else if(y_n=='n'){good_y_n=true;}
        } 
        std::cout<<"Summarising data: \n";
        obj.print_info();
        good_y_n = false;
        std::cout<<"Are you happy with this data? (y/n)\n";
        while(!good_y_n){
            is.clear();
            is.ignore(2,'\n');
            is>>y_n;
            if(y_n=='y'){good_y_n=true; good_input=true;}
            else if(y_n=='n'){good_y_n=true;has_looped=false;}
        };
    }
    return is;}

std::ostream & operator<<(std::ostream &os, celestial_object &obj){
    os<<obj.get_object()<<","<<obj.get_type()<<","<<obj.get_name()<<","<<obj.get_apparent_mag()<<","<<obj.get_distance()<<","<<obj.get_mass()<<","<<obj.get_dec()<<","<<obj.get_ra()<<",";
    if(obj.get_fact()!="No additional information given."){os<<obj.get_fact()<<",";}
    else os<<",";
    obj.show_specific_info(os);
    
    return os;
}

const void celestial_object::print_info(){
    std::cout<<this->get_object()<<" name: "<<get_name()<<std::endl;
    std::cout<<this->get_object()<<" Type: "<<get_type()<<std::endl;
    std::cout<<this->get_object()<<" mass: "<<get_mass()<<" kg\n";
    std::cout<<"Apparent Magnitude: "<<get_apparent_mag()<<std::endl;
    std::cout<<"Distance from Earth: "<<get_distance()<<" km\n";
    print_specific_data();
    std::cout<<"RA: "<<get_ra()<<" Dec: "<<get_dec()<<std::endl;
    std::cout<<"Additional information: "<<get_fact()<<std::endl;
}

void celestial_object::print_to_txt(){
    print_info();
    std::cout<<"Would you like to view this object on a star chart? (y/n)\n";
    char y_n;
    bool good_y_n{false};
    while(!good_y_n){
        std::cin.clear();
        std::cin>>y_n;
        if(y_n=='y'){good_y_n=true;
            show_location();
        }
        else if(y_n=='n'){good_y_n=true;}
    };
    
    std::cout<<"Would you like to save this data to a txt file? (y/n)\n";
    good_y_n = false;
    while(!good_y_n){
        std::cin.clear();
        std::cin>>y_n;
        if(y_n=='y'){good_y_n=true;
            // Creating a file to save to, and overriding stream buffer of cout to insead print to txt,
            // then returning cout to normal
            bool file_exists{true};
            int num{0};
            std::string name = get_name();
            while(file_exists){
                if(std::filesystem::exists(get_name()+".csv")){
                name = get_name() + "("+std::to_string(num)+")";
                num++;
                }
                else{file_exists=false;};
            }
            std::ofstream new_file(name+".txt");
            std::streambuf *coutbuf = std::cout.rdbuf();
            std::cout.rdbuf(new_file.rdbuf());
            print_info();
            std::cout.rdbuf(coutbuf);
            new_file.close();
        }
        else if(y_n=='n'){good_y_n=true;}
    };
}

const void planet::print_specific_data(){
    std::cout<<"Number of Moons: "<<get_moons()<<std::endl;
    std::cout<<"Number of Moons: "<<get_rings()<<std::endl;
};

const std::vector<std::string> planet::get_specific_info(){
    std::vector<std::string> additional_info;
    additional_info.push_back("Moons: "+std::to_string(get_moons()));
    additional_info.push_back("Rings: "+std::to_string(get_rings()));
    return additional_info;
}

void planet::set_specific_info(std::istream &is){
    int temp_int;
    std::cout<<"How many moons does this object have?\n";
    is>>temp_int;
    if(!is.good()){std::cout<<"Bad input.";return;};
    this->set_moons(temp_int);
    std::cout<<"How many rings does this object have?\n";
    is>>temp_int;
    if(!is.good()){std::cout<<"Bad input.";return;};
    this->set_rings(temp_int);
}

void planet::show_specific_info(std::ostream &os){
    os<<get_moons()<<","<<get_rings();
}

const std::vector<std::string> galaxy::get_specific_info(){
    std::vector<std::string> additional_info;
    additional_info.push_back("Galaxy Sub-Type: "+get_sub_type());
    std::ostringstream stringstream;
    stringstream.clear();
    stringstream<<get_stellar_M_frac();
    additional_info.push_back("Stellar Mass %: "+ stringstream.str());
    additional_info.push_back("Satellites: "+std::to_string(get_satellites()));
    return additional_info;
};

void galaxy::set_specific_info(std::istream &is){
    int temp_int;
    std::string temp_string;
    double temp_double;

    std::cout<<"What galaxy sub type is this?\n";
    is.ignore(2,'\n');
    std::getline(is, temp_string);
    this->set_sub_type(temp_string);     
    temp_string = "";
    std::cout<<"What is the stellar mass fraction of this galaxy?\n";
    is>>temp_double;
    if(!is.good()){std::cout<<"Bad input.";return;};
    this->set_stellar_mass_fraction(temp_double);
    std::cout<<"How many satellite galaxies does this galaxy have?\n";
    is>>temp_int;
    if(!is.good()){std::cout<<"Bad input.";return;};
    this->set_satellites(temp_int);
}

void galaxy::show_specific_info(std::ostream &os){
    os<<get_sub_type()<<","<<get_stellar_M_frac()<<","<<get_satellites();
}

const void galaxy::print_specific_data(){
    std::cout<<"Galaxy sub-type: "<<get_sub_type()<<std::endl;
    std::cout<<"Stellar mass fraction: "<<get_stellar_M_frac()<<std::endl;
    std::cout<<"Number of satellite galaxies: "<<get_satellites()<<std::endl;
};

const void nebula::print_specific_data(){
};

const std::vector<std::string> star::get_specific_info(){
    std::vector<std::string> additional_info;
    std::ostringstream luminosity;
    luminosity<<get_luminosity();
    additional_info.push_back("Luminosity: "+luminosity.str());
    additional_info.push_back("Spectral Class: "+get_spectral_class());
    return additional_info;
};

void star::set_specific_info(std::istream &is){
    int temp_int;
    std::string temp_string;
    double temp_double;
    std::cout<<"What is this star's spectral class?\n";
    is.ignore(2,'\n');
    std::getline(is, temp_string);
    this->set_spectral_class(temp_string);     
    temp_string = "";
    std::cout<<"What is the luminosity of this star?\n";
    is>>temp_double;
    if(!is.good()){std::cout<<"Bad input.";return;};
    this->set_luminosity(temp_double);
}

void star::show_specific_info(std::ostream &os){
    os<<get_luminosity()<<","<<get_spectral_class();
}

const void star::print_specific_data(){
    std::cout<<"Luminosity: "<<get_luminosity()<<std::endl;
    std::cout<<"Spectral Class: "<<get_spectral_class()<<std::endl;
    std::cout<<"Distance from Earth: "<<get_distance()<<" km\n";
};