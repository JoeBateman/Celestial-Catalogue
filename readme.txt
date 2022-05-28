Additional libraries:
This code makes use of the boost/algorithm library, in particular boost/algorithm/string.hpp, which may need to be installed to successfully compile.


Formatting CSV Files for data entry:

Note: 
	- special characters may not be interpetted correctly, and that commas should not be used in strings due to the nature of a comma separated file
	- additional info is optional, and the cell can safely be left empty

By convention, celestial coordinates RA & Dec are defined from the J2000 epoch.

PLANETS
str	str	     str  double	     double   double double      double          str(optional)   int   int
Planet,*Planet type*,name,apparent magnitude,distance,mass  ,declination,right ascension,additional info,moons,rings

GALAXIES
str	str	     str  double	     double   double double      double          str		 str 		 double		       Int
Galaxy,*Galaxy type*,name,apparent magnitude,distance,mass  ,declination,right ascension,additional info,galaxy sub-type,stellar mass fraction,number of satellites

NEBULAE
str	str	     str  double	     double   double double      double      	 str    
Nebula,*Nebula type*,name,apparent magnitude,distance,mass  ,declination,right ascension,additional info

STARS
str	str	 str   double	         double   double  double     double          str	     double	str
Star,*Star type*,name,apparent magnitude,distance,mass  ,declination,right ascension,additional info,luminosity,spectral class
____________________________________________________________________________________

Lines containing '%' symbols are not included in the data