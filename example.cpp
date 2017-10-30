#include "lambda-ut.h"

using namespace lambda_ut;

LUTSUIT(TestSuitA) {
  LUTCASE(CaseA) {
    ASSERT(Eq, 100, 100);
  };
  LUTCASE(CaseB) {};
};

void testSuitB() {
  LUTROOT(TestSuitB);

  LUTCASE(CaseA) {
    LUTCASE(NestedCaseA) {
      ASSERT(True, true);
    };

    LUTCASE(NestedCaseB) {
      ASSERT(False, false);
    };
  };
}

int main() {
  LUTSUIT_RUN(TestSuitA);
  testSuitB();
  return 0;
}
