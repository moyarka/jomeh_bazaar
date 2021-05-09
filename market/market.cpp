#include "market.hpp"
#include "misc_funcs.hpp"
#include "sort_methods.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

#define COMMA ','

std::string Market::logout()
{
    currentLoggedInUser = nullptr;
    return printSuccessMessage();
}

std::string Market::handleImport(CommandLine& cl)
{
    if(cl.size() != 7) throwBadRequest();
    if(cl[3] != "type") throwBadRequest();
    if(cl[4] != "mobile" && cl[4] != "tv" && cl[4] != "car") throwBadRequest();
    if(cl[5] != "filePath") throwBadRequest();

    std::string fileName = cl[6];
    std::ifstream file(fileName.c_str());
    if(!file.good()) throwNotFound();
    else {
        std::string dataLine;
        getline(file, dataLine);
        while(getline(file, dataLine)) {
            productData pd = tokenizer(dataLine, COMMA);
            if(cl[4] == "mobile") {
                Mobile* mobile = new Mobile(productIDIndexer, pd[0], pd[1], pd[2], pd[3], 
                    pd[4], pd[5], pd[6], pd[7]);
                mobiles.push_back(mobile);
            }

            if(cl[4] == "tv") {
                TV* tv = new TV(productIDIndexer, pd[0], pd[1], pd[2], pd[3], 
                    pd[4], pd[5]);
                tvs.push_back(tv);
            }

            if(cl[4] == "car") {
                Car* car = new Car(productIDIndexer, pd[0], pd[1], pd[2], pd[3], 
                    pd[4], pd[5]);
                cars.push_back(car);
            }

            productIDIndexer += 1;        
        }
    }

    return printSuccessMessage();
}

std::string Market::handleSignup(CommandLine& cl)
{
    std::string userType = "buyer";
    if(cl.size() != 9 && cl.size() != 11) throwBadRequest();
    if(cl[2] != "?") throwBadRequest();
    if(cl[3] != "email") throwBadRequest;
    if(cl[5] != "username") throwBadRequest();
    if(cl[7] != "password") throwBadRequest();
    if(cl.size() == 11) {
        if(cl[9] != "type") throwBadRequest();
        if(cl[10] != "buyer" && cl[10] != "seller") throwBadRequest();
        userType = cl[10];
    }
    checkIfUserExists(cl[4], cl[6]);
    if(userType == "buyer") {
        Buyer* buyer = new Buyer(UserIDIndexer, cl[4], cl[6], compute_hash(cl[8]));
        buyers.push_back(buyer);
        currentLoggedInUser = buyer;
    }
    
    else {
        Seller* seller = new Seller(UserIDIndexer, cl[4], cl[6], compute_hash(cl[8]));
        sellers.push_back(seller);
        currentLoggedInUser = seller;
    }

    UserIDIndexer += 1;

    return printSuccessMessage();
}

std::string Market::handleLogin(CommandLine& cl)
{
    if(cl.size() != 7) throwBadRequest();
    if(cl[2] != "?") throwBadRequest();
    if(cl[3] != "email") throwBadRequest();
    if(cl[5] != "password") throwBadRequest();
    User* user = this->findUserForLogin(cl[4], cl[6]);
    if(user == nullptr) throwBadRequest();
    
    currentLoggedInUser = user;

    return printSuccessMessage();
}

void Market::handleOffersForManager(CommandLine& cl)
{
    if(cl.size() != 7) throwBadRequest();
    if(cl[2] != "?") throwBadRequest();
    if(cl[3] != "order") throwBadRequest();
    if(cl[5] != "field") throwBadRequest();
    std::string order = cl[4];
    std::string field = cl[6];
    if(order != "ASCEND" && order != "DESCEND") throwBadRequest;
    if(field != "offerId" && field != "offerPrice")
        throwBadRequest();
    this->printAllOffers(order, field);
}

