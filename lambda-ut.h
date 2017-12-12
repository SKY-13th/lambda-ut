#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>

namespace lambda_ut {

struct LutResult {
  bool        value;
  std::string data;
};

namespace __lambda_ut {
namespace {
  using namespace std;
  enum CaseState { RUN, PASS, FAIL };
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
  string formatErrorMsg(const string& file, const size_t line, const string& msg) {
    stringstream out;
    out << "In file: " << file << ":" << line << endl << msg;
    return out.str();
  }
} // namespace
  
  struct TestAssertion : public logic_error {
    TestAssertion(string what) : logic_error(move(what)) {};
  };
  struct TestData {
    const string name;
    unordered_map<string, bool> state;
    TestData( string name ) : name( move( name ) ) {}
    bool operator += ( LutResult&& result ) {
      state.emplace(make_pair(result.data, result.value));
    }
    size_t subCasesPassed() const {
      size_t passed = 0;
      for (const auto &s : state)
        passed += s.second;
      return passed;
    }
  };
  template<typename Functor>
  struct SuiteExecutor{
    TestData testData;
    Functor functor;
    SuiteExecutor( TestData&& testData, Functor&& functor )
      : testData( testData )
      , functor( functor ) {};
    LutResult operator()() {
      return testData << functor;
    }
  };
  template<typename Functor>
  LutResult operator<<( TestData& testData, Functor&& functor ) {
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
    return { result, testData.name };
  }
  template<typename Functor>
  LutResult operator<<( TestData&& testData, Functor &&functor ) {
    return testData << functor;
  }
  template<typename Functor>
  const auto operator|( TestData testData, Functor functor ) -> SuiteExecutor<Functor> {
    return SuiteExecutor<Functor>(
      std::move(testData), std::move(functor) );
  }
} // namespace __lambda_ut

namespace {
  template <typename T>
  std::string toString(const T& val) {
    std::stringstream out; out << val;
    return out.str();
  }
  template <>
  std::string toString<bool>(const bool& val) {
    return val ? "true" : "false";
  }
} // namespace

template <typename A, typename B>
LutResult Eq(const A& expect, const B& actual) {
  const bool result = expect == actual;
  return { result, result ? ""
      : "Expect: "   + toString(expect)
      + ", Actual: " + toString(actual) };
};
LutResult True (bool val) { return Eq(true, val); }
LutResult False(bool val) { return Eq(false, val); }
} // namespace lambda_ut
namespace __lut = lambda_ut::__lambda_ut;

#define LUTSUITE(NAME) auto __LUTSUITEe_##NAME = __lut::TestData(#NAME) | [&](__lut::TestData& __lutSpace)

#define LUTSUITE_RUN(NAME) __LUTSUITEe_##NAME();

#define LUTCASE(NAME) \
const bool __lutCase_##NAME = __lutSpace += __lut::TestData(__lutSpace.name +'.'+ #NAME) << [&](__lut::TestData& __lutSpace)

#define ASSERT(FUNCTOR, ...) {       \
const auto r = FUNCTOR(__VA_ARGS__); \
if(!r.value) throw __lut::TestAssertion(__lut::formatErrorMsg(__FILE__,__LINE__,r.data)); }
