#ifndef __MARKET_HPP__
#define __MARKET_HPP__
#include "user.hpp"
#include "offer.hpp"
#include "product.hpp"
#include "misc_funcs.hpp"
#include "comment_labeller.hpp"
#include <utility>

#define Post "POST"
#define Get "GET"


typedef std::vector<std::string> CommandLine;
typedef std::vector<std::string> productData;
typedef std::pair<Product*, Product*> foundProducts;

class Market
{
private:
    User* manager = new User(0, "admin@gmail.com", "admin", compute_hash("admin"));

    std::vector<Seller*> sellers;
    std::vector<Buyer*> buyers;

    std::vector<Mobile*> mobiles;
    std::vector<TV*> tvs;
    std::vector<Car*> cars;

    unsigned int productIDIndexer = 1;
    unsigned int OfferIDIndexer = 1;
    unsigned int UserIDIndexer = 1;

    User* currentLoggedInUser = nullptr;

    Comment_Labeller labeller;
public:
    Market() {}
    std::string processCommand(std::string command);
    std::string processGET(CommandLine& cl);
    std::string processPOST(CommandLine& cl);
    
    std::string logout();
    std::string logoutForBuyer();
    std::string handleImport(CommandLine& cl);
    std::string handleSignup(CommandLine& cl);
    std::string handleLogin(CommandLine& cl);
    void handleDiscountCodes(CommandLine& cl);
    void handleOffersForManager(CommandLine& cl);
    std::string handleOffersForBuyer(CommandLine& cl);
    void handleAllOffersForBuyer(CommandLine& cl);
    void handleComment(CommandLine& cl);
    std::string handleAddingToCart(CommandLine& cl);
    std::string handlePrintCart();
    std::string handleWalletCharge(CommandLine& cl);
    std::string handleCartSubmission();

    void printAllOffers(std::string order, std::string field);
    std::string printProducts();
    std::string printProductForBuyer(CommandLine& cl);
    std::string printCreditHistory(CommandLine& cl);
    std::string printCommentsOnProduct(CommandLine& cl);
    std::string printComaparison(CommandLine& cl);
    void printOrders(CommandLine& cl);
    void printSells();
    
    std::string addOffer(CommandLine& cl);
    void changeOffer(CommandLine& cl);

    void checkIfUserExists(std::string _email, std::string _name);
    bool checkIfCurrentUserIsBuyer();
    bool checkIfCurrentUserIsSeller();
    bool checkIfCurrentUserIsAdmin();
    bool checkIfProductIDIsValid(unsigned int id);
    User* findUserForLogin(std::string _email, std::string _pass);
    Offer* findOffer(unsigned int offerid);
    Product* findProduct(unsigned int productid);
    foundProducts findProductsForcomparison(int id1, int id2);
    
    std::vector<Offer*> findOffersOnProduct(unsigned int productid);
    bool checkIfDiscountCodeisValid(std::string discountCode);
    std::string generateDiscountCode();
};

#endif