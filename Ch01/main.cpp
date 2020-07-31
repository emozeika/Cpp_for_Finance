
#include <iostream>
#include "Random1.h"
#include <cmath>
#include <string>

using namespace std;


double Payoff(string OptionType, double Strike, double Spot);
double SimpleMonteCarlo1(double Expiry,
                         double Strike,
                         double Spot,
                         double Vol,
                         double r,
                         unsigned long NumberofPaths,
                         string OptionType);
double DoubleDigitMC1(double Expiry,
                      double StrikeLow,
                      double StrikeHigh,
                      double Spot,
                      double Vol,
                      double r,
                      unsigned long NumberofPaths);


int main()
{
    double Expiry, Strike, Spot, Vol, r;
    unsigned long NumberOfPath;
    string OptionType;

    cout << "\nEnter expiry\n";
    cin >> Expiry;

    cout << "\nEnter Strike\n";
    cin >> Strike;

    cout << "\nEnter spot\n";
    cin >> Spot;

    cout << "\nEnter vol\n";
    cin >> Vol;

    cout << "\nEnter r\n";
    cin >> r;

    cout << "\nNumber of paths\n";
    cin >> NumberOfPath;

    cout << "\nWhat type of option is this? \n";
    cin >> OptionType;

    if(OptionType == "ddigit"){
        double Strike2;
        cout << "\nWhat is the other Strike \n";
        cin >> Strike2;
        double result = DoubleDigitMC1(Expiry, Strike, Strike2, Spot, Vol, r, NumberOfPath);
        cout << "the price is "<< result << "\n";
        double tmp;
        cin >>tmp;
    }
    else{
        double result = SimpleMonteCarlo1(Expiry,
                                          Strike,
                                          Spot,
                                          Vol,
                                          r,
                                          NumberOfPath,
                                          OptionType);

        cout << "the price is "<< result << "\n";
        double tmp;
        cin >>tmp;


    }
    return 0;
}


double Payoff(string OptionType, double Strike, double Spot){
    if(OptionType == "call"){
        double payoff = Spot - Strike;
        return payoff = payoff > 0 ? payoff : 0;
    }
    else if (OptionType == "put"){
        double payoff = Strike - Spot;
        return payoff = payoff > 0 ? payoff : 0;
    }
    else{
        cout << "\n invalid option type \n";
        return -1;
    }
}

double SimpleMonteCarlo1(double Expiry,
                         double Strike,
                         double Spot,
                         double Vol,
                         double r,
                         unsigned long NumberofPaths,
                         string OptionType){ //unsigned changes the min value to 0

    double variance = Vol*Vol*Expiry;
    double rootVariance = sqrt(variance); // to scale our BM step
    double itoCorrection = -0.5*variance; // simplify our discount exponent

    //create variables to store execute price, running total, and discounted price
    double movedSpot = Spot*exp(r*Expiry + itoCorrection);
    double thisSpot;
    double runningSum = 0;
    double thisPayoff;

    for (unsigned long i = 0; i < NumberofPaths; i++){
        double thisGaussian = GetOneGaussianByBoxMuller();
        thisSpot = movedSpot*exp(rootVariance*thisGaussian);
        if(OptionType == "call"){
            thisPayoff = thisSpot - Strike;
        }
        else if (OptionType == "put"){
            thisPayoff = Strike - thisSpot;
        }
        else{
            cout<< "\ninvalid option type \n";
            return -1;
        }
        thisPayoff = thisPayoff > 0 ? thisPayoff : 0; //this is a one line if-else

        //thisPayoff = Payoff(isCall, Strike, thisSpot);
        runningSum += thisPayoff;
    }

    double mean = runningSum / NumberofPaths;
    mean *= exp(-r*Expiry); //discount
    return mean;

}


double DoubleDigitMC1(double Expiry,
                      double StrikeLow,
                      double StrikeHigh,
                      double Spot,
                      double Vol,
                      double r,
                      unsigned long NumberofPaths){

    double variance = Vol*Vol*Expiry;
    double rootVariance = sqrt(variance); // to scale our BM step
    double itoCorrection = -0.5*variance; // simplify our discount exponent

    //create variables to store execute price, running total, and discounted price
    double movedSpot = Spot*exp(r*Expiry + itoCorrection);
    double thisSpot;
    double runningSum = 0;
    double thisPayoff;

    for (unsigned long i = 0; i < NumberofPaths; i++){
        double thisGaussian = GetOneGaussianByBoxMuller();
        thisSpot = movedSpot*exp(rootVariance*thisGaussian);
        if(thisSpot > StrikeLow && thisSpot < StrikeHigh){
            thisPayoff = 1;
        }
        else{
            thisPayoff = 0;
        }
        runningSum += thisPayoff;
    }

    double mean = runningSum / NumberofPaths;
    mean *= exp(-r*Expiry); //discount
    return mean;
}
