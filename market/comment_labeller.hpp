#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <iomanip>
#include <cmath>

const std::string train_dir = "Data/train.csv";
const std::string test_dir = "Data/test.csv";
const std::string class_prob_dir = "Data/class probabilities";
const std::string output_dir = "output.txt";

typedef std::unordered_map<std::string, std::pair<long double, long double>> Word_Data;
typedef std::vector<std::pair<std::string, std::string>> Samples;
typedef std::vector<long double> Probs;

class Comment_Labeller
{
private:
    Word_Data word_data;
    Samples samples;
    long double total_good_prob;

    void makeMap();
    void readSamples();
    int labelComment(const Probs& good_ones,const Probs& bad_ones, long double total_good_prob);
    
public:
    Comment_Labeller();
    void evaluateModel();
    int checkIfCommentCanBePosted(std::string comment);
};