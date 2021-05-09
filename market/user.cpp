#include "user.hpp"
#include "misc_funcs.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstdlib>

User::User(unsigned int _id, std::string _email, std::string _name, long long _pass)
    : id(_id), email(_email), username(_name), password(_pass) {
        creditHistory = std::vector<double>(1);
    }

bool User::areTheseMyEmailAndUsername(std::string _email, std::string _name)
{
    return (email == _email || username == _name);
}

bool User::areTheseMyEMailAndPassword(std::string _email, long long _pass)
{
    if(email == _email && password == _pass) {
        return true;
    }
    return false;
}

void User::PrintUser()
{
    std::cout << id << ' ' << email << ' ' << username << ' ' << password << std::endl;
}

std::string User::printCreditHistory(int count)
{
    std::stringstream credit;
    int size = creditHistory.size();
    for(int i = 0; i < size && i < count; i++) {
        credit << std::setprecision(2) << std::fixed << creditHistory[size - i - 1];
    }
    return credit.str();
}

std::string User::getName() {return username; } 

int User::getID() { return id; }

double User::getCredit() {return creditHistory.back(); }

void User::updateCredit(double c)
{
    double currentCredit = creditHistory.back();
    currentCredit += c;
    creditHistory.push_back(currentCredit);
}


void Seller::AddToOffers(Offer* o)
{
    offers.push_back(o);
}

bool Seller::checkIfOfferCanBeAdded(unsigned int productid)
{
    for(int i = 0; i < offers.size(); i++) {
        if(productid == offers[i]->getProductID() && offers[i]->getAmount() != 0) return false;
    }

    return true;
}

bool compareOfferIDs(Offer* a, Offer* b) { return a->getID() < b->getID(); }

std::string Seller::printOffers()
{
    std::string body;
    if(offers.size() == 0) {
        body = "<p style=\"font-size=16\">No Offers have been posted yet!</p>";
        std::cout << body << std::endl;
        return body;
    }
    
    body += "<table>"; 
    body += "<th>Product ID</th><th>Offer ID</th><th>Offer Unit Price</th><th>Offer Amount</th>";
    std::sort(offers.begin(), offers.end(), compareOfferIDs);
    for(int i = 0; i < offers.size(); i++) {
        body += offers[i]->printOfferForSeller();   
    }
    body += "</table>";
    return body;
}

Offer* Seller::findOffer(unsigned int offerid) {
    for(int i = 0; i < offers.size(); i++) {
        if(offers[i]->getID() == offerid) return offers[i];
    }

    return nullptr;
}

void Seller::changeOffer(unsigned int offerid, double price, unsigned int amount)
{
    Offer* offer = this->findOffer(offerid);
    if(offer == nullptr) std::abort();
    offer->setPrice(price);
    offer->setAmount(amount);
}

std::vector<Offer*> Seller::getOffers() { return offers; }

void Buyer::addToCart(BuyerOffer* bo) { cart.push_back(bo); }

std::vector<BuyerOffer*> Buyer::getCart() {return cart; }

std::vector<SoldItem*> Buyer::getOrders() {return orders; }

std::vector<SoldItem*> Seller::getSells() {return sells; }

void Seller::addToSoldItems(SoldItem* si) { sells.push_back(si); }
void Buyer::emptyCart()
{
    for(int i = 0; i < cart.size(); i++) {
        cart[i]->offer->updateAmount(-1 * cart[i]->amount);
        delete cart[i];
    }

    cart.clear();
}

void Buyer::submitCart()
{
    if(cart.size() == 0) return;
    std::vector<BuyerOffer*> bo;
    std::vector<Seller*> sellers;
    double totalPrice = 0;
    bool seller_is_found = false;
    for(int i = 0; i < cart.size(); i++) {
        totalPrice += cart[i]->FinalPrice;
        for(int j = 0; j < sellers.size(); j++) {
            if(sellers[j]->getID() == cart[i]->seller->getID()) seller_is_found = true;
        }
        if(!seller_is_found) sellers.push_back(cart[i]->seller);
    }

    if(totalPrice > creditHistory.back()) throwBadRequest();
    for(int i = 0; i < sellers.size(); i++) {
        for(int j = 0; j < cart.size(); j++) {
            if(cart[j]->seller == sellers[i]) {
                bo.push_back(cart[j]);
            }
        }
        double sellerIncome = 0;
        for(int j = 0; j < bo.size(); j++) {
            sellerIncome += bo[j]->FinalPrice;
        }
        sellers[i]->updateCredit(sellerIncome);
        bo.clear();
    }

    for(int i = 0; i < cart.size(); i++) {
        BuyerOffer* item = cart[i];
        SoldItem* si = new SoldItem(item->offer->getProductID(), this->id, item->seller->getID(), 
        item->offer->getID(), item->amount, (double)item->FinalPrice / item->amount);
        orders.push_back(si);
        item->seller->addToSoldItems(si);
        delete cart[i];
    }
    cart.clear();
    this->updateCredit(-1 * totalPrice);

}
