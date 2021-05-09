#include "product.hpp"
#include <iostream>
#include <sstream>
std::string Comment::PrintComment()
{
    std::string body;
    body += "<tr>";
    body += "<td>" + username + "</td>";
    body += "<td>" + comment + "</td>";
    body += "</tr>";
    return body;
}

Mobile::Mobile(unsigned int id, std::string n, std::string w, std::string cpu, std::string storage, std::string ram, 
    std::string screenres, std::string camerares, std::string os) : Product(id, n) 
{
    weight = w; 
    CPUFrequency = cpu; 
    internalStorage = storage;
    screenResolution = screenres; 
    cameraResolution = camerares;
    OS = os;
} 

Car::Car(unsigned int id, std::string n,std::string w, std::string nos, 
    std::string noc, std::string ec, std::string reverse) : Product(id, n) 
{
    weight = w; 
    numOfCylinders = noc; 
    numOfSeats = nos; 
    engineCapacity = ec; 
    reverseParkingSensors = reverse;
}

TV::TV(unsigned int id, std::string n, std::string size, std::string type, 
    std::string res, std::string _3d, std::string hdr) : Product(id, n) 
{
    screenSize = size;
    screenType = type;
    resolution = res;
    _3D = _3d;
    HDR = hdr;    
}

std::string Mobile::printDetails()
{
    std::string body;
    body += "<br><h1>" + name + "</h1>";
    body += "<table>";
    body += "<tr><td>Weight</td><td>" + weight + "</td></tr>";
    body += "<tr><td>CPUFrequency</td><td>" + CPUFrequency + "</td></tr>";
    body += "<tr><td>Built-in Storage</td><td>" + internalStorage + "</td></tr>";
    body += "<tr><td>Screen Resolution</td><td>" + screenResolution + "</td></tr>";
    body += "<tr><td>Camera Resolution</td><td>" + cameraResolution + "</td></tr>";
    body += "<tr><td>OS</td><td>" + OS + "</td></tr>";
    body += "</table>";
    return body;    
}

std::string TV::printDetails()
{
    std::string body;
    body += "<br><h1>" + name + "</h1>";
    body += "<table>";
    body += "<tr><td>Screen Size</td><td>" + screenSize + "</td></tr>";
    body += "<tr><td>Screen Type</td><td>" + screenType + "</td></tr>";
    body += "<tr><td>Resolution</td><td>" + resolution + "</td></tr>";
    body += "<tr><td>3D</td><td>" + _3D + "</td></tr>";
    body += "<tr><td>HDR</td><td>" + HDR + "</td></tr>";
    body += "</table>";
    return body;
}

std::string Car::printDetails()
{
    std::string body;
    body += "<br><h1>" + name + "</h1>";
    body += "<table>";
    body += "<tr><td>Weight</td><td>" + weight + "</td></tr>";
    body += "<tr><td>Num. of Seats</td><td>" + numOfSeats + "</td></tr>";
    body += "<tr><td>Num. of Cylinders</td><td>" + numOfCylinders + "</td></tr>";
    body += "<tr><td>Engine Capacity</td><td>" + engineCapacity + "</td></tr>";
    body += "<tr><td>Reverse Parking Sensors</td><td>" + reverseParkingSensors + "</td></tr>";
    body += "</table>";
    return body;
}

void Product::addToComments(Comment c)
{
    comments.push_back(c);
}

std::string Product::printComments()
{
    std::string body;
    int size = comments.size();
    if(size == 0) {
        body = "<p>No Comments Are Posted On This Product</p>";
        return body;
    }
    body += "<table>";
    body += "<th>Username</th><th>Comment</th>";
    for(int i = 0; i < size; i++) {
        body += comments[size - i - 1].PrintComment();
    }
    body += "</table>";
    return body;
}

std::string Mobile::printComparison(Product* p2)
{
    std::string body;
    body += "<table>";
    body += "<th>Feature</th><th>Product 1</th><th>Product 2</th>";
    Mobile m2 = *(Mobile*)p2;
    std::stringstream row;
    row << "<tr><td></td><td>" << name << "</td><td>" << m2.name << "</td></tr>";
    row << "<tr><td>Weight</td><td>" << weight << "</td><td>" << m2.weight << "</td></tr>";
    row << "<tr><td>CPU Frequency</td><td>" << CPUFrequency << "</td><td>" << m2.CPUFrequency << "</td></tr>";
    row << "<tr><td>Built-in Storage</td><td>" << internalStorage << "</td><td>" << m2.internalStorage << "</td></tr>";
    row << "<tr><td>Screen Resolution</td><td>" << screenResolution << "</td><td>" << m2.screenResolution << "</td></tr>";
    row << "<tr><td>Camera Resolution</td><td>" << cameraResolution << "</td><td>" << m2.cameraResolution << "</td></tr>";
    row << "<tr><td>OS</td><td>" << OS << "</td><td>" << m2.OS << "</td></tr>";
    body += row.str();
    body += "</table>";
    return body;
}

std::string TV::printComparison(Product* p2) 
{
    std::string body;
    body += "<table>";
    body += "<th>Feature</th><th>Product 1</th><th>Product 2</th>";
    TV t2 = *(TV*)p2;
    std::stringstream row;
    row << "<tr><td></td><td>" << name << "</td><td>" << t2.name << "</td></tr>";
    row << "<tr><td>Screen Size</td><td>" << screenSize << "</td><td>" << t2.screenSize << "</td></tr>";
    row << "<tr><td>screenType</td><td>" << screenType << "</td><td>" << t2.screenType << "</td></tr>";
    row << "<tr><td>Resolution</td><td>" << resolution << "</td><td>" << t2.resolution << "</td></tr>";
    row << "<tr><td>3D</td><td>" << _3D << "</td><td>" << t2._3D << "</td></tr>";
    row << "<tr><td>HDR</td><td>" << HDR << "</td><td>" << t2.HDR << "</td></tr>";
    body += row.str();
    body += "</table>";
    return body;
}

std::string Car::printComparison(Product* p2) 
{   
    std::string body;
    body += "<table>";
    body += "<th>Feature</th><th>Product 1</th><th>Product 2</th>";
    Car c2 = *(Car*)p2;
    std::stringstream row;
    row << "<tr><td></td><td>" << name << "</td><td>" << c2.name << "</td></tr>";
    row << "<tr><td>Weight</td><td>" << weight << "</td><td>" << c2.weight << "</td></tr>";
    row << "<tr><td>Num. of Seats</td><td>" << numOfSeats << "</td><td>" << c2.numOfSeats << "</td></tr>";
    row << "<tr><td>Num. of Cylinders</td><td>" << numOfCylinders << "</td><td>" << c2.numOfCylinders << "</td></tr>";
    row << "<tr><td>Engine Capacity</td><td>" << engineCapacity << "</td><td>" << c2.engineCapacity << "</td></tr>";
    row << "<tr><td>Reverse Parking Sensors</td><td>" << reverseParkingSensors << "</td><td>" << c2.reverseParkingSensors << "</td></tr>"; 
    body += row.str();
    body += "</table>";
    return body;
}