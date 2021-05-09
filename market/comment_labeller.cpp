#include "comment_labeller.hpp"
#include "misc_funcs.hpp"

void Comment_Labeller::evaluateModel()
{
    int EVAL_GOOD = 0, EVAL_BAD = 0;
    int REAL_GOOD = 0, REAL_BAD = 0;
    int CORRECT_GOOD = 0, CORRECT_BAD = 0;

    for(int i = 0; i < samples.size(); i++) {
        int label = stoi(samples[i].second);
        std::string comment = samples[i].first;
        std::vector<std::string> words = tokenizer(comment, ' ');
        std::vector<long double> good_ones, bad_ones;
        for(int j = 0; j < words.size(); j++) {
            if(words[j] != "")
            {
                auto c = word_data.find(words[j]);
                if(c != word_data.end()) {
                    good_ones.push_back(c->second.first);
                    bad_ones.push_back(c->second.second);
                }
            }
        }

        int eval_label = this->labelComment(good_ones, bad_ones, total_good_prob);
        if(eval_label == 0) {
            EVAL_GOOD += 1;
            if(label == 0) CORRECT_GOOD += 1;
        }

        if(eval_label == 1) {
            EVAL_BAD += 1;
            if(label == 1) CORRECT_BAD += 1;
        }
        if(label == 0) REAL_GOOD += 1;
        if(label == 1) REAL_BAD += 1;
    }
    std::ofstream output(output_dir);
    output << "Recall: ";
    output << (int)((double)CORRECT_GOOD / REAL_GOOD * 100) << std::endl;
    output << "Precision: ";
    output << (int)((double)CORRECT_GOOD / EVAL_GOOD * 100) << std::endl;
    output << "Accuracy: ";
    output << (int)((double)(CORRECT_GOOD + CORRECT_BAD) / (REAL_GOOD + REAL_BAD) * 100) << std::endl;
}

void Comment_Labeller::makeMap() 
{
    std::ifstream file(train_dir);
    std::string line;
    getline(file, line);
    while (getline(file, line))
    {
        std::vector<std::string> tokens = tokenizer(line, ',');
        word_data[tokens[0]] = std::make_pair(std::stold(tokens[1]), std::stold(tokens[2]));
    }

    std::ifstream prob(class_prob_dir);
    if(prob.good()) {
        getline(prob, line);
        total_good_prob = stold(line);
    }
}

void Comment_Labeller::readSamples()
{
    std::string line;
    std::ifstream test(test_dir);
    getline(test, line);
    while(getline(test, line)) {
        std::vector<std::string> pieces = tokenizer(line, ',');
        samples.push_back(std::make_pair(pieces[0], pieces[1]));    
    }
}

int Comment_Labeller::labelComment(const Probs& good_ones,const Probs& bad_ones, long double total_good_prob)
{
    int size = good_ones.size();
    int good_zero_count = 0, bad_zero_count = 0;
    for(int i = 0; i < size; i++) {
        if(good_ones[i] == 0.0) good_zero_count += 1;
        if(bad_ones[i] == 0.0) bad_zero_count += 1;
    }

    if(good_zero_count > bad_zero_count) return 1;
    if(good_zero_count < bad_zero_count) return 0;
    long double good_prob = 0, bad_prob = 0;
    for(int i = 0; i < size; i++) {
        if(good_ones[i] != 0) good_prob += log(good_ones[i]);
        if(bad_ones[i] != 0) bad_prob += log(bad_ones[i]);
    }
    good_prob += log(total_good_prob);
    bad_prob += log(1 - total_good_prob);
    if(good_prob > bad_prob) return 0;
    else if(good_prob < bad_prob) return 1;
    else return -1;
}

Comment_Labeller::Comment_Labeller()
{
    this->makeMap();
    this->readSamples();
}

int Comment_Labeller::checkIfCommentCanBePosted(std::string comment) {
    std::vector<std::string> words = tokenizer(comment, ' ');
    std::vector<long double> good_ones, bad_ones;
    for(int j = 0; j < words.size(); j++) {
        if(words[j] != "")
        {
            auto c = word_data.find(words[j]);
            if(c != word_data.end()) {
                good_ones.push_back(c->second.first);
                bad_ones.push_back(c->second.second);
            }
        }
    }
    return this->labelComment(good_ones, bad_ones, total_good_prob);
}
