#include "lambda-ut.h"

using namespace lambda_ut;

LUTSUIT(TestSuit) {
  LUTCASE(CaseA) {
    ASSERT(Eq, 100, 100);
  };
  LUTCASE(CaseB) {
    LUTCASE(NestedCaseA) {
      ASSERT(True, true);
    };
    LUTCASE(NestedCaseB) {
      ASSERT(False, false);
    };
  };
};

int main() {
  LUTSUIT_RUN(TestSuit);
  return 0;
}
