#include "misc_funcs.hpp"
#include "exceptions.hpp"
#include <iostream>

std::string printSuccessMessage()
{
    return "OK\n";
}

void throwBadRequest()
{
    BadRequest* bad  = new BadRequest();
    throw bad;
}

void throwNotFound()
{
    NotFound* not_found  = new NotFound();
    throw not_found;
}

void throwPermissionDenied()
{
    PermissionDenied* permission  = new PermissionDenied();
    throw permission;
}

void throwPasswordMismatch()
{
    PasswordMismatch* passmmatch  = new PasswordMismatch();
    throw passmmatch;
}

std::vector<std::string> tokenizer(std::string line, char seperator)
{
    int start = 0;
    std::string token;
    std::vector<std::string> tokens;
    for(int i = 0; i < line.length(); i++){
        if(i == line.length() - 1 && line[i] != seperator){
            token = line.substr(start, i - start + 1);
            tokens.push_back(token);
        }
        else {
            if(line[i] == seperator){
            token = line.substr(start, i - start);
            start = i + 1;
            tokens.push_back(token);
            }
        }    
    }   
    return tokens;
}

long long compute_hash(std::string s) {
    const int p = 31;
    const int m = 1e9 + 9;
    long long hash_value = 0;
    long long p_pow = 1;
    for (int i = 0; i < s.size(); i++) {
        hash_value = (hash_value + (s[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value;
}

std::string makeNavBarCSS() {
  std::string style;
  style += "ul {position: fixed;top: 0;width: 100%;list-style-type: none;margin: 0;padding: 0;overflow: hidden;background-color: #333;}";
  style += "li {float: left;}li a {display: block;color: white;text-align: center;padding: 14px 16px;text-decoration: none;}li a:hover:not(.active) {background-color: #111;}.active {background-color: #4CAF50;}";
  return style;
}

std::string makeTableCSS() {
  std::string style;
  style += "button:not(.ordinary) {background: none;border: none;padding: 0;color: black;text-decoration: underline;cursor: pointer;}";
  style += "table, th, td {width: 50%; border: 2px solid green;border-collapse: collapse;font-size: medium}";
  style += "th {background-color: #4CAF50;color: white;}";
  style += "th, td {padding: 15px;text-align: center;}";
  style += "tr:nth-child(even) {background-color: #f2f2f2;}";
  style += "";
  return style;
}

std::string makeBuyerNavBar() {
  std::string body;
  body += "<ul>";
  body += "<li><a href=\"/\" class=\"active\">Home</a></li>";
  body += "<li><a href=\"/wallet\">Wallet</a></li>";
  body += "<li><a href=\"/view_cart\">View Cart</a></li>";
  body += "<li><a href=\"/compare\">Compare Products</a></li>";
  body += "<li><a href=\"/logout\">Log Out</a></li>";
  body += "</ul>";
  return body;
}

std::string makeSellerNavBar() {
  std::string body;
  body += "<ul>";
  body += "<li><a href=\"/\" class=\"active\">Home</a></li>";
  body += "<li><a href=\"/wallet\">Wallet</a></li>";
  body += "<li><a href=\"/add_offer\">Add Offer</a></li>";
  body += "<li><a href=\"/logout\">Log Out</a></li>";
  body += "</ul>";
  return body;
}

std::string makeFormCSS() {
  std::string result;
  result += "input[type=text] {width: 40%;padding: 12px 20px;margin: 8px 0;box-sizing: border-box;border: 2px solid red;border-radius: 4px;}";
  result += "input[type=submit] {background-color: white;border: 2px solid green;color: black;padding: 15px 32px;text-align: center;text-decoration: black;display: inline-block;font-size: 16px;}";
  result += "input[type=submit]:hover {background-color: lightgreen;}";
  return result;
}

Response* errorPage(std::exception* ex) {
  Response* res = new Response;
  res->setHeader("Content-Type", "text/html");
  std::string body;
  body += "<!DOCTYPE html>";
  body += "<style>";
  body += "* {box-sizing: border-box;font-family: Verdana, Geneva, Tahoma, sans-serif;}";
  body += ".button {background-color: white;border: 2px solid green;color: black;padding: 15px 32px;text-align: center;text-decoration: black;display: inline-block;font-size: 16px;}";
  body += "a:hover {background-color: lightgreen;}";
  body += ".message {margin: 0;position: absolute;top: 50%;left: 50%;margin-right: -50%;transform: translate(-50%, -50%);text-align: center;font-size: 30px;}";
  body += "</style>";
  body += "<body>";
  body += "<div class=\"message\">";
  body += "<p>Oops! An Error Occured While Handling Your Request</p>";
  body += "<p>[Error Code: 500, Message: ";
  body += ex->what();
  body += "]</p><a href=\"/\" class=\"button\">Back To Home</a>";
  body += "</div></body></html>";
  res->setBody(body);
  delete ex;
  return res;
}