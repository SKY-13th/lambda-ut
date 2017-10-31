#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>

namespace lambda_ut {
namespace __lambda_ut {
  using namespace std;
  using lutResult = std::pair<string, bool>;
  enum CaseState { RUN, PASS, FAIL };
  struct TestAssertion : public logic_error {
    TestAssertion(string what)
        : logic_error(move(what)) {};
  };
  void printCaseState(const string& caseName, CaseState state) {
    string stateStr;
    switch (state) {
      case  RUN: stateStr = "[ RUN] "; break;
      case PASS: stateStr = "[PASS] "; break;
      case FAIL: stateStr = "[FAIL] "; break;
    } cout << stateStr << caseName << endl;
  }
  void printError(const string& caseName, const string& msg) {
    cout << "> ERROR: " << msg << "\n\n";
  }
  void printPassed(size_t passed, size_t ofCount) {
    cout << "       Passed " << passed << " of " << ofCount << endl;
  }
  string formatErrorMsg( const string& file, const size_t line, const string& msg ) {
    stringstream out;
    out << "In file: " << file << ":" << line << endl << msg;
    return out.str();
  }
  struct TestData {
    const string name;
    unordered_map<string, bool> state;
    TestData( string name ) : name( move( name ) ) {}
    bool operator += ( lutResult&& result ) {
      state.emplace(result);
    }
    size_t subCasesPassed() const {
      size_t passed = 0;
      for (const auto &s : state)
        passed += s.second;
      return passed;
    }
  };
  template<typename Functor>
  lutResult operator<<( TestData& testData, Functor&& functor ) {
    printCaseState( testData.name, RUN );
    bool result = true;
    try {
      functor( testData );
    } catch ( const TestAssertion& ex ) {
      printError( testData.name, ex.what() );
      result = false;
    } catch(...) {
      printError( testData.name, "Unexpected exception!!!" );
      result = false;
    }
    const size_t passed  = testData.subCasesPassed();
    const size_t count = testData.state.size();
    result &= passed == count;
    printCaseState( testData.name, result ? PASS : FAIL );
    if (passed < count) printPassed(passed, count);
    return make_pair( testData.name, result );
  }
  template<typename Functor>
  lutResult operator<<( TestData&& testData, Functor &&functor ) {
    return testData << functor;
  }
  template<typename Functor>
  auto operator|( TestData&& testData, Functor&& functor ) {
    return [=]() mutable {
      return testData << functor;
    };
  }
  template <typename T>
  std::string toString(const T& val) {
    stringstream out; out << val;
    return out.str();
  }
  template <>
  std::string toString<bool>(const bool& val) {
    return val ? "true" : "false";
  }
} // namespace __lambda_ut
using __lambda_ut::lutResult;

template <typename A, typename B>
lutResult Eq(const A& expect, const B& actual) {
  const bool result = expect == actual;
  return std::make_pair( result ? ""
      : "Expect: "   + __lambda_ut::toString(expect)
      + ", Actual: " + __lambda_ut::toString(actual), result );
};
lutResult True (bool val) { return Eq(true, val); }
lutResult False(bool val) { return Eq(false, val); }

} // namespace lambda_ut
namespace __lut = lambda_ut::__lambda_ut;

#define LUTSUIT(NAME) auto __lutSuit_##NAME = __lut::TestData(#NAME) | [&](__lut::TestData& __lutSpace)

#define LUTSUIT_RUN(NAME) __lutSuit_##NAME();

#define LUTCASE(NAME) \
const bool __lutCase_##NAME = __lutSpace += __lut::TestData(__lutSpace.name +'.'+ #NAME) << [&](__lut::TestData& __lutSpace)

#define ASSERT(FUNCTOR, ...) {       \
const auto r = FUNCTOR(__VA_ARGS__); \
if(!r.second) throw __lut::TestAssertion(__lut::formatErrorMsg(__FILE__,__LINE__,r.first)); }
