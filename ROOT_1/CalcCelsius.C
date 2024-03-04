// K. Rusiecka 08.10.2020
// Pracownia Specjalistyczna
// Na podstawie wykładu "Podstawy programowania
// - język C z elementami C++", prof. dr hab. W. Płaczek

#include <iostream>
using namespace std;

Bool_t CalcCelsius(Double_t fahr = 100){ 
  
 cout << "This function recalculates temperature from Fahrenheit degrees to Celsius degrees" << endl;

 Double_t cels = (5./9.)*(fahr - 32.);	//formula to recalculate degF to degC

 cout << fahr << " deg F = " << cels << " deg C" << endl;

 return kTRUE; 
}

Bool_t CalcFahrenheit(Double_t cels = 28.0){

 cout << "This function recalculates temperature from Celsius degrees to Fahrenheit degrees" << endl;

 Double_t fahr = 32. + (9./5.)*cels;	//formula to recalculate degC to degF

 cout << cels << " deg C = " << fahr << " deg F" << endl;

 return kTRUE;
}

