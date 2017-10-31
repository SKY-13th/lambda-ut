#include "lambda-ut.h"

using namespace lambda_ut;

LUTSUITE(TestSuite) {
  LUTCASE(CaseA) {
    ASSERT(Eq, 100, 100);
  };
  LUTCASE(CaseB) {};
  LUTCASE(CaseC) {
    LUTCASE(NestedCaseA) {
      ASSERT(True, true);
    };
    LUTCASE(NestedCaseB) {
      ASSERT(False, true);
    };
  };
};

int main() {
  LUTSUITE_RUN(TestSuite);
  return 0;
}
