// Simulating a random walk
// Written by Prof. Sreenivas for IE523: Financial Computing
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <random>
#include <chrono>
using namespace std;

double risk_free_rate, initial_stock_price, expiration_time, volatility;
int no_of_trials, no_of_divisions;

unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator;

double get_uniform()
{
    // http://www.cplusplus.com/reference/random/exponential_distribution/
    std::uniform_real_distribution<double> distribution(0.0,1.0);
    double number = distribution(generator);
    return (number);
}


int main (int argc, char* argv[])
{
    sscanf (argv[1], "%lf", &expiration_time);
    sscanf (argv[2], "%lf", &risk_free_rate);
    sscanf (argv[3], "%lf", &volatility);
    sscanf (argv[4], "%lf", &initial_stock_price);
    sscanf (argv[5], "%d", &no_of_divisions);
    ofstream outf(argv[6]); // 6th command line argument is the CSV output file name
    
    // chop expiration time into no_of_divisions many segments
    // figure out the motion within each segment
    double delta_T = expiration_time/((double) no_of_divisions);
    double delta_R = (risk_free_rate - 0.5*pow(volatility,2))*delta_T;
    double delta_SD = volatility*sqrt(delta_T);

    // by sharing random variables we create 4 paths
    double current_stock_price1 = initial_stock_price;
    double current_stock_price2 = initial_stock_price;
    double current_stock_price3 = initial_stock_price;
    double current_stock_price4 = initial_stock_price;
    for (int j = 0; j < no_of_divisions; j++)
    {
        
        outf << expiration_time/((double) no_of_divisions) * ((double) j) << ", " <<
        current_stock_price1 << ", " << current_stock_price2 << ", " <<
        current_stock_price3 << ", " << current_stock_price4 << endl;
        
        // create the unit normal variates using the Box-Muller Transform
        double x = get_uniform();
        double y = get_uniform();
        double a =  sqrt(-2.0*log(x)) * cos(6.283185307999998*y);
        double b =  sqrt(-2.0*log(x)) * sin(6.283185307999998*y);
        
        current_stock_price1 = current_stock_price1*exp(delta_R + delta_SD*a);
        current_stock_price2 = current_stock_price2*exp(delta_R - delta_SD*a);
        current_stock_price3 = current_stock_price3*exp(delta_R + delta_SD*b);
        current_stock_price4 = current_stock_price4*exp(delta_R - delta_SD*b);
    }
    
    cout << "--------------------------------" << endl;
    cout << "Sample Path Synthesis" << endl;
    cout << "Expiration Time (Years) = " << expiration_time << endl;
    cout << "Risk Free Interest Rate = " << risk_free_rate << endl;
    cout << "Volatility (%age of stock value) = " << volatility*100 << endl;
    cout << "Initial Stock Price = " << initial_stock_price << endl;
    cout << "Number of Steps/Divisions in the sample path = " << no_of_divisions << endl;
    cout << "The CSV output filename: " << argv[6] << endl;
    cout << "--------------------------------" << endl;
}
