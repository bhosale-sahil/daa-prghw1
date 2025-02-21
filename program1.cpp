#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cmath>
#include <chrono>

using namespace std;
using namespace std::chrono;

pair<int, vector<string>> max_profit(vector<string> &cards, vector<int> &prices, map<string, int> &market_file_read, int budget)
{

    int max_profit = 0;
    vector<string> best_subset;
    int n = cards.size();

    for (int i = 0; i < (pow(2, n)); i++)
    {
        vector<string> subset;
        int cost = 0;
        int profit = 0;

        for (int j = 0; j < n; j++)
        {
            if ((i >> j) & 1)
            {
                subset.push_back(cards[j]);
                cost += prices[j];
                profit += market_file_read.at(cards[j]) - prices[j];
            }
        }
        if (cost <= budget && profit > max_profit)
        {
            best_subset = subset;
            max_profit = profit;
        }
    }
    return {max_profit, best_subset};
}

int main(int arglen, char *args[])
{

    string mp_file, pl_file, out_file;
    for (int i = 1; i < arglen - 1; i++)
    {

        string arg = args[i];
        if (arg == "-m")
        {
            mp_file = args[++i];
        }
        else if (arg == "-p")
        {
            pl_file = args[++i];
        }
        else if (arg == "-o")
        {
            out_file = args[++i];
        }
    }

    ifstream market_file(mp_file);
    if (!market_file)
    {
        cerr << "Could not open market price file" << endl;
        return 1;
    }

    map<string, int> market_file_read;
    int num_market_cards;
    market_file >> num_market_cards;

    for (int i = 0; i < num_market_cards; i++)
    {
        string card;
        int price;
        market_file >> card >> price;
        market_file_read[card] = price;
    }
    market_file.close();

    ifstream price_file(pl_file);
    if (!price_file)
    {
        cerr << "Could not open price list file" << endl;
    }
    ofstream output_file(out_file);
    if (!output_file)
    {
        cerr << "Could not open output file" << endl;
    }

    while (true)
    {
        int n, W;
        price_file >> n >> W;
        if (price_file.eof())
        {
            break;
        }

        vector<string> cards;
        vector<int> prices;
        bool exist = true;

        for (int i = 0; i < n; i++)
        {
            string card;
            int price;
            price_file >> card >> price;

            if (market_file_read.find(card) == market_file_read.end())
            {
                cerr << card << "not found in market price" << endl;
                exist = false;
                break;
            }
            cards.push_back(card);
            prices.push_back(price);
        }
        if (!exist)
        {
            continue;
        }
        auto s = high_resolution_clock::now();
        auto profit = max_profit(cards, prices, market_file_read, W);
        auto e = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(e - s).count() / 1e6;

        output_file << n << " " << profit.first << " " << profit.second.size() << " " << duration << endl;
        for (string &card : profit.second)
        {
            output_file << card << endl;
        }
    }

    price_file.close();
    output_file.close();

    return 0;
}