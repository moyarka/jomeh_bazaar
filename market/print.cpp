#include "market.hpp"
#include "misc_funcs.hpp"
#include "sort_methods.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

void Market::printAllOffers(std::string order, std::string field) 
{
    std::cout << "productId | offerId | offerUnitPrice | offerAmount" << std::endl;
    std::vector<Offer*> all_offers;
    for(int i = 0; i < sellers.size(); i++) {
        std::vector<Offer*> offers = sellers[i]->getOffers();
        all_offers.insert(all_offers.end(), offers.begin(), offers.end());
    }

    if(order == "ASCEND") {
        if(field == "offerId") std::sort(all_offers.begin(), all_offers.end(), ascendOffer);
        if(field == "productId") std::sort(all_offers.begin(), all_offers.end(), ascendProduct);
        if(field == "offerPrice") std::sort(all_offers.begin(), all_offers.end(), ascendPrice);
        if(field == "offerAmount") std::sort(all_offers.begin(), all_offers.end(), ascendAmount);
    }

    if(order == "DESCEND") {
        if(field == "offerId") std::sort(all_offers.begin(), all_offers.end(), descendOffer);
        if(field == "productId") std::sort(all_offers.begin(), all_offers.end(), descendProduct);
        if(field == "offerPrice") std::sort(all_offers.begin(), all_offers.end(), descendPrice);
        if(field == "offerAmount") std::sort(all_offers.begin(), all_offers.end(), descendAmount);
    }

    for(int i = 0; i < all_offers.size(); i++) {
        all_offers[i]->printOfferForSeller();
    }
}

bool compareIDs(Product* a, Product* b) {
    return (a->getID() < b->getID());
}

std::string Market::printProducts()
{
    std::string body;
    std::vector<Product*> all_products;
    all_products.insert(all_products.end(), mobiles.begin(), mobiles.end());
    all_products.insert(all_products.end(), tvs.begin(), tvs.end());
    all_products.insert(all_products.end(), cars.begin(), cars.end());
    if(all_products.size() == 0) {
        body += "<p style=\"font-size=16\">No Products Imported Yet</p>";
        return body;
    }
    std::sort(all_products.begin(), all_products.end(), compareIDs);
    body += "<form method=\"get\" action=\"/print_details\">";
    body += "<table>";
    body += "<th>Product ID</th><th>Product Name</th>";
    for(int i = 0; i < all_products.size(); i++) {
        body += "<tr>";
        body += "<td>" + std::to_string(all_products[i]->getID()) + "</td>";
        body += "<td><button type=\"submit\" name=\"product_id\" value=" + std::to_string(all_products[i]->getID()) + ">";
        body += all_products[i]->getName() + "</button></td>";
        body += "</tr>";
    }
    body += "</table></form>";
    return body;
}

std::string Market::printCreditHistory(CommandLine& cl)
{
    if(cl.size() != 5) throwBadRequest();
    if(cl[2] != "?") throwBadRequest();
    if(cl[3] != "count") throwBadRequest();
    return currentLoggedInUser->printCreditHistory(stoi(cl[4]));
}

std::string Market::printProductForBuyer(CommandLine& cl)
{
    if(cl.size() != 5) throwNotFound();
    if(cl[2] != "?") throwBadRequest();
    if(cl[3] != "productId") throwBadRequest();
    Product* product = this->findProduct(stoi(cl[4]));
    if(product == nullptr) throwNotFound();

    return product->printDetails();
}

std::string Market::printCommentsOnProduct(CommandLine& cl)
{
    if(cl.size() != 5) throwBadRequest();
    if(cl[2] != "?") throwBadRequest();
    if(cl[3] != "productId") throwBadRequest();
    int id = stoi(cl[4]);
    Product* product = this->findProduct(id);
    if(product == nullptr) throwBadRequest();

    return product->printComments();
}

std::string Market::handlePrintCart()
{
    std::string body;
    Buyer* buyer = (Buyer*)currentLoggedInUser;
    std::vector<BuyerOffer*> cart = buyer->getCart();
    if(cart.size() == 0) {
        body += "<br><br><p>No Item Is Added To Cart Yet</p>";
        return body;
    }
    
    body += "<table>";
    body += "<th>Product ID</th>";
    body += "<th>Product Name</th>";
    body += "<th>Offer ID</th>";
    body += "<th>Seller ID</th>";
    body += "<th>Total Price</th>";
    body += "<th>Amount</th>";
    long double totalPrice = 0;
    int size = cart.size();
    for(int i = 0; i < size; i++) {
        std::stringstream row;
        BuyerOffer* bo = cart[size - i - 1]; 
        row << "<tr>";
        row << "<td>" << std::to_string(bo->offer->getProductID()) << "</td>";
        row << "<td>" << this->findProduct(bo->offer->getProductID())->getName() << "</td>";
        row << "<td>" << std::to_string(bo->offer->getID()) << "</td>";
        row << "<td>" << std::to_string(bo->seller->getID()) << "</td>";
        row << "<td>" << std::setprecision(2) << std::fixed << bo->FinalPrice << "</td>";
        totalPrice += bo->FinalPrice;
        row << "<td>" << std::to_string(bo->amount) << "</td>";
        row << "</tr>";
        body += row.str();
    }
    body += "</table>";
    std::stringstream rest;
    rest <<  "<br><br><p>Total Price Of Your Reserved Items Is: ";
    rest << std::setprecision(2) << std::fixed << totalPrice << "</p>";
    body += rest.str();
    body += "<form method=\"post\" action=\"/view_cart\">";
    body += "<input type=\"submit\" value=\"Sumbit Cart\">";
    body += "</form>";
    return body;
}

std::string Market::printComaparison(CommandLine& cl)
{
    if(cl.size() != 7) throwBadRequest();
    if(cl[2] != "?") throwBadRequest();
    if(cl[3] != "productId1") throwBadRequest();
    if(cl[5] != "productId2") throwBadRequest();

    int id1 = stoi(cl[4]);
    int id2 = stoi(cl[6]);
    if(id1 > productIDIndexer || id2 > productIDIndexer) throwNotFound();
    foundProducts fp = this->findProductsForcomparison(id1, id2);
    return fp.first->printComparison(fp.second); 
}

void Market::printOrders(CommandLine& cl)
{
    if(cl.size() != 5) throwBadRequest();
    if(cl[2] != "?") throwBadRequest();
    if(cl[3] != "count") throwBadRequest();
    int count = stoi(cl[4]);

    std::cout << "productId | productName | offerId | sellerId | soldPrice | amount" << std::endl;
    Buyer* buyer = (Buyer*)currentLoggedInUser;
    std::vector<SoldItem*> si = buyer->getOrders();
    int size = si.size();
    for(int i = 0; i < size && i < count; i++) {
        int productid = si[size - i - 1]->getProductID();
        Product* p = this->findProduct(productid);
        std::cout << productid << " | ";
        std::cout << p->getName() << " | ";
        si[i]->printForBuyer();
        if(i != si.size() - 1) std::cout << "****\n";
    }
}

void Market::printSells()
{
    std::cout << "productId | productName | offerId | buyerId | soldPrice | amount" << std::endl;
    Seller* seller = (Seller*)currentLoggedInUser;
    std::vector<SoldItem*> si = seller->getSells();
    int size = si.size();
    for(int i = 0; i < size; i++) {
        int productid = si[size - i - 1]->getProductID();
        Product* p = this->findProduct(productid);
        std::cout << productid << " | ";
        std::cout << p->getName() << " | ";
        si[size - i - 1]->printForSeller();
    }
}