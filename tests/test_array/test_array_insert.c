#include <assert.h>
#include <errno.h>

#include "../include/array.h"

static void test_array_insert_into_empty(void) {
  Array* arr = array_init(sizeof(int));
  assert(arr);

  int value = 10;

  assert(array_insert(arr, &value, 0) == 0);

  assert(array_size(arr) == 1);

  int out;
  assert(array_get(arr, 0, &out) == 0);
  assert(out == value);

  array_delete(&arr);
}

static void test_array_insert_at_front(void) {
  Array* a = array_init(sizeof(int));
  assert(a);

  // arrange
  const int initial[] = {1, 2, 3, 4, 5};
  const size_t initial_n = sizeof(initial) / sizeof(initial[0]);

  for (size_t i = 0; i < initial_n; ++i) {
    assert(array_insert(a, &initial[i], i) == 0);
  }

  const size_t initial_size = array_size(a);

  // act
  int value = 100;

  // assert size increase
  assert(array_insert(a, &value, 0) == 0);

  // assert
  const int expect[] = {100, 1, 2, 3, 4, 5};
  const size_t expect_n = sizeof(expect) / sizeof(expect[0]);

  assert(array_size(a) == initial_size + 1);

  for (size_t i = 0; i < expect_n; ++i) {
    int out;
    assert(array_get(a, i, &out) == 0);
    assert(out == expect[i]);
  }

  array_delete(&a);
}

static void test_array_insert_in_middle(void) {
  Array* a = array_init(sizeof(int));
  assert(a);

  const int initial[] = {1, 2, 3, 4, 5};
  const size_t initial_n = sizeof(initial) / sizeof(initial[0]);

  for (size_t i = 0; i < initial_n; ++i) {
    assert(array_insert(a, &initial[i], i) == 0);
  }

  const size_t initial_size = array_size(a);

  int value = 100;
  assert(array_insert(a, &value, 3) == 0);

  const int expect[] = {1, 2, 3, 100, 4, 5};
  const size_t expect_n = sizeof(expect) / sizeof(expect[0]);

  assert(array_size(a) == initial_size + 1);

  for (size_t i = 0; i < expect_n; ++i) {
    int out;
    assert(array_get(a, i, &out) == 0);
    assert(out == expect[i]);
  }

  array_delete(&a);
}

static void test_array_insert_at_back(void) {
  Array* a = array_init(sizeof(int));
  assert(a);

  const int initial[] = {1, 2, 3, 4, 5};
  const size_t initial_n = sizeof(initial) / sizeof(initial[0]);

  for (size_t i = 0; i < initial_n; ++i) {
    assert(array_insert(a, &initial[i], i) == 0);
  }

  const size_t initial_size = array_size(a);

  int value = 100;
  assert(array_insert(a, &value, array_size(a)) == 0);

  const int expect[] = {1, 2, 3, 4, 5, 100};
  const size_t expect_n = sizeof(expect) / sizeof(expect[0]);

  assert(array_size(a) == initial_size + 1);

  for (size_t i = 0; i < expect_n; ++i) {
    int out;
    assert(array_get(a, i, &out) == 0);
    assert(out == expect[i]);
  }

  array_delete(&a);
}

static void test_array_insert_self_insertion(void) {
  Array* a = array_init(sizeof(int));
  assert(a);

  int value = 1;
  assert(array_insert(a, &value, 0) == 0);

  const int* p = array_data(a);

  int error = array_insert(a, p, 0);

  assert(error == EINVAL);

  array_delete(&a);
}

static void assert_array_invariants(const Array* arr) {
  assert(arr != NULL);
  assert(array_capacity(arr) >= array_size(arr));
}

static void test_array_insert_empty_at_zero(void) {
  Array* arr = array_init(sizeof(int));
  assert(arr != NULL);
  assert(array_size(arr) == 0);

  int val = 42;
  assert(array_insert(arr, &val, 0) == 0);
  assert(array_size(arr) == 1);

  int got = 0;
  assert(array_get(arr, 0, &got) == 0);
  assert(got == 42);

  assert(array_capacity(arr) >= 1);

  array_delete(&arr);
}

static void test_array_insert_at_end_on_empty(void) {
  Array* arr = array_init(sizeof(int));
  assert(arr != NULL);
  assert(array_size(arr) == 0);

  int val = 777;
  assert(array_insert(arr, &val, array_size(arr)) == 0);

  assert(array_size(arr) == 1);
  int got = 0;
  assert(array_get(arr, 0, &got) == 0);
  assert(got == 777);

  array_delete(&arr);
}

