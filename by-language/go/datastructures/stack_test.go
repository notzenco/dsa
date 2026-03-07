package datastructures

import (
	"math"
	"testing"
)

func TestStack(t *testing.T) {
	s := NewStack[int]()

	if !s.IsEmpty() {
		t.Error("new stack should be empty")
	}
	if s.Size() != 0 {
		t.Error("new stack should have size 0")
	}

	s.Push(1)
	s.Push(2)
	s.Push(3)

	if s.Size() != 3 {
		t.Errorf("expected size 3, got %d", s.Size())
	}

	val, ok := s.Peek()
	if !ok || val != 3 {
		t.Errorf("expected peek 3, got %d", val)
	}

	val, ok = s.Pop()
	if !ok || val != 3 {
		t.Errorf("expected pop 3, got %d", val)
	}

	val, ok = s.Pop()
	if !ok || val != 2 {
		t.Errorf("expected pop 2, got %d", val)
	}

	val, ok = s.Pop()
	if !ok || val != 1 {
		t.Errorf("expected pop 1, got %d", val)
	}

	_, ok = s.Pop()
	if ok {
		t.Error("pop from empty stack should return false")
	}

	_, ok = s.Peek()
	if ok {
		t.Error("peek from empty stack should return false")
	}
}

func TestStackClear(t *testing.T) {
	s := NewStack[string]()
	s.Push("a")
	s.Push("b")
	s.Clear()
	if !s.IsEmpty() {
		t.Error("cleared stack should be empty")
	}
}

func TestStackToSlice(t *testing.T) {
	s := NewStack[int]()
	s.Push(1)
	s.Push(2)
	s.Push(3)

	slice := s.ToSlice()
	expected := []int{1, 2, 3}
	for i, v := range expected {
		if slice[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, slice[i])
		}
	}
}

func TestStackFromSlice(t *testing.T) {
	s := StackFromSlice([]int{1, 2, 3})
	val, _ := s.Pop()
	if val != 3 {
		t.Errorf("expected 3, got %d", val)
	}
	if s.Size() != 2 {
		t.Errorf("expected size 2, got %d", s.Size())
	}
}

func TestMinStack(t *testing.T) {
	s := NewMinStack()

	s.Push(5)
	s.Push(3)
	s.Push(7)
	s.Push(1)
	s.Push(4)

	min, ok := s.GetMin()
	if !ok || min != 1 {
		t.Errorf("expected min 1, got %d", min)
	}

	s.Pop() // remove 4
	min, _ = s.GetMin()
	if min != 1 {
		t.Errorf("expected min 1, got %d", min)
	}

	s.Pop() // remove 1
	min, _ = s.GetMin()
	if min != 3 {
		t.Errorf("expected min 3, got %d", min)
	}

	s.Pop() // remove 7
	min, _ = s.GetMin()
	if min != 3 {
		t.Errorf("expected min 3, got %d", min)
	}

	s.Pop() // remove 3
	min, _ = s.GetMin()
	if min != 5 {
		t.Errorf("expected min 5, got %d", min)
	}
}

func TestMaxStack(t *testing.T) {
	s := NewMaxStack()

	s.Push(5)
	s.Push(3)
	s.Push(7)
	s.Push(1)

	max, ok := s.GetMax()
	if !ok || max != 7 {
		t.Errorf("expected max 7, got %d", max)
	}

	s.Pop() // remove 1
	max, _ = s.GetMax()
	if max != 7 {
		t.Errorf("expected max 7, got %d", max)
	}

	s.Pop() // remove 7
	max, _ = s.GetMax()
	if max != 5 {
		t.Errorf("expected max 5, got %d", max)
	}
}

func TestIsBalancedParentheses(t *testing.T) {
	tests := []struct {
		input    string
		expected bool
	}{
		{"()", true},
		{"()[]{}", true},
		{"([])", true},
		{"{[()]}", true},
		{"(]", false},
		{"([)]", false},
		{"{", false},
		{"", true},
	}

	for _, tc := range tests {
		result := IsBalancedParentheses(tc.input)
		if result != tc.expected {
			t.Errorf("IsBalancedParentheses(%q) = %v, want %v", tc.input, result, tc.expected)
		}
	}
}

func TestEvaluatePostfix(t *testing.T) {
	result, err := EvaluatePostfix("3 4 + 2 *")
	if err != nil {
		t.Fatal(err)
	}
	if result != 14 {
		t.Errorf("expected 14, got %f", result)
	}

	result, err = EvaluatePostfix("5 1 2 + 4 * + 3 -")
	if err != nil {
		t.Fatal(err)
	}
	if result != 14 {
		t.Errorf("expected 14, got %f", result)
	}

	result, err = EvaluatePostfix("10 2 /")
	if err != nil {
		t.Fatal(err)
	}
	if math.Abs(result-5) > 1e-9 {
		t.Errorf("expected 5, got %f", result)
	}
}

func TestNextGreaterElement(t *testing.T) {
	result := NextGreaterElement([]int{4, 5, 2, 25})
	expected := []int{5, 25, 25, -1}
	for i, v := range expected {
		if result[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, result[i])
		}
	}

	result = NextGreaterElement([]int{13, 7, 6, 12})
	expected = []int{-1, 12, 12, -1}
	for i, v := range expected {
		if result[i] != v {
			t.Errorf("index %d: expected %d, got %d", i, v, result[i])
		}
	}
}
