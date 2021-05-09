#include "market.hpp"
#include "misc_funcs.hpp"
#include <iostream>
void Market::checkIfUserExists(std::string _email, std::string _name)
{
    if(_email == "admin@gmail.com" || _name == "admin") throwBadRequest();
    for(int i = 0; i < buyers.size(); i++) {
        if(buyers[i]->areTheseMyEmailAndUsername(_email, _name)) throwBadRequest();
    }
    
    for(int i = 0; i < sellers.size(); i++) {
        if(sellers[i]->areTheseMyEmailAndUsername(_email, _name)) throwBadRequest();
    }
}

bool Market::checkIfCurrentUserIsBuyer() 
{
    for(int i = 0; i < buyers.size(); i++) {
        if(currentLoggedInUser == buyers[i]) return true;
    }
    return false;
}

bool Market::checkIfCurrentUserIsSeller() 
{
    for(int i = 0; i < sellers.size(); i++) {
        if(currentLoggedInUser == sellers[i]) return true;
    }
    return false;
}

bool Market::checkIfCurrentUserIsAdmin()
{
    return (currentLoggedInUser == manager);
}
User* Market::findUserForLogin(std::string _email, std::string _pass)
{
    for(int i = 0; i < sellers.size(); i++) {
        if(sellers[i]->areTheseMyEMailAndPassword(_email, compute_hash(_pass))) return sellers[i];
    }

    for(int i = 0; i < buyers.size(); i++) {
        if(buyers[i]->areTheseMyEMailAndPassword(_email, compute_hash(_pass))) return buyers[i];
    }

    if(_email == "admin@gmail.com" && _pass == "admin") return manager;
    return nullptr; 
}

Offer* Market::findOffer(unsigned int offerid)
{
    for(int i = 0; i < sellers.size(); i++) {
        Offer* offer = sellers[i]->findOffer(offerid);
        if(offer != nullptr) return offer;
    }
    return nullptr;
}

bool Market::checkIfProductIDIsValid(unsigned int id) 
{
    for(int i = 0; i < mobiles.size(); i++) {
        if(mobiles[i]->getID() == id) return true;
    }

    for(int i = 0; i < tvs.size(); i++) {
        if(tvs[i]->getID() == id) return true;
    }

    for(int i = 0; i < cars.size(); i++) {
        if(cars[i]->getID() == id) return true;
    }

    return false;
}

Product* Market::findProduct(unsigned int productid)
{
    for(int i = 0; i < mobiles.size(); i++) {
        if(mobiles[i]->getID() == productid) return mobiles[i];
    }

    for(int i = 0; i < tvs.size(); i++) {
        if(tvs[i]->getID() == productid) return tvs[i];
    }

    for(int i = 0; i < cars.size(); i++) {
        if(cars[i]->getID() == productid) return cars[i];
    }

    return nullptr;
}

std::vector<Offer*> Market::findOffersOnProduct(unsigned int productid)
{
    std::vector<Offer*> relevantOffers;
    for(int i = 0; i < sellers.size(); i++) {
        std::vector<Offer*> sellerOffers = sellers[i]->getOffers();
        for(int j = 0; j < sellerOffers.size(); j++) {
            if(sellerOffers[j]->getProductID() == productid) relevantOffers.push_back(sellerOffers[j]);
        }
    }

    return relevantOffers;
}

bool Market::checkIfDiscountCodeisValid(std::string _code) 
{
    for(int i = 0; i < sellers.size(); i++) {
        std::vector<Offer*> offers = sellers[i]->getOffers();
        for(int j = 0; j < offers.size(); j++) {
            std::vector<DiscountCode> codes = offers[i]->getCodes();
            for(int k = 0; k < codes.size(); k++) {
                if(codes[i].code == _code) return false;
            }
        }
    }
    return true;
}

foundProducts Market::findProductsForcomparison(int id1, int id2)
{
    foundProducts fp;
    for(int i = 0; i < mobiles.size(); i++) {
        if(mobiles[i]->getID() == id1) fp.first = mobiles[i];
        if(mobiles[i]->getID() == id2) fp.second = mobiles[i];
    }

    if((fp.first != nullptr && fp.second == nullptr) || (fp.first == nullptr && fp.second != nullptr)) {
        throwBadRequest();
    }

    for(int i = 0; i < tvs.size(); i++) {
        if(tvs[i]->getID() == id1) fp.first = tvs[i];
        if(tvs[i]->getID() == id2) fp.second = tvs[i];
    }

    if((fp.first != nullptr && fp.second == nullptr) || (fp.first == nullptr && fp.second != nullptr)) {
        throwBadRequest();
    }

    for(int i = 0; i < cars.size(); i++) {
        if(cars[i]->getID() == id1) fp.first = cars[i];
        if(cars[i]->getID() == id2) fp.second = cars[i];
    }

    if((fp.first != nullptr && fp.second == nullptr) || (fp.first == nullptr && fp.second != nullptr)) {
        throwBadRequest();
    }

    if(fp.first == nullptr || fp.second == nullptr) throwNotFound();
    return fp;
}