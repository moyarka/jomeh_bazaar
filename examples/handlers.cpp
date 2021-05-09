#include "handlers.hpp"
#include "../market/misc_funcs.hpp"
#include "../market/market.hpp"

using namespace std;

Response *RandomNumberHandler::callback(Request *req) {
  Response *res = new Response;
  res->setHeader("Content-Type", "text/html");
  string body;
  body += "<!DOCTYPE html>";
  body += "<html>";
  body += "<body style=\"text-align: center;\">";
  body += "<h1>AP HTTP</h1>";
  body += "<p>";
  body += "a random number in [1, 10] is: ";
  body += to_string(rand() % 10 + 1);
  body += "</p>";
  body += "<p>";
  body += "SeddionId: ";
  body += req->getSessionId();
  body += "</p>";
  body += "</body>";
  body += "</html>";
  res->setBody(body);
  return res;
}

Response *LoginHandler::callback(Request *req) {
  string username = req->getBodyParam("username");
  string password = req->getBodyParam("password");
  if (username == "root")
    throw Server::Exception("Remote root access has been disabled.");
  cout << "username: " << username << ",\tpassword: " << password << endl;
  Response *res = Response::redirect("/rand");
  res->setSessionId("SID");
  return res;
}

Response *UploadHandler::callback(Request *req) {
  string name = req->getBodyParam("file_name");
  string file = req->getBodyParam("file");
  cout << name << " (" << file.size() << "B):\n" << file << endl;
  Response *res = Response::redirect("/");
  return res;
}

ColorHandler::ColorHandler(string filePath) : TemplateHandler(filePath) {}

map<string, string> ColorHandler::handle(Request *req) {
  map<string, string> context;
  string newName = "I am " + req->getQueryParam("name");
  context["name"] = newName;
  context["color"] = req->getQueryParam("color");
  return context;
}

// --------------------------------------------------------------

Response* signupHandler::callback(Request* req) {
  try {
    string password = req->getBodyParam("password");
    if(password != req->getBodyParam("re_pass"))
      throwPasswordMismatch();
    
    string command = "POST signup ? email " + req->getBodyParam("email");
    command += " username " + req->getBodyParam("username");
    command += " password " + req->getBodyParam("password");
    command += " type " + req->getBodyParam("type");
    market->processCommand(command);
    Response* res = Response::redirect("/");
    res->setSessionId(req->getBodyParam("username"));
    return res;
  }
  catch(exception* ex) { return errorPage(ex); }
}

Response* loginHandler::callback(Request* req) {
  try {
    Response* res;
    string command;
    string email = req->getBodyParam("email");
    string password = req->getBodyParam("password");
    command += "POST login ? email " + email;
    command += " password " + password;
    market->processCommand(command);
    cout << "Market part is okay\n";
    string username = market->findUserForLogin(email, password)->getName();
    res = Response::redirect("/");
    res->setSessionId(username);
    return res;
  }
  catch(exception* ex) { return errorPage(ex); }
}

Response* logoutHandler::callback(Request* req) {
  try {
    if(market->checkIfCurrentUserIsBuyer()) market->logoutForBuyer();
    else market->logout();
    Response* res;
    res = Response::redirect("/");
    res->setSessionId("");
    return res;
  }
  catch(exception* ex) { return errorPage(ex); }
}

Response* handleBuyerHome::callback(Request* req) {
  try {
    if(!market->checkIfCurrentUserIsBuyer()) throwPermissionDenied();
    string body;
    body += "<!DOCTYPE html>";
    body += "<html>";
    body += "<style> *{ box-sizing: border-box; font-family: Verdana, Geneva, Tahoma, sans-serif;}";
    body += makeNavBarCSS();
    body += makeTableCSS();
    body += "</style>";
    body += "<body>";
    body += makeBuyerNavBar();
    body += "<br><h2>Jomeh Bazaar</h2>";
    body += "<h2>" + req->getSessionId() + "</h2>";
    body += market->printProducts();
    body += "</body>";
    body += "</html>";
    Response* res = new Response;
    res->setHeader("Content-Type", "text/html");
    res->setBody(body);
    return res;
  }
  catch(exception* ex) { return errorPage(ex); }
}

Response* handleHome::callback(Request* req) {
  Response* res;
  if(market->checkIfCurrentUserIsBuyer()) res = Response::redirect("/buyer_home");
  else if(market->checkIfCurrentUserIsSeller()) res = Response::redirect("/seller_home");
  else if(market->checkIfCurrentUserIsAdmin()) res = Response::redirect("/manager_home");
  else res = Response::redirect("/first");
  return res;
}