std::string Market::handleOffersForBuyer(CommandLine& cl) 
{
    if(cl.size() != 5) throwBadRequest();
    if(cl[2] != "?") throwBadRequest();
    if(cl[3] != "productId") throwBadRequest();

    int productid = stoi(cl[4]);
    Product* product = this->findProduct(productid);
    if(product == nullptr) throwNotFound();

    std::vector<Offer*> relevantOffers = this->findOffersOnProduct(productid);
    std::sort(relevantOffers.begin(), relevantOffers.end(), ascendOffer);
    std::string body;
    if(relevantOffers.size() == 0) {
        body += "<p>No Offers Have Been Posted On This Product</p>";
        return body;
    }
    body += "<table>";
    body += "<th>Offer ID</th>";
    body += "<th>Offer Unit Price</th>";
    body += "<th>Offer Amount</th>";
    body += "<th>Add To Cart</th>";
    for(int i = 0; i < relevantOffers.size(); i++) {
        Offer* o = relevantOffers[i];
        std::stringstream row;
        row << "<tr>";
        row << "<td>" << std::to_string(o->getID()) << "</td>";
        row << "<td>" << std::setprecision(2) << std::fixed << o->getPrice() << "</td>";
        row << "<td>" << std::to_string(o->getAmount()) << "</td>";
        row << "<td>" << "<form method=\"get\" action=\"/add_to_cart\">";
        row << "<label for=\"amount\">Amount: </label>";
        row << "<input type=\"number\" name=\"amount\"min=\"1\" max=\"" << std::to_string(o->getAmount()) << "\"";
        if(o->getAmount() == 0) row << "disabled";
        row << ">";
        row << "<button type=\"submit\" class=\"ordinary\" name=\"offer_id\" value=\"" << o->getID() << "\"";
        if(o->getAmount() == 0) row << "disabled";
        row << "> Submit</button>";
        row << "</td></form></tr>";
        body += row.str();
    }
    body += "</table>";
    return body;
}

void Market::handleAllOffersForBuyer(CommandLine& cl) 
{
    this->printAllOffers("ASCEND", "offerId");
}


std::string Market::addOffer(CommandLine& cl)
{
    if(cl.size() != 9) throwBadRequest();
    if(cl[2] != "?") throwBadRequest();
    if(cl[3] != "productId") throwBadRequest();
    if(cl[5] != "offerUnitPrice") throwBadRequest();
    if(cl[7] != "offerAmount") throwBadRequest();

    unsigned int productid = stoi(cl[4]);
    if(!this->checkIfProductIDIsValid(productid)) throwBadRequest();
    
    Seller* seller = (Seller*)currentLoggedInUser;
    if(!seller->checkIfOfferCanBeAdded(productid)) throwBadRequest();
    Offer* o = new Offer(stoi(cl[4]), OfferIDIndexer, stoi(cl[8]), stod(cl[6]));
    seller->AddToOffers(o);
    OfferIDIndexer += 1;

    return printSuccessMessage();
}

void Market::changeOffer(CommandLine& cl)
{
    if(cl.size() != 9) throwBadRequest();
    if(cl[2] != "?") throwBadRequest();
    if(cl[3] != "offerId") throwBadRequest();
    if(cl[5] != "offerUnitPrice") throwBadRequest();
    if(cl[7] != "offerAmount") throwBadRequest();
    Seller* seller = (Seller*)currentLoggedInUser;
    if(seller->findOffer(stoi(cl[4])) == nullptr) throwBadRequest();
    seller->changeOffer(stoi(cl[4]), stod(cl[6]), stoi(cl[8]));

    printSuccessMessage();
}

