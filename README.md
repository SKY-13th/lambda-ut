# lambda-ut

Single header, small, lambda-based, unit test framework for C++

## Usage:

**`LUTSUITE( NAME )`** - root node to a suite of test cases.
* **`NAME`** - name of a test suite.
```cpp
LUTSUITE(TestSuite) {
   // Test Cases...
}; //...
```
##

**`LUTSUITE_RUN( NAME )`** - macro to run related test suite.
* **`NAME`** - name of a test suite to run.
```cpp
int main() {
  LUTSUITE_RUN(TestSuite); // ...
  return 0;
}
```
##

**`LUTCASE( NAME )`** - test case which should be nested in test execution space.
* **`NAME`** - name of a test case.
```cpp
LUTSUITE(TestSuite) {
  // Test execution space
  LUTCASE(TestCase) {
     // ...
  }; // ...
};
```
**`LUTCASE()`** by itself is an test execution space, so that they can be nested in other test cases. 
> **Note:** If test case have at least one nested, on end of its execution, it will print count of passed tests in it.

> **Note:** If at least one of nested tests will fail, parent will be marked as failed to.
```cpp
// Test execution space
LUTCASE(TestCase) {
  LUTCASE(NestedCaseA) {
     // ...
  };
  LUTCASE(NestedCaseB) {
     // ...
  }; // ...
};   // ...
```
##

**`ASSERT(FUNCTOR, ...)`** - lumbda-ut assertion. Can be in both test cases and suites.
* **`FUNCTOR`** - a callable entity(lambda, function pointer, functional object) which return lutResult.
* **`...`** - list of args, which will be passed to **`FUNCTOR`**
```cpp
// ...
ASSERT(Eq, 100, 100); // ...
```
##

**`lutResult`** - result of assertion functor, represented by pair of string and boolean.
* **`first`**  - string with info about assertion failure if failed.
* **`second`** - actual result of assertion.
##

**`Eq`** - functor for assertion. Return whether 2 objects have same value.
* **`expect`** - value which is expected
* **`actual`** - actual value
* **return**: **`lutResult`** with result of comparison
##

**`True/False`** - functor for assertion. Return whether boolean value is `true/false`
* **`val`** - value for validation
* **return**: **`lutResult`** with result of comparison
##
