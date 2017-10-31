# lambda-ut

Single header, small, lambda-based, unit test framework for C++

## Usage:

**`LUTSUIT( NAME )`** - root node to a suit of test cases.
* **`NAME`** - name of a test suit.
```cpp
LUTSUIT(TestSuit) {
   // Test Cases...
}; //...
```
##

**`LUTSUIT_RUN( NAME )`** - macro to run related test suit.
* **`NAME`** - name of a test suit to run.
```cpp
int main() {
  LUTSUIT_RUN(TestSuit); // ...
  return 0;
}
```
##

**`LUTCASE( NAME )`** - test case which should be nested in test execution space.
* **`NAME`** - name of a test case.
```cpp
LUTSUIT(TestSuit) {
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

**`ASSERT(FUNCTOR, ...)`** - lumbda-ut assertion. Can be in both test cases and suits.
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
