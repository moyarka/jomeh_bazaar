#ifndef __OFFER_HPP__
#define __OFFER_HPP__
#include <string>
#include <vector>

class DiscountCode
{
public:
    double discountPercent;
    std::string code;
    DiscountCode(double dispercent, std::string _code) : discountPercent(dispercent), code(_code) {}
};

class SoldItem
{
private:
    int productID;
    int buyerID;
    int sellerID;
    int offerID;
    int amount;
    double soldPrice;
public:
    SoldItem(int pid, int bid, int sid, int oid, int a, double sp);
    void printForSeller();
    void printForBuyer();
    int getProductID();
};

class Offer
{
private:
    unsigned int productID;
    unsigned int offerID;
    unsigned int amount;
    double price;
    std::vector<DiscountCode> discountCodes;
public:
    Offer(unsigned int productid, unsigned int offerid, unsigned int _amount, double _price);
    unsigned int getID();
    unsigned int getProductID();
    unsigned int getAmount();
    double getPrice();

    void setPrice(double _price);
    void setAmount(unsigned int _amount);
    std::string printOfferForSeller();
    bool isThisMyDiscountCode(std::string code);
    void addToDiscountCodes(DiscountCode code);
    void updateAmount(int soldAmount);
    std::vector<DiscountCode> getCodes();
    DiscountCode getDiscountCode(std::string code);
};

#endif