std::string Market::generateDiscountCode()
{
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    while(true)
    {
        int length = 4 + rand() % 4;
        std::string code(length, '?');
        for (int i = 0; i < length; i++) {
            code[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
        }
        if(this->checkIfDiscountCodeisValid(code)) {
            return code;
        }
    }
}

void Market::handleDiscountCodes(CommandLine& cl)
{
    if(cl.size() != 9) throwBadRequest();
    if(cl[2] != "?") throwBadRequest();
    if(cl[3] != "offerId") throwBadRequest();
    if(cl[5] != "discountPercent") throwBadRequest();
    if(cl[7] != "discountNumber") throwBadRequest();

    Offer* offer = this->findOffer(stoi(cl[4]));
    if(offer == nullptr) throwNotFound();

    double disPercent = stod(cl[6]);
    int disAmount = stoi(cl[8]);
    for(int i = 0; i < disAmount; i++) {
        std::string code = this->generateDiscountCode();
        offer->addToDiscountCodes(DiscountCode(disPercent, code));
    }

    std::vector<DiscountCode> codes = offer->getCodes();
    for(int i = 0; i < codes.size(); i++) {
        std::cout << codes[i].code << std::endl;
    }
}

void Market::handleComment(CommandLine& cl)
{
    if(cl.size() <= 7) throwBadRequest();
    if(cl[2] != "?") throwBadRequest();
    if(cl[3] != "productId") throwBadRequest();
    if(cl[5] != "comment") throwBadRequest();

    int id = stoi(cl[4]);
    Product* product = this->findProduct(id);
    if(product == nullptr) throwNotFound();

    std::string comment;
    for(int i = 6; i < cl.size(); i++) {
        if(i == cl.size() - 1) comment += cl[i];
        else comment += cl[i] + ' ';
    }

    if(labeller.checkIfCommentCanBePosted(comment) == 1) {
        throwBadRequest();
    }

    std::string username = currentLoggedInUser->getName();
    product->addToComments(Comment(username, comment));
    
    printSuccessMessage();
}

std::string Market::handleAddingToCart(CommandLine& cl)
{
    if(cl.size() != 7 && cl.size() != 9) throwBadRequest();
    if(cl[2] != "?") throwBadRequest();
    if(cl[3] != "offerId") throwBadRequest();
    if(cl[5] != "amount") throwBadRequest();
    int offerid = stoi(cl[4]);
    int amount = stoi(cl[6]);

    std::string code = "";
    if(cl.size() == 9) {
        if(cl[7] != "discountCode") throwBadRequest();
        code = cl[8];
    }
    Offer* offer;
    Seller* seller;
    for(int i = 0; i < sellers.size(); i++) {
        offer = sellers[i]->findOffer(offerid);
        if(offer != nullptr) {
            seller = sellers[i];
            break;
        }
    }

    if(offer == nullptr || seller == nullptr) throwNotFound();

    if(amount > offer->getAmount()) throwBadRequest();

    DiscountCode disCode(0, "");

    if(code != "") {
        if(!offer->isThisMyDiscountCode(code)) throwBadRequest();
        disCode = offer->getDiscountCode(code);
    }
    Buyer* buyer = (Buyer*)currentLoggedInUser;
    double finalPrice = amount * offer->getPrice() * (1.00 - (double)disCode.discountPercent / 100.00);
    BuyerOffer* bo = new BuyerOffer(offer, seller, amount, finalPrice);

    buyer->addToCart(bo);
    offer->updateAmount(amount);
    return printSuccessMessage();
}

std::string Market::handleWalletCharge(CommandLine& cl)
{
    if(cl.size() != 5) throwBadRequest();
    if(cl[2] != "?") throwBadRequest();
    if(cl[3] != "amount") throwBadRequest();
    double amount = stod(cl[4]);
    if(amount <= 0) throwBadRequest();
    currentLoggedInUser->updateCredit(amount);

    return printSuccessMessage();
}

std::string Market::logoutForBuyer()
{
    Buyer* buyer = (Buyer*)currentLoggedInUser;
    buyer->emptyCart();
    return this->logout();
}

std::string Market::handleCartSubmission()
{
    Buyer* buyer = (Buyer*)currentLoggedInUser;
    buyer->submitCart();
    return printSuccessMessage();
}