#ifndef __PRODUCT_HPP__
#define __PRODUCT_HPP__
#include <string>
#include <vector>

class Comment
{
private:
    std::string username;
    std::string comment;

public:
    Comment(std::string _name, std::string _comment) : username(_name), comment(_comment) {}
    std::string PrintComment();
};
class Product
{
protected:
    unsigned int productID;
    std::string name;
    std::vector<Comment> comments;

public:
    Product(unsigned int _id, std::string _name) : productID(_id), name(_name) {}
    virtual std::string printDetails() = 0;
    unsigned int getID() { return productID; }
    std::string getName() { return name; }
    void addToComments(Comment c);
    std::string printComments();
    virtual std::string printComparison(Product* p2) = 0;
};

class Mobile : public Product
{
private:
    std::string weight;
    std::string CPUFrequency;
    std::string internalStorage;
    std::string RAM;
    std::string screenResolution;
    std::string cameraResolution;
    std::string OS;
public:
    Mobile(unsigned int id, std::string n, std::string w, std::string cpu, std::string storage, std::string ram, 
        std::string screenres, std::string camerares, std::string os);
    std::string printDetails();
    std::string printComparison(Product* p2);
};

class Car : public Product
{
private:
    std::string weight;
    std::string numOfSeats;
    std::string numOfCylinders;
    std::string engineCapacity;
    std::string reverseParkingSensors;
public:
    Car(unsigned int id, std::string n,std::string w, std::string nos, 
        std::string noc, std::string ec, std::string reverse);
    std::string printDetails();
    std::string printComparison(Product* p2);
};

class TV : public Product
{
private:
    std::string screenSize;
    std::string screenType;
    std::string resolution;
    std::string _3D;
    std::string HDR;
public:
    TV(unsigned int id, std::string n, std::string size, std::string type, 
        std::string res, std::string _3d, std::string hdr);
    std::string printDetails();
    std::string printComparison(Product* p2);
};

#endif