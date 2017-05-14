#ifndef LOGGING_H
#define LOGGING_H

#include <iostream>

# define WARN(x) do { std::cout << __PRETTY_FUNCTION__ <<":"<< __LINE__ << " " << x << std::endl; } while(0)
# define INFO(x) do { std::cout << __PRETTY_FUNCTION__ <<":"<< __LINE__ << " " << x << std::endl; } while(0)


#endif // GPS_MANAGER_H