Response* importHandler::callback(Request* req)  {
  try {
    Response* res;
    string command;
    command += "POST importProducts ? type " + req->getBodyParam("type");
    command += " filePath " + req->getBodyParam("file");
    market->processCommand(command);
    res = Response::redirect("/OK");
    return res;
  }
  catch(exception* ex) { return errorPage(ex); }
}

Response* handleSellerHome::callback(Request* req) {
  try {
    if(!market->checkIfCurrentUserIsSeller()) throwPermissionDenied();
    string body;
    body += "<!DOCTYPE html>";
    body += "<html>";
    body += "<style> *{ box-sizing: border-box; font-family: Verdana, Geneva, Tahoma, sans-serif;}";
    body += makeNavBarCSS();
    body += makeTableCSS();
    body += "</style>";
    body += "<body>";
    body += makeSellerNavBar();
    body += "<br><h1>Jomeh Bazaar</h1>";
    body += "<h2>" + req->getSessionId() + "</h2>";
    string command = "GET myOffers";
    body += market->processCommand(command);
    body += "</body></html>";
    Response* res = new Response;
    res->setHeader("Content-Type", "text/html");
    res->setBody(body);
    return res;
  }
  catch(exception* ex) { return errorPage(ex); }
}

Response* handlePrintingDetails::callback(Request* req) {
  try {
  string productID = req->getQueryParam("product_id");
  string command = "GET productDetail ? productId " + productID;
  string body;
  body += "<html>";
  body += "<style>";
  body += "*{ box-sizing: border-box; font-family: Verdana, Geneva, Tahoma, sans-serif;}";
  body += "button[type=submit].ordinary {background-color: white; border: 2px solid navy;color: black;padding: 15px 32px;text-align: center;text-decoration: black;display: inline-block;font-size: 16px;} button[type=submit].ordinary:hover {background-color: lightblue;}";
  body += "input[type=text] {width: 40%;padding: 12px 20px;margin: 8px 0;box-sizing: border-box;border: 2px solid navy;border-radius: 4px;}";
  body += makeTableCSS();
  body += makeNavBarCSS();
  body += "</style>";
  body += "<body>";
  if(market->checkIfCurrentUserIsSeller()) body += makeSellerNavBar();
  else if(market->checkIfCurrentUserIsBuyer()) body += makeBuyerNavBar();
  else throwPermissionDenied();
  body += market->processCommand(command);
  Response* res = new Response;
  res->setHeader("Content-Type", "text/html");
  
  if(market->checkIfCurrentUserIsSeller()) {
    body += "</body></html>"; 
    res->setBody(body);
    return res;
  }
  body += "<br><br><br>";
  command = "GET offersOnProduct ? productId " + productID;
  body += market->processCommand(command) + "<br><br><br>";
  command = "GET comments ? productId " + productID;
  body += market->processCommand(command);
  body += "<br><form method=\"get\" action=\"/submit_comment\">";
  body += "<label for=\"comment\">Add A Comment On This Product: </label>";
  body += "<br><input type=\"text\" name=\"comment\">";
  body += "<br><button type=\"submit\" class= \"ordinary\" name=\"product_id\" value=\"" + productID + "\">";
  body += "Submit Comment</button>";
  body += "</body></html>";
  res->setBody(body);
  return res;
  }
  catch(exception* ex) { return errorPage(ex); }
}

Response* handleAddingOffer::callback(Request* req) {
  try {
    string body;
    body += "<!DOCTYPE html>";
    body += "<html>";
    body += "<style>"; 
    body += "*{ box-sizing: border-box; font-family: Verdana, Geneva, Tahoma, sans-serif;}";
    body += makeFormCSS();  
    body += makeNavBarCSS();
    body += makeTableCSS();
    body += "</style>";
    body += "<body>";
    body += makeSellerNavBar();
    body += "<br><h2>Jomeh Bazaar</h2>";
    body += "<form method=\"post\" action=\"/add_offer\">";
    body += "Product ID<br><input type=\"text\" name=\"product_id\"><br>";
    body += "Offer Unit Price<br><input type=\"text\" name=\"offer_unit_price\"><br>";
    body += "Offer Amount<br><input type=\"text\" name=\"offer_amount\"><br>";
    body += "<br><input type=\"submit\" value=\"Sumbit Offer\">";
    body += "</form><br>";
    body += market->processCommand("GET getProducts");
    body += "</body></html>";
    Response* res = new Response;
    res->setHeader("Content-Type", "text/html");
    res->setBody(body);
    return res;
  }

  catch(exception* ex) { return errorPage(ex); }
}

Response* submitOfferHandler::callback(Request* req) {
  try {
    Response* res;
    string command = "POST offer ? productId " + req->getBodyParam("product_id");
    command += " offerUnitPrice " + req->getBodyParam("offer_unit_price");
    command += " offerAmount " + req->getBodyParam("offer_amount");
    market->processCommand(command);
    res = Response::redirect("/OK");
    return res;
  }

  catch(exception* ex) { return errorPage(ex); }
}

