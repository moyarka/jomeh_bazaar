#ifndef __EXCEPTIONS_HPP__
#define __EXCEPTIONS_HPP__

#include <exception>

class BadRequest : public std::exception
{
public :
    const char* what() const noexcept { return "Bad Request";}
};

class NotFound : public std::exception
{
public :
    const char* what() const noexcept { return "Not Found";}
};

class PermissionDenied : public std::exception
{
public:
    const char* what() const noexcept { return "Permission Denied";}
};

class PasswordMismatch : public std::exception
{
public:
    const char* what() const noexcept { return "Pawwords Do Not Match";}
};

#endif