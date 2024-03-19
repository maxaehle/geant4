#include "CommonHeader.h"

// -*- C++ -*-
// ----------------------------------------------------------------------

#include "CLHEP/Evaluator/Evaluator.h"

#include <cmath>	// for sqrt and pow

static G4double eval_abs  (G4double a)           { return (a < 0) ? -a : a; } 
static G4double eval_min  (G4double a, G4double b) { return (a < b) ?  a : b; } 
static G4double eval_max  (G4double a, G4double b) { return (a > b) ?  a : b; } 
static G4double eval_sqrt (G4double a)           { return std::sqrt(a); } 
static G4double eval_pow  (G4double a, G4double b) { return std::pow(a,b); } 
static G4double eval_sin  (G4double a)           { return std::sin(a); } 
static G4double eval_cos  (G4double a)           { return std::cos(a); } 
static G4double eval_tan  (G4double a)           { return std::tan(a); } 
static G4double eval_asin (G4double a)           { return std::asin(a); } 
static G4double eval_acos (G4double a)           { return std::acos(a); } 
static G4double eval_atan (G4double a)           { return std::atan(a); } 
static G4double eval_atan2(G4double a, G4double b) { return std::atan2(a,b); } 
static G4double eval_sinh (G4double a)           { return std::sinh(a); } 
static G4double eval_cosh (G4double a)           { return std::cosh(a); } 
static G4double eval_tanh (G4double a)           { return std::tanh(a); } 
static G4double eval_exp  (G4double a)           { return std::exp(a); } 
static G4double eval_log  (G4double a)           { return std::log(a); } 
static G4double eval_log10(G4double a)           { return std::log10(a); } 

namespace HepTool {

void Evaluator::setStdMath() {

  //   S E T   S T A N D A R D   C O N S T A N T S

  setVariable("pi",     3.14159265358979323846);
  setVariable("e",      2.7182818284590452354);
  setVariable("gamma",  0.577215664901532861);
  setVariable("radian", 1.0);
  setVariable("rad",    1.0);
  setVariable("degree", 3.14159265358979323846/180.);
  setVariable("deg",    3.14159265358979323846/180.);

  //   S E T   S T A N D A R D   F U N C T I O N S

  setFunction("abs",   eval_abs);
  setFunction("min",   eval_min);
  setFunction("max",   eval_max);
  setFunction("sqrt",  eval_sqrt);
  setFunction("pow",   eval_pow);
  setFunction("sin",   eval_sin);
  setFunction("cos",   eval_cos);
  setFunction("tan",   eval_tan);
  setFunction("asin",  eval_asin);
  setFunction("acos",  eval_acos);
  setFunction("atan",  eval_atan);
  setFunction("atan2", eval_atan2);
  setFunction("sinh",  eval_sinh);
  setFunction("cosh",  eval_cosh);
  setFunction("tanh",  eval_tanh);
  setFunction("exp",   eval_exp);
  setFunction("log",   eval_log);
  setFunction("log10", eval_log10);
}

} // namespace HepTool
