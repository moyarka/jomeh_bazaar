#ifndef __MISC_FUNCS_HPP__
#define __MISC_FUNCS_HPP__
#include <vector>
#include <string>
#include "../server/server.hpp"

std::string printSuccessMessage();
void throwBadRequest();
void throwNotFound();
void throwPermissionDenied();
void throwPasswordMismatch();

std::vector<std::string> tokenizer(std::string line, char seperator);
long long compute_hash(std::string s);

std::string makeNavBarCSS();
std::string makeTableCSS();
std::string makeFormCSS();
std::string makeBuyerNavBar();
std::string makeSellerNavBar();
Response* errorPage(std::exception* ex);
#endif