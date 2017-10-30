#pragma once

#include <string>
#include <functional>
#include <iostream>
#include <sstream>
#include <unordered_map>

namespace lambda_ut {
namespace __lambda_ut {
  using namespace std;
  using lutResult = std::pair<string, bool>;
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
    void printResult( bool result ) const {
      size_t succeeded = 0;
      if(state.empty()) {
        printCaseState(name, result ? SUCCESS : FAILURE);
        return;
      }
      for (const auto &s : state)
        succeeded += s.second;
      printCaseState(name, succeeded == state.size()
                         ? SUCCESS : FAILURE);
      cout << "          Succeeded "
           << succeeded    << " of "
           << state.size() << endl;
    }
  };
  template<typename Functor>
  lutResult operator<<( TestData& testData, Functor &&functor ) {
    printCaseState( testData.name, START );
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
    testData.printResult(result);
    return make_pair( testData.name, result );
  }
  template<typename Functor>
  lutResult operator<<( TestData&& testData, Functor &&functor ) {
    return testData << functor;
  }
  struct TestSuit : public TestData {
    function<void(TestData&)> scope;
    TestSuit(string name) : TestData(move(name)) {}
    lutResult operator()() {
      return *this << scope;
    }
    TestSuit operator <<=(function<void(TestData&)> scope) {
      this->scope = scope;
      return *this;
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

#define LUTSUIT(NAME) auto __lutSuit_##NAME = __lut::TestSuit(#NAME) <<= [&](__lut::TestData& __lutSpace)

#define LUTSUIT_RUN(NAME) __lutSuit_##NAME();

#define LUTCASE(NAME) \
const bool __lutCase_##NAME = __lutSpace += __lut::TestData(__lutSpace.name +'.'+ #NAME) << [&](__lut::TestData& __lutSpace)

#define ASSERT(FUNCTOR, ...) {       \
const auto r = FUNCTOR(__VA_ARGS__); \
if(!r.second) throw __lut::TestAssertion(__lut::formatErrorMsg(__FILE__,__LINE__,r.first)); }