static void test_array_insert_single_element_middle(void) {
  Array* arr = array_init(sizeof(int));
  assert(arr != NULL);

  int vals[] = {10, 30};
  assert(array_push_back(arr, &vals[0]) == 0);
  assert(array_push_back(arr, &vals[1]) == 0);
  assert(array_size(arr) == 2);

  int insert_val = 20;
  assert(array_insert(arr, &insert_val, 1) == 0);

  assert(array_size(arr) == 3);

  int expected[] = {10, 20, 30};
  for (size_t i = 0; i < 3; ++i) {
    int got = 0;
    assert(array_get(arr, i, &got) == 0);
    assert(got == expected[i]);
  }

  array_delete(&arr);
}

static void test_array_insert_front_on_nonempty(void) {
  Array* arr = array_init(sizeof(int));
  assert(arr != NULL);

  int vals[] = {20, 30, 40};
  for (size_t i = 0; i < 3; ++i) {
    assert(array_push_back(arr, &vals[i]) == 0);
  }
  assert(array_size(arr) == 3);

  int insert_val = 10;
  assert(array_insert(arr, &insert_val, 0) == 0);

  assert(array_size(arr) == 4);

  int expected[] = {10, 20, 30, 40};
  for (size_t i = 0; i < 4; ++i) {
    int got = 0;
    assert(array_get(arr, i, &got) == 0);
    assert(got == expected[i]);
  }

  array_delete(&arr);
}

static void test_array_insert_back_equivalent_to_push(void) {
  Array* arr = array_init(sizeof(int));
  assert(arr != NULL);
  assert(array_size(arr) == 0);

  // fill using array_insert at end (should behave exactly as push_back)
  for (int i = 0; i < 25; ++i) {
    assert(array_insert(arr, &i, array_size(arr)) == 0);
  }

  assert(array_size(arr) == 25);

  // verify identical content as if push_back was used
  for (int i = 0; i < 25; ++i) {
    int val = -1;
    assert(array_get(arr, (size_t)i, &val) == 0);
    assert(val == i);
  }

  array_delete(&arr);
}

static void test_array_insert_middle_single(void) {
  Array* arr = array_init(sizeof(int));
  assert(arr != NULL);

  // initial data: 0 1 2 3 4 5 6 7 8 9
  for (int i = 0; i < 10; ++i) {
    assert(array_push_back(arr, &i) == 0);
  }

  size_t old_capacity = array_capacity(arr);

  // insert 999 exactly in the middle (index 5)
  int insert_val = 999;
  assert(array_insert(arr, &insert_val, 5) == 0);

  assert(array_size(arr) == 11);
  assert(array_capacity(arr) >= old_capacity);

  // expected: 0 1 2 3 4 999 5 6 7 8 9
  const int expected[] = {0, 1, 2, 3, 4, 999, 5, 6, 7, 8, 9};
  for (size_t i = 0; i < 11; ++i) {
    int val = -1;
    assert(array_get(arr, i, &val) == 0);
    assert(val == expected[i]);
  }

  array_delete(&arr);
}

static void test_array_insert_at_last_position(void) {
  Array* arr = array_init(sizeof(int));
  assert(arr != NULL);

  // fill with 1...5
  for (int i = 1; i <= 5; ++i) {
    assert(array_push_back(arr, &i) == 0);
  }

  int val = 999;
  // insert right before end (at position 4, becomes new last-1)
  assert(array_insert(arr, &val, 4) == 0);

  assert(array_size(arr) == 6);

  const int expected[] = {1, 2, 3, 4, 999, 5};
  for (size_t i = 0; i < 6; ++i) {
    int got = 0;
    assert(array_get(arr, i, &got) == 0);
    assert(got == expected[i]);
  }

  array_delete(&arr);
}

static void test_array_insert_when_full_capacity(void) {
  Array* arr = array_init(sizeof(int));
  assert(arr != NULL);

  size_t cap = array_capacity(arr);

  // fill exactly to capacity
  for (size_t i = 0; i < cap; ++i) {
    assert(array_push_back(arr, &i) == 0);
  }

  assert(array_size(arr) == cap);
  assert(array_capacity(arr) == cap);

  int val = -777;
  // insert at beginning must trigger growth
  assert(array_insert(arr, &val, 0) == 0);

  assert(array_size(arr) == cap + 1);
  assert(array_capacity(arr) > cap);

  int got = 0;
  assert(array_get(arr, 0, &got) == 0);
  assert(got == -777);

  // check last element still correct
  assert(array_get(arr, cap, &got) == 0);
  assert(got == (int)(cap - 1));

  array_delete(&arr);
}

static void test_array_insert_triggers_realloc(void) {
  Array* arr = array_init(sizeof(int));
  assert(arr != NULL);

  size_t init_cap = array_capacity(arr);

  // fill to capacity
  for (size_t i = 0; i < init_cap; ++i) {
    assert(array_push_back(arr, &i) == 0);
  }

  assert(array_size(arr) == init_cap);
  assert(array_capacity(arr) == init_cap);

  int val = 999;
  assert(array_insert(arr, &val, init_cap) == 0);

  assert(array_size(arr) == init_cap + 1);
  assert(array_capacity(arr) > init_cap);

  array_delete(&arr);
}

static void test_array_insert_no_change_enough_space(void) {
  Array* arr = array_init(sizeof(int));
  assert(arr != NULL);

  size_t init_cap = array_capacity(arr);

  // fill leaving 3 free slots
  for (size_t i = 0; i < init_cap - 3; ++i) {
    assert(array_push_back(arr, &i) == 0);
  }

  assert(array_capacity(arr) == init_cap);

  int val = -1;
  assert(array_insert(arr, &val, 0) == 0);                // front
  assert(array_insert(arr, &val, array_size(arr)) == 0);  // back
  assert(array_insert(arr, &val, 5) == 0);                // middle

  assert(array_size(arr) == init_cap - 3 + 3);
  assert(array_capacity(arr) == init_cap);  // no growth

  array_delete(&arr);
}

static void test_array_insert_back(void) {
  Array* arr = array_init(sizeof(int));
  assert(arr != NULL);

  // empty array initially
  assert_array_invariants(arr);

  size_t initial_capacity = array_capacity(arr);

  // insert 20 elements at the back using insert at current size
  for (int i = 0; i < 20; ++i) {
    assert(array_insert(arr, &i, array_size(arr)) == 0);
    assert(array_size(arr) == (size_t)i + 1);
  }

  // verify preserved order and values after multiple reallocations
  for (int i = 0; i < 20; ++i) {
    int value = -1;
    assert(array_get(arr, (size_t)i, &value) == 0);
    assert(value == i);
  }

  // capacity must have grown beyond initial value
  assert(array_capacity(arr) > initial_capacity);

  array_delete(&arr);
}

static void test_array_insert_front(void) {
  Array* arr = array_init(sizeof(int));
  assert(arr != NULL);

  // empty array initially
  assert_array_invariants(arr);

  size_t initial_capacity = array_capacity(arr);

  // values to insert at front (order in array will be reversed)
  const int values[] = {5, 4, 3, 2, 1};
  const int n = sizeof(values) / sizeof(values[0]);

  // repeatedly insert at position 0
  for (int i = 0; i < n; ++i) {
    assert(array_insert(arr, &values[i], 0) == 0);
    assert(array_size(arr) == (size_t)(i + 1));
  }

  // verify reversed order after all front insertions
  for (int i = 0; i < n; ++i) {
    int out = 0;
    assert(array_get(arr, (size_t)i, &out) == 0);
    assert(out == values[n - i - 1]);
  }

  // capacity should not shrink (at least initial)
  assert(array_capacity(arr) >= initial_capacity);

  array_delete(&arr);
}

void run_array_insert_tests(void) {
  test_array_insert_into_empty();
  test_array_insert_at_front();
  test_array_insert_in_middle();
  test_array_insert_at_back();
  test_array_insert_self_insertion();
  test_array_insert_empty_at_zero();
  test_array_insert_at_end_on_empty();
  test_array_insert_single_element_middle();
  test_array_insert_front_on_nonempty();
  test_array_insert_back_equivalent_to_push();
  test_array_insert_middle_single();
  test_array_insert_at_last_position();
  test_array_insert_when_full_capacity();
  test_array_insert_triggers_realloc();
  test_array_insert_no_change_enough_space();
  test_array_insert_back();
  test_array_insert_front();
}
