#ifndef _MY_HANDLERS_
#define _MY_HANDLERS_

#include "../market/market.hpp"
#include "../server/server.hpp"
#include <cstdlib> // for rand and srand
#include <ctime>   // for time
#include <iostream>

class RandomNumberHandler : public RequestHandler {
public:
  Response *callback(Request *);
};

class LoginHandler : public RequestHandler {
public:
  Response *callback(Request *);
};

class UploadHandler : public RequestHandler {
public:
  Response *callback(Request *);
};

class ColorHandler : public TemplateHandler {
public:
  ColorHandler(std::string filePath);
  std::map<std::string, std::string> handle(Request *req);
};
// -----------------------------------------------------------


class signupHandler : public RequestHandler {
private:
  Market* market;
public:
  signupHandler(Market* m) { market = m; }
  Response* callback(Request*);
};

class loginHandler : public RequestHandler {
private:
  Market* market;
public:
  loginHandler(Market* m) { market = m; };
  Response* callback(Request*);
};

class logoutHandler : public RequestHandler {
private:
  Market* market;
public:
  logoutHandler(Market* m) { market = m; }
  Response* callback(Request*);
};

class handleBuyerHome : public RequestHandler {
private:
  Market* market;
public:
  handleBuyerHome(Market* m) { market = m; }
  Response* callback(Request*);
};

class handleSellerHome : public RequestHandler {
private:
  Market* market;
public:
  handleSellerHome(Market* m) { market = m; }
  Response* callback(Request*);
};

class handleHome : public RequestHandler {
private:
  Market* market;
public:
  handleHome(Market* m) { market = m; }
  Response* callback(Request*);
};

class importHandler : public RequestHandler {
private:
  Market* market;
public:
  importHandler(Market* m) { market = m; }
  Response* callback(Request*);
};

class handlePrintingDetails : public RequestHandler {
private:
  Market* market;
public:
  handlePrintingDetails(Market* m) { market = m; }
  Response* callback(Request*);
};

class handleAddingOffer : public RequestHandler {
private:
  Market* market;
public:
  handleAddingOffer(Market* m) { market = m; }
  Response* callback(Request*);
};

class submitOfferHandler : public RequestHandler {
private:
  Market* market;
public:
  submitOfferHandler(Market* m) { market = m; }
  Response* callback(Request*);
};

class walletHandler : public RequestHandler {
private:
  Market* market;
public:
  walletHandler(Market* m) { market = m; }
  Response* callback(Request*);
};

class walletChargeHandler : public RequestHandler {
private:
  Market* market;
public:
  walletChargeHandler(Market* m) { market = m; }
  Response* callback(Request*); 
};

class addToCartHandler : public RequestHandler {
private:
  Market* market;
public:
  addToCartHandler(Market* m) { market = m; }
  Response* callback(Request*); 
};

class cartHandler : public RequestHandler {
private:
  Market* market;
public:
  cartHandler(Market* m) { market = m; }
  Response* callback(Request*); 
};

class submitCartHandler : public RequestHandler {
private:
  Market* market;
public:
  submitCartHandler(Market* m) { market = m; }
  Response* callback(Request*); 
};

class submitCommentHandler : public RequestHandler {
private:
  Market* market;
public:
  submitCommentHandler(Market* m) { market = m; }
  Response* callback(Request*); 
};

class compareHandler : public RequestHandler {
private:
  Market* market;
public:
  compareHandler(Market* m) { market = m; }
  Response* callback(Request*); 
};

#endif
