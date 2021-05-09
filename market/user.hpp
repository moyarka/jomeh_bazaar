#ifndef __USER_HPP__
#define __USER_HPP__
#include <string>
#include <vector>
#include "./offer.hpp"

class User
{
protected:
    unsigned int id;
    std::string email;
    std::string username;
    // std::string password;
    long long password;
    std::vector<double> creditHistory;
public:
    User(unsigned int _id, std::string _email, std::string _name, long long _pass);
    bool areTheseMyEmailAndUsername(std::string _email, std::string _name);
    bool areTheseMyEMailAndPassword(std::string _email, long long _pass);
    void PrintUser();
    std::string printCreditHistory(int count);
    std::string getName();
    int getID();
    double getCredit();
    void updateCredit(double c);
};

class Seller : public User
{
private:
    std::vector<Offer*> offers;
    std::vector<SoldItem*> sells;
public:
    Seller(unsigned int _id, std::string _email, std::string _name, long long  _pass)
    : User(_id, _email, _name, _pass) {}

    bool checkIfOfferCanBeAdded(unsigned int productid);
    Offer* findOffer(unsigned int offerid);
    std::vector<Offer*> getOffers();
    void changeOffer(unsigned int offerid, double price, unsigned int amount);
    void AddToOffers(Offer* o);
    void addToSoldItems(SoldItem* si);
    std::vector<SoldItem*> getSells();
    std::string printOffers();
};

class BuyerOffer 
{
public:
    Seller* seller;
    Offer* offer;
    double FinalPrice;
    int amount;
    BuyerOffer(Offer* o, Seller* s, int a, double p) : offer(o), seller(s), amount(a), FinalPrice(p) {}
};

class Buyer : public User
{
private:
    std::vector<BuyerOffer*> cart;
    std::vector<SoldItem*> orders;

public:
    Buyer(unsigned int _id, std::string _email, std::string _name, long long _pass)
    : User(_id, _email, _name, _pass) {}
    
    void chargeWallet(double amount);
    void addToCart(BuyerOffer* bo);
    std::vector<BuyerOffer*> getCart();
    std::vector<SoldItem*> getOrders();
    void submitCart();
    void emptyCart();
};
#endif
