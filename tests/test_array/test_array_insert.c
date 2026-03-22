#include <assert.h>
#include <errno.h>

#include "../include/array.h"

#ifdef __GNUC__
  #define MAYBE_UNUSED __attribute__((unused))
#else
  #define MAYBE_UNUSED
#endif

static void assert_array_invariants(const Array* array MAYBE_UNUSED) {
  assert(array != NULL);
  assert(array_capacity(array) >= array_size(array));
}

static void test_array_insert_into_empty(void) {
  Array* array = array_init(sizeof(int));
  assert(array);

  int value MAYBE_UNUSED = 10;

  assert(array_insert(array, &value, 0) == 0);

  assert(array_size(array) == 1);

  int out MAYBE_UNUSED;
  assert(array_get(array, 0, &out) == 0);
  assert(out == value);

  array_delete(&array);
}

static void test_array_insert_at_front(void) {
  /// ARRANGE

  Array* array = array_init(sizeof(int));
  assert(array);
  assert_array_invariants(array);

  const size_t initial_capacity  MAYBE_UNUSED = array_capacity(array);

  /// ACT

  const int values[] = {1, 2, 3, 4, 5};
  const size_t n = sizeof(values) / sizeof(values[0]);

  for (size_t i = 0; i < n; ++i) {
    assert(array_insert(array, &values[i], 0) == 0);

    // size increase
    assert(array_size(array) == i + 1);

    // invariants must hold
    assert_array_invariants(array);
  }

  /// ASSERT

  for (size_t i = 0; i < n; ++i) {
    int out MAYBE_UNUSED;

    assert(array_get(array, i, &out) == 0);
    assert(out == values[n - i - 1]);
  }

  /// ACT

  int v MAYBE_UNUSED = 100;

  const size_t old_size MAYBE_UNUSED = array_size(array);

  assert(array_insert(array, &v, 0) == 0);

  /// ASSERT

  assert(array_size(array) == old_size + 1);

  int out MAYBE_UNUSED;
  assert(array_get(array, 0, &out) == 0);
  assert(out == v);

  /// ASSERT

  for (size_t i = 1; i < array_size(array); ++i) {
    int current MAYBE_UNUSED = 0;
    int previous MAYBE_UNUSED = 0;

    assert(array_get(array, i, &current) == 0);
    assert(array_get(array, i - 1, &previous) == 0);

    // compare with original reversed sequence
    assert(current == values[n - i]);
  }

  /// ASSERT

  assert(array_capacity(array) >= initial_capacity);

  assert_array_invariants(array);

  array_delete(&array);
}

static void test_array_insert_in_middle(void) {
  Array* array = array_init(sizeof(int));
  assert(array);

  const int initial[] = {1, 2, 3, 4, 5};
  const size_t initial_n = sizeof(initial) / sizeof(initial[0]);

  for (size_t i = 0; i < initial_n; ++i) {
    assert(array_insert(array, &initial[i], i) == 0);
  }

  const size_t initial_size MAYBE_UNUSED = array_size(array);

  int value MAYBE_UNUSED = 100;
  assert(array_insert(array, &value, 3) == 0);

  const int expect[] = {1, 2, 3, 100, 4, 5};
  const size_t expect_n = sizeof(expect) / sizeof(expect[0]);

  assert(array_size(array) == initial_size + 1);

  for (size_t i = 0; i < expect_n; ++i) {
    int out MAYBE_UNUSED;
    assert(array_get(array, i, &out) == 0);
    assert(out == expect[i]);
  }

  array_delete(&array);
}

static void test_array_insert_at_back(void) {
  Array* array = array_init(sizeof(int));
  assert(array);

  const int initial[] = {1, 2, 3, 4, 5};
  const size_t initial_n = sizeof(initial) / sizeof(initial[0]);

  for (size_t i = 0; i < initial_n; ++i) {
    assert(array_insert(array, &initial[i], i) == 0);
  }

  const size_t initial_size MAYBE_UNUSED = array_size(array);

  int value MAYBE_UNUSED = 100;
  assert(array_insert(array, &value, array_size(array)) == 0);

  const int expect[] = {1, 2, 3, 4, 5, 100};
  const size_t expect_n = sizeof(expect) / sizeof(expect[0]);

  assert(array_size(array) == initial_size + 1);

  for (size_t i = 0; i < expect_n; ++i) {
    int out MAYBE_UNUSED;
    assert(array_get(array, i, &out) == 0);
    assert(out == expect[i]);
  }

  array_delete(&array);
}

static void test_array_insert_self_insertion(void) {
  Array* array = array_init(sizeof(int));
  assert(array);

  int value MAYBE_UNUSED = 1;
  assert(array_insert(array, &value, 0) == 0);

  const int* p = array_data(array);

  int error MAYBE_UNUSED = array_insert(array, p, 0);

  assert(error == EINVAL);

  array_delete(&array);
}

// static void test_array_insert_empty_at_zero(void) {
//   Array* array = array_init(sizeof(int));
//   assert(array != NULL);
//   assert(array_size(array) == 0);

//   int val = 42;
//   assert(array_insert(array, &val, 0) == 0);
//   assert(array_size(array) == 1);

//   int got = 0;
//   assert(array_get(array, 0, &got) == 0);
//   assert(got == 42);

//   assert(array_capacity(array) >= 1);

//   array_delete(&array);
// }

// static void test_array_insert_at_end_on_empty(void) {
//   Array* array = array_init(sizeof(int));
//   assert(array != NULL);
//   assert(array_size(array) == 0);

//   int val = 777;
//   assert(array_insert(array, &val, array_size(array)) == 0);

//   assert(array_size(array) == 1);
//   int got = 0;
//   assert(array_get(array, 0, &got) == 0);
//   assert(got == 777);

//   array_delete(&array);
// }

// static void test_array_insert_single_element_middle(void) {
//   Array* array = array_init(sizeof(int));
//   assert(array != NULL);

//   int vals[] = {10, 30};
//   assert(array_push_back(array, &vals[0]) == 0);
//   assert(array_push_back(array, &vals[1]) == 0);
//   assert(array_size(array) == 2);

//   int insert_val = 20;
//   assert(array_insert(array, &insert_val, 1) == 0);

//   assert(array_size(array) == 3);

//   int expected[] = {10, 20, 30};
//   for (size_t i = 0; i < 3; ++i) {
//     int got = 0;
//     assert(array_get(array, i, &got) == 0);
//     assert(got == expected[i]);
//   }

//   array_delete(&array);
// }

// static void test_array_insert_front_on_nonempty(void) {
//   Array* array = array_init(sizeof(int));
//   assert(array != NULL);

//   int vals[] = {20, 30, 40};
//   for (size_t i = 0; i < 3; ++i) {
//     assert(array_push_back(array, &vals[i]) == 0);
//   }
//   assert(array_size(array) == 3);

//   int insert_val = 10;
//   assert(array_insert(array, &insert_val, 0) == 0);

//   assert(array_size(array) == 4);

//   int expected[] = {10, 20, 30, 40};
//   for (size_t i = 0; i < 4; ++i) {
//     int got = 0;
//     assert(array_get(array, i, &got) == 0);
//     assert(got == expected[i]);
//   }

//   array_delete(&array);
// }

// static void test_array_insert_back_equivalent_to_push(void) {
//   Array* array = array_init(sizeof(int));
//   assert(array != NULL);
//   assert(array_size(array) == 0);

//   // fill using array_insert at end (should behave exactly as push_back)
//   for (int i = 0; i < 25; ++i) {
//     assert(array_insert(array, &i, array_size(array)) == 0);
//   }

//   assert(array_size(array) == 25);

//   // verify identical content as if push_back was used
//   for (int i = 0; i < 25; ++i) {
//     int val = -1;
//     assert(array_get(array, (size_t)i, &val) == 0);
//     assert(val == i);
//   }

//   array_delete(&array);
// }

// static void test_array_insert_middle_single(void) {
//   Array* array = array_init(sizeof(int));
//   assert(array != NULL);

//   // initial data: 0 1 2 3 4 5 6 7 8 9
//   for (int i = 0; i < 10; ++i) {
//     assert(array_push_back(array, &i) == 0);
//   }

//   size_t old_capacity = array_capacity(array);

//   // insert 999 exactly in the middle (index 5)
//   int insert_val = 999;
//   assert(array_insert(array, &insert_val, 5) == 0);

//   assert(array_size(array) == 11);
//   assert(array_capacity(array) >= old_capacity);

//   // expected: 0 1 2 3 4 999 5 6 7 8 9
//   const int expected[] = {0, 1, 2, 3, 4, 999, 5, 6, 7, 8, 9};
//   for (size_t i = 0; i < 11; ++i) {
//     int val = -1;
//     assert(array_get(array, i, &val) == 0);
//     assert(val == expected[i]);
//   }

//   array_delete(&array);
// }

// static void test_array_insert_at_last_position(void) {
//   Array* array = array_init(sizeof(int));
//   assert(array != NULL);

//   // fill with 1...5
//   for (int i = 1; i <= 5; ++i) {
//     assert(array_push_back(array, &i) == 0);
//   }

//   int val = 999;
//   // insert right before end (at position 4, becomes new last-1)
//   assert(array_insert(array, &val, 4) == 0);

//   assert(array_size(array) == 6);

//   const int expected[] = {1, 2, 3, 4, 999, 5};
//   for (size_t i = 0; i < 6; ++i) {
//     int got = 0;
//     assert(array_get(array, i, &got) == 0);
//     assert(got == expected[i]);
//   }

//   array_delete(&array);
// }

// static void test_array_insert_when_full_capacity(void) {
//   Array* array = array_init(sizeof(int));
//   assert(array != NULL);

//   size_t cap = array_capacity(array);

//   // fill exactly to capacity
//   for (size_t i = 0; i < cap; ++i) {
//     assert(array_push_back(array, &i) == 0);
//   }

//   assert(array_size(array) == cap);
//   assert(array_capacity(array) == cap);

//   int val = -777;
//   // insert at beginning must trigger growth
//   assert(array_insert(array, &val, 0) == 0);

//   assert(array_size(array) == cap + 1);
//   assert(array_capacity(array) > cap);

//   int got = 0;
//   assert(array_get(array, 0, &got) == 0);
//   assert(got == -777);

//   // check last element still correct
//   assert(array_get(array, cap, &got) == 0);
//   assert(got == (int)(cap - 1));

//   array_delete(&array);
// }

// static void test_array_insert_triggers_realloc(void) {
//   Array* array = array_init(sizeof(int));
//   assert(array != NULL);

//   size_t init_cap = array_capacity(array);

//   // fill to capacity
//   for (size_t i = 0; i < init_cap; ++i) {
//     assert(array_push_back(array, &i) == 0);
//   }

//   assert(array_size(array) == init_cap);
//   assert(array_capacity(array) == init_cap);

//   int val = 999;
//   assert(array_insert(array, &val, init_cap) == 0);

//   assert(array_size(array) == init_cap + 1);
//   assert(array_capacity(array) > init_cap);

//   array_delete(&array);
// }

// static void test_array_insert_no_change_enough_space(void) {
//   Array* array = array_init(sizeof(int));
//   assert(array != NULL);

//   size_t init_cap = array_capacity(array);

//   // fill leaving 3 free slots
//   for (size_t i = 0; i < init_cap - 3; ++i) {
//     assert(array_push_back(array, &i) == 0);
//   }

//   assert(array_capacity(array) == init_cap);

//   int val = -1;
//   assert(array_insert(array, &val, 0) == 0);
//   assert(array_insert(array, &val, array_size(array)) == 0);
//   assert(array_insert(array, &val, 5) == 0);

//   assert(array_size(array) == init_cap - 3 + 3);
//   assert(array_capacity(array) == init_cap);  // no growth

//   array_delete(&array);
// }

// static void test_array_insert_back(void) {
//   Array* array = array_init(sizeof(int));
//   assert(array != NULL);

//   assert_array_invariants(array);

//   size_t initial_capacity = array_capacity(array);

//   // insert 20 elements at the back using insert at current size
//   for (int i = 0; i < 20; ++i) {
//     assert(array_insert(array, &i, array_size(array)) == 0);
//     assert(array_size(array) == (size_t)i + 1);
//   }

//   // verify preserved order and values after multiple reallocations
//   for (int i = 0; i < 20; ++i) {
//     int value = -1;
//     assert(array_get(array, (size_t)i, &value) == 0);
//     assert(value == i);
//   }

//   // capacity must have grown beyond initial value
//   assert(array_capacity(array) > initial_capacity);

//   array_delete(&array);
// }

void run_array_insert_tests(void) {
  test_array_insert_into_empty();
  test_array_insert_at_front();
  test_array_insert_in_middle();
  test_array_insert_at_back();
  test_array_insert_self_insertion();
  // test_array_insert_empty_at_zero();
  // test_array_insert_at_end_on_empty();
  // test_array_insert_single_element_middle();
  // test_array_insert_front_on_nonempty();
  // test_array_insert_back_equivalent_to_push();
  // test_array_insert_middle_single();
  // test_array_insert_at_last_position();
  // test_array_insert_when_full_capacity();
  // test_array_insert_triggers_realloc();
  // test_array_insert_no_change_enough_space();
  // test_array_insert_back();
}
