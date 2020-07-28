
#include <iostream>
#include "Random1.h"
#include <cmath>

using namespace std;

double SimpleMonteCarlo1(double Expiry,
                         double Strike,
                         double Spot,
                         double Vol,
                         double r,
                         unsigned long NumberofPaths);

int main()
{
    double Expiry, Strike, Spot, Vol, r;
    unsigned long NumberOfPath;

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

    cout << "\n Number of paths\n";
    cin >> NumberOfPath;

    double result = SimpleMonteCarlo1(Expiry,
                                      Strike,
                                      Spot,
                                      Vol,
                                      r,
                                      NumberOfPath);

    cout << "the price is "<< result << "\n";
    double tmp;
    cin >>tmp;

    return 0;
}




double SimpleMonteCarlo1(double Expiry,
                         double Strike,
                         double Spot,
                         double Vol,
                         double r,
                         unsigned long NumberofPaths){ //unsigned changes the min value to 0

    double variance = Vol*Vol*Expiry;
    double rootVariance = sqrt(variance); // to scale our BM step
    double itoCorrection = -0.5*variance; // simplify our discount exponent

    //create variables to store execute price, running total, and discounted price
    double movedSpot = Spot*exp(r*Expiry + itoCorrection);
    double thisSpot;
    double runningSum = 0;

    for (unsigned long i = 0; i < NumberofPaths; i++){
        double thisGaussian = GetOneGaussianByBoxMuller();
        thisSpot = movedSpot*exp(rootVariance*thisGaussian);
        double thisPayoff = thisSpot - Strike;
        thisPayoff = thisPayoff > 0 ? thisPayoff : 0; //this is a one line if-else
        runningSum += thisPayoff;
    }

    double mean = runningSum / NumberofPaths;
    mean *= exp(-r*Expiry); //discount
    return mean;

}