Response* walletHandler::callback(Request* req) {
  try {
    string body;
    body += "<!DOCTYPE html>";
    body += "<html>";
    body += "<style>";
    body += "*{ box-sizing: border-box; font-family: Verdana, Geneva, Tahoma, sans-serif;}";
    body += makeNavBarCSS();
    body += makeFormCSS();
    body += "</style>";
    body += "<body>";
    if(market->checkIfCurrentUserIsBuyer()) body += makeBuyerNavBar();
    else if(market->checkIfCurrentUserIsSeller()) body += makeSellerNavBar();
    else throwPermissionDenied();
    body += "<br><h2>Jomeh Bazaar</h2><br>";
    body += "<p>Your Current Balance is: ";
    string command = "GET wallet ? count 1";
    body += market->processCommand(command);
    Response* res = new Response;
    res->setHeader("Content-Type", "text/html");
    if(market->checkIfCurrentUserIsSeller()) {
      body += "</body></html>";
      res->setBody(body);
      return res;
    }
    body += "<form method=\"post\" action=\"/wallet\">";
    body += "<br>To Increase Your Balance, Type In The Field Below: <br>";
    body += "<input type=\"text\" name=\"amount\" placeholder=\"Amount\" required>";
    body += "<br><input type=\"submit\" value=\"Charge\">";
    body += "</form></body></html>";
    res->setBody(body);
    return res;
  }
  catch(exception* ex) { return errorPage(ex); }
}

Response* walletChargeHandler::callback(Request* req) {
  try {
    string command;
    command += "POST chargeWallet ? amount " + req->getBodyParam("amount");
    string dummy = market->processCommand(command);
    Response* res = Response::redirect("/OK");
    return res;
  }
  catch(exception* ex) { return errorPage(ex); }
}

Response* addToCartHandler::callback(Request* req) {
  try {
    string command;
    command += "POST addToCart ? offerId " + req->getQueryParam("offer_id");
    command += " amount " + req->getQueryParam("amount");
    cout << command << endl;
    string dummy = market->processCommand(command);
    Response* res = Response::redirect("/OK");
    return res;
  }
  catch(exception* ex) { return errorPage(ex); }
}

Response* cartHandler::callback(Request* req) {
  try {
    string command = "GET cart"; 
    string body;
    body += "<!DOCTYPE html>";
    body += "<html>";
    body += "<style>";
    body += "*{ box-sizing: border-box; font-family: Verdana, Geneva, Tahoma, sans-serif;}";
    body += "input[type=submit] {background-color: white; border: 2px solid navy;color: black;padding: 15px 32px;text-align: center;text-decoration: black;display: inline-block;font-size: 16px;}";
    body += "input[type=submit]:hover {background-color: lightblue;}";
    body += makeNavBarCSS();
    body += makeTableCSS();
    body += "</style>";
    body += "<body>";
    body += makeBuyerNavBar();
    body += "<br><h2>Jomeh Bazaar</h2>";
    body += market->processCommand(command);
    body += "</body></html>";
    Response* res = new Response;
    res->setHeader("Content-Type", "text/html");
    res->setBody(body);
    return res;
  }
  catch(exception* ex) { return errorPage(ex); }
}

Response* submitCartHandler::callback(Request* req) {
  try {
    string command = "POST submitCart";
    string dummy = market->processCommand(command);
    Response* res = Response::redirect("/OK");
    return res;
  }
  catch(exception* ex) { return errorPage(ex); }
}

Response* submitCommentHandler::callback(Request* req) {
  try {
    string command = "POST comment ? productId " + req->getQueryParam("product_id");
    command += " comment " + req->getQueryParam("comment");
    market->processCommand(command);
    Response* res = Response::redirect("/OK");
    return res;
  }
  catch(exception* ex) {
    return errorPage(ex);
  }
}

Response* compareHandler::callback(Request* req) {
  try {
    string command = "GET compare ? productId1 " + req->getBodyParam("product_id_1");
    command += " productId2 " + req->getBodyParam("product_id_2");
    cout << command;
    string body;
    body += "<!DOCTYPE html>";
    body += "<html>";
    body += "<style>";
    body += "*{ box-sizing: border-box; font-family: Verdana, Geneva, Tahoma, sans-serif;}";
    body += makeNavBarCSS();
    body += makeTableCSS();
    body += "</style>";
    body += "<body>";
    body += makeBuyerNavBar() + "<br><br><br><br>";
    body += market->processCommand(command);
    body += "</body></html>";
    Response* res = new Response;
    res->setHeader("Content-Type", "text/html");
    res->setBody(body);
    return res;
  }
  catch(exception* ex) { return errorPage(ex); }
}