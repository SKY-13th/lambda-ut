#pragma once

#include <string>
#include <functional>
#include <iostream>
#include <sstream>

namespace lambda_ut {
namespace __lambda_ut {
  using namespace std;
  enum CaseState { START, SUCCESS, FAILURE };
  struct TestAssertion : public logic_error {
    TestAssertion(string what)
        : logic_error(move(what)) {};
  };
  void printCaseState(const string& caseName, CaseState state) {
    string stateStr;
    switch (state) {
      case   START: stateStr = "[ START ] "; break;
      case SUCCESS: stateStr = "[SUCCESS] "; break;
      case FAILURE: stateStr = "[FAILURE] "; break;
    } cout << stateStr << caseName << endl;
  }
  void printError(const string& caseName, const string& msg) {
    cout << "> ERROR:\n" << msg << endl;
    printCaseState(caseName, FAILURE);
  }
  string formatErrorMsg( const string& file, const size_t line, const string& msg ) {
    stringstream out;
    out << "In file: " << file << ":" << line << endl << msg;
    return out.str();
  }
  struct TestCase {
    const string name;
    TestCase( string name ) : name( move( name ) ) {}
    template<typename Functor>
    bool operator<<( Functor &&functor ) const {
      printCaseState( name, START );
      try {
        functor( name );
      } catch ( const TestAssertion& ex ) {
        printError( name, ex.what() );
        return false;
      } catch(...) {
        printError( name, "Unexpected exception" );
        throw;
      }
      printCaseState( name, SUCCESS );
      return true;
    }
  };
  template <typename T>
  std::string toString(const T& val) {
    stringstream out;
    out << val;
    return out.str();
  }
  template <>
  std::string toString<bool>(const bool& val) {
    return val ? "true" : "false";
  }
} // namespace __lambda_ut
using lutResult = std::pair<bool, std::string>;

template <typename A, typename B>
lutResult Eq(const A& expect, const B& actual) {
  const bool result = expect == actual;
  return std::make_pair( result , result ? ""
      : "Expect: "   + __lambda_ut::toString(expect)
      + ", Actual: " + __lambda_ut::toString(actual) );
};
lutResult True (bool val) { return Eq(true, val); }
lutResult False(bool val) { return Eq(false, val); }

} // namespace lambda_ut
namespace __lut = lambda_ut::__lambda_ut;

#define LUTROOT(NAME) const std::string __lutSpace = #NAME;

#define LUTSUIT(NAME) LUTROOT(NAME) \
const auto __lutSuit_##NAME = [&]()

#define LUTSUIT_RUN(NAME) __lutSuit_##NAME();

#define LUTCASE(NAME) \
const bool __lutCase_##NAME = __lut::TestCase(__lutSpace +'.'+ #NAME) << [&](const std::string& __lutSpace)

#define ASSERT(FUNCTOR, ...) {       \
const auto r = FUNCTOR(__VA_ARGS__); \
if(!r.first) throw __lut::TestAssertion(__lut::formatErrorMsg(__FILE__,__LINE__,r.second)); }
