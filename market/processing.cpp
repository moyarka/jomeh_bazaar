#include "market.hpp"
#include "misc_funcs.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

std::string Market::processCommand(std::string command)
{
    CommandLine cl = tokenizer(command, ' ');
    if(cl.size() == 0) throwNotFound();
    std::string selector = cl[0];
    if(selector == Get) return this->processGET(cl);
    else if(selector == Post) return this->processPOST(cl);
    else throwBadRequest();
}

std::string Market::processGET(CommandLine& cl)
{
    if(cl[1] == "offers") {
        if(this->checkIfCurrentUserIsSeller()) throwPermissionDenied();
        if(currentLoggedInUser == manager) this->handleOffersForManager(cl);
        else this->handleAllOffersForBuyer(cl);
    }

    else if(cl[1] == "offersOnProduct") {
        if(!this->checkIfCurrentUserIsBuyer()) throwBadRequest();
        return this->handleOffersForBuyer(cl);
    }

    else if(cl[1] == "getProducts") {
        if(currentLoggedInUser == manager) throwPermissionDenied();
        return this->printProducts();
    }

    else if(cl[1] == "myOffers") {
        if(!this->checkIfCurrentUserIsSeller()) throwPermissionDenied();
        Seller* seller = (Seller*)currentLoggedInUser;
        return seller->printOffers();
    }

    else if(cl[1] == "wallet") {
        if(currentLoggedInUser == manager) throwPermissionDenied();
        return this->printCreditHistory(cl);
    }

    else if(cl[1] == "productDetail") {
        if(this->checkIfCurrentUserIsAdmin()) throwPermissionDenied();
        return this->printProductForBuyer(cl);
    }

    else if(cl[1] == "comments") {
        if(!this->checkIfCurrentUserIsBuyer()) throwPermissionDenied();
        return this->printCommentsOnProduct(cl);
    }

    else if(cl[1] == "cart") {
        if(!this->checkIfCurrentUserIsBuyer()) throwPermissionDenied();
        return this->handlePrintCart();
    }

    else if(cl[1] == "compare") {
        if(!this->checkIfCurrentUserIsBuyer()) throwPermissionDenied();
        return this->printComaparison(cl);
    }

    else if(cl[1] == "orders") {
        if(!this->checkIfCurrentUserIsBuyer()) throwBadRequest();
        this->printOrders(cl);
    }

    else if(cl[1] == "offerBuyers") {
        if(!this->checkIfCurrentUserIsSeller()) throwBadRequest();
        this->printSells();
    }

    else throwNotFound();
}

std::string Market::processPOST(CommandLine& cl)
{
    if(cl[1] == "importProducts") {
        if(currentLoggedInUser == manager) return this->handleImport(cl);
        else throwPermissionDenied();
    }
    
    else if(cl[1] == "logout") {
        if(currentLoggedInUser == nullptr) throwPermissionDenied();
        if(this->checkIfCurrentUserIsBuyer()) return this->logoutForBuyer();
        else this->logout();
    }

    else if(cl[1] == "signup") {
        if(currentLoggedInUser != nullptr) {
            std::cout << "current logged in user is " << currentLoggedInUser->getName() << std::endl;
            throwPermissionDenied();
            }
        else return this->handleSignup(cl);
    }

    else if(cl[1] == "login") {
        if(currentLoggedInUser != nullptr) throwPermissionDenied();
        else return this->handleLogin(cl);
    }

    else if(cl[1] == "offer") {
        if(!this->checkIfCurrentUserIsSeller()) throwPermissionDenied();
        return this->addOffer(cl);
    }

    else if(cl[1] == "changeOffer") {
        if(!this->checkIfCurrentUserIsSeller()) throwPermissionDenied();
        this->changeOffer(cl);
    }

    else if(cl[1] == "generateDiscountCode") {
        if(!this->checkIfCurrentUserIsSeller()) throwPermissionDenied();
        this->handleDiscountCodes(cl);
    }

    else if(cl[1] == "comment") {
        if(!this->checkIfCurrentUserIsBuyer()) throwPermissionDenied();
        this->handleComment(cl);
    }

    else if(cl[1] == "addToCart") {
        if(!this->checkIfCurrentUserIsBuyer()) throwPermissionDenied();
        return this->handleAddingToCart(cl); 
    }

    else if(cl[1] == "chargeWallet") {
        if(!this->checkIfCurrentUserIsBuyer()) throwPermissionDenied();
        return this->handleWalletCharge(cl);
    }

    else if(cl[1] == "submitCart") {
        if(!this->checkIfCurrentUserIsBuyer()) throwBadRequest();
        return this->handleCartSubmission();
    }

    else if(cl[1] == "evaluateModel") {
        labeller.evaluateModel();
    }
    else throwNotFound();
    return "";
}