#include "handlers.hpp"
#include "my_server.hpp"
#include "../market/market.hpp"
#include "../market/misc_funcs.hpp"
#include "../market/exceptions.hpp"
#include <cstdlib> // for rand and srand
#include <ctime>   // for time
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
  srand(time(NULL)); // for rand
  try {
    Market* market = new Market();
    MyServer server(argc > 1 ? atoi(argv[1]) : 5000);
    server.setNotFoundErrPage("static/404.html");
    server.get("/first", new ShowPage("html_tests/first.html"));
    server.get("/signup", new ShowPage("html_tests/signup_page.html"));
    server.post("/signup", new signupHandler(market));
    server.get("/login", new ShowPage("html_tests/login_page.html"));
    server.post("/login", new loginHandler(market));
    server.get("/logout",new logoutHandler(market));
    server.get("/", new handleHome(market));

    server.get("/buyer_home", new handleBuyerHome(market));
    server.get("/seller_home", new handleSellerHome(market));
    server.get("/wallet", new walletHandler(market));
    server.post("/wallet", new walletChargeHandler(market));

    server.get("/add_offer", new handleAddingOffer(market));
    server.post("/add_offer", new submitOfferHandler(market));

    server.get("/compare", new ShowPage("html_tests/compare.html"));
    server.post("/compare", new compareHandler(market));
    server.get("/manager_home", new ShowPage("html_tests/manager_home.html"));
    server.post("/manager_home", new importHandler(market));
    server.get("/OK", new ShowPage("html_tests/OK.html"));
    server.get("/print_details", new handlePrintingDetails(market));
    server.get("/add_to_cart", new addToCartHandler(market));
    server.get("/view_cart", new cartHandler(market));
    server.post("/view_cart", new submitCartHandler(market));
    server.get("/submit_comment", new submitCommentHandler(market));
    server.get("/manager.png", new ShowImage("html_tests/manager.png"));
    server.get("/bazaar.png", new ShowImage("html_tests/bazaar.png"));
    server.run();
  }
  catch (Server::Exception e) {
    cerr << e.getMessage() << endl;
  }
}