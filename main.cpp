// Implementation of the introductory example in Longstaff and Schwartz's paper
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include "/Users/sreenivas/Documents/Courses/IE523/newmat10/newmat.h"

double risk_free_rate, strike_price, initial_stock_price, expiration_time, volatility;
int no_of_trials, no_of_divisions;

double max(double a, double b) {
    return (b < a )? a:b;
}

double get_uniform()
{
    return (((float) random())/(pow(2.0, 31.0)-1.0));
}

Matrix polynomial_regression(Matrix Independent_Variables, Matrix Dependent_Variable, int order, int no_of_observations)
{
    Matrix X(no_of_observations, order);
    Matrix Y(no_of_observations, 1);
    
    for (int i = 1; i <= no_of_observations; i++)
        Y(i,1) = Dependent_Variable(i,1);
    
    for (int j = 1; j <= order; j++)
        for (int i = 1; i <= no_of_observations; i++)
            X(i,j) = pow(Independent_Variables(i,1), j-1);
    
    // return inv(XT*X)*XT*Y
    Matrix X_transpose_times_X(order, order);
    X_transpose_times_X = X.t()*X;
    return (X_transpose_times_X.i() * X.t() * Y);
}



int main (int argc, char* argv[])
{
    double asset_price[500][1000];

    for (int i = 0; i < 8; i++)
        asset_price[i][0] = 1.0;
    
    // The Longstaff and Schwartz paper had eight sample paths that were for 3 steps
    // I am just filling the relevant values.
    asset_price[0][1] = 1.09; asset_price[0][2] = 1.08; asset_price[0][3] = 1.34;
    asset_price[1][1] = 1.16; asset_price[1][2] = 1.26; asset_price[1][3] = 1.54;
    asset_price[2][1] = 1.22; asset_price[2][2] = 1.07; asset_price[2][3] = 1.03;
    asset_price[3][1] = 0.93; asset_price[3][2] = 0.97; asset_price[3][3] = 0.92;
    asset_price[4][1] = 1.11; asset_price[4][2] = 1.56; asset_price[4][3] = 1.52;
    asset_price[5][1] = 0.76; asset_price[5][2] = 0.77; asset_price[5][3] = 0.90;
    asset_price[6][1] = 0.92; asset_price[6][2] = 0.84; asset_price[6][3] = 1.01;
    asset_price[7][1] = 0.88; asset_price[7][2] = 1.22; asset_price[7][3] = 1.34;
    
    no_of_trials = 8;
    no_of_divisions = 3;
    double R = 1.0/0.94176;
    strike_price = 1.1;
    
    double value[no_of_trials];
    // initialize the value based on the price at final stage
    for (int i = 0; i < no_of_trials; i++) {
        value[i] = max(0.0, strike_price - asset_price[i][no_of_divisions]);
        cout << "Stage: " << no_of_divisions << "\t value[" << i << "] = " << value[i] << endl;
    }
    cout << "-------------------" << endl;
    
    for (int i = (no_of_divisions-1); i > 0; i--)
    {
        Matrix independent_variables(no_of_trials,1);
        Matrix dependent_variables(no_of_trials,1);
        int no_of_variables = 0;
        for (int j = 0; j < no_of_trials; j++)
        {
            if (max(0.0, strike_price - asset_price[j][i]) > 0)
            {
                no_of_variables++;
                independent_variables(no_of_variables, 1) = asset_price[j][i];
                dependent_variables(no_of_variables, 1) = value[j]/R;
            }
        }
        
        // regressing the dependent_variables on the independent variables using a 3rd order polynomial
        Matrix a(3,1);
        a = polynomial_regression(independent_variables, dependent_variables, 3, no_of_variables);
        cout << "a[1] = " << a(1,1) << "\t a[2] = " << a(2,1) <<  "\t a[3] = " << a(3,1) << endl;
        for (int j = 0; j < no_of_trials; j++)
        {
            if ( (strike_price - asset_price[j][i]) > (a(1,1) + (a(2,1)*asset_price[j][i]) + (a(3,1)*asset_price[j][i]*asset_price[j][i])) &&
                ( (strike_price -asset_price[j][i]) > 0.0 ) )
                value[j] = strike_price - asset_price[j][i];
            else
                value[j] = value[j]/R;
        }
        
        for (int j = 0; j < no_of_trials; j++)
            cout << "Stage: " << i << "\t value[" << j << "] = " << value[j] << endl;
        cout << "-------------------" << endl;
    }

        
    double put_price = 0.0;
    for (int j = 0; j < no_of_trials; j++)
        put_price += value[j];
    cout << "Put Price = " << (put_price/((float) no_of_trials))/R << endl;
}
