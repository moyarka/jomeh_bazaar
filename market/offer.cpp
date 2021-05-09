#include "offer.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

Offer::Offer(unsigned int productid, unsigned int offerid, unsigned int _amount, double _price)
{
    productID = productid;
    offerID = offerid;
    amount = _amount;
    price = _price;
}

unsigned int Offer::getID() { return offerID; }

unsigned int Offer::getAmount() { return amount; }

unsigned int Offer::getProductID() { return productID; }

double Offer::getPrice() { return price; }

std::string Offer::printOfferForSeller()
{
    std::stringstream body;
    body << "<tr>";
    body << "<td>" << std::to_string(productID) << "</td>";
    body << "<td>" << std::to_string(offerID) << "</td>";
    body << "<td>" << std::setprecision(2) << std::fixed << price << "</td>";
    body << "<td>" << std::to_string(amount) << "</td>";
    body << "</tr>";
    return body.str();
}

void Offer::setAmount(unsigned int _amount) { amount = _amount; }
void Offer::setPrice(double _price) { price = _price; }

bool Offer::isThisMyDiscountCode(std::string code) 
{
    for(int i = 0; i < discountCodes.size(); i++) {
        if(discountCodes[i].code == code) return true;
    }

    return false;
}

void Offer::addToDiscountCodes(DiscountCode code) { discountCodes.push_back(code); }
std::vector<DiscountCode> Offer::getCodes() { return discountCodes; }

DiscountCode Offer::getDiscountCode(std::string code)
{
    for(int i = 0; i < discountCodes.size(); i++) {
        if(discountCodes[i].code == code) {
            DiscountCode dc = discountCodes[i];
            discountCodes.erase(discountCodes.begin() + i);
            return dc;
        }
    }

    return DiscountCode(0, "");
}

void Offer::updateAmount(int soldAmount) { amount -= soldAmount; }

SoldItem::SoldItem(int pid, int bid, int sid, int oid, int a, double sp) 
{
    productID = pid;
    buyerID = bid;
    sellerID = sid;
    offerID = oid;
    amount = a;
    soldPrice = sp;
}

int SoldItem::getProductID() { return productID; }

void SoldItem::printForBuyer()
{
    std::cout << offerID <<" | "  << sellerID << " | "  << soldPrice << " | " << amount << std::endl;
}

void SoldItem::printForSeller()
{
    std::cout << offerID <<" | "  << buyerID << " | "  << soldPrice << " | " << amount << std::endl;
}