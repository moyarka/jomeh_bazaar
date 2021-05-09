#include "sort_methods.hpp"

bool ascendOffer(Offer* a, Offer* b) { return a->getID() < b->getID(); }
bool descendOffer(Offer* a, Offer* b) { return a->getID() > b->getID(); }
bool ascendProduct(Offer* a, Offer* b) { return a->getProductID() < b->getProductID(); }
bool descendProduct(Offer* a, Offer* b) { return a->getProductID() > b->getProductID(); }
bool ascendPrice(Offer* a, Offer* b) 
{
        if(a->getPrice() < b->getPrice()) return true;
        else if(a->getPrice() == b->getPrice()) return ascendOffer(a, b);
        else return false;
}

bool descendPrice(Offer* a, Offer* b) 
{
        if(a->getPrice() > b->getPrice()) return true;
        else if(a->getPrice() == b->getPrice()) return ascendOffer(a, b);
        else return false;
}

bool ascendAmount(Offer* a, Offer* b) 
{
        if(a->getAmount() < b->getAmount()) return true;
        else if(a->getAmount() == b->getAmount()) return ascendOffer(a, b);
        else return false;
}

bool descendAmount(Offer* a, Offer* b) 
{
        if(a->getAmount() > b->getAmount()) return true;
        else if(a->getAmount() == b->getAmount()) return ascendOffer(a, b);
        else return false;
}