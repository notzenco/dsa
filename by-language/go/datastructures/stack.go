package datastructures

import (
	"fmt"
	"strconv"
	"strings"
)

// Stack is a generic LIFO data structure.
type Stack[T any] struct {
	items []T
}

func NewStack[T any]() *Stack[T] {
	return &Stack[T]{}
}

func (s *Stack[T]) Push(item T) {
	s.items = append(s.items, item)
}

func (s *Stack[T]) Pop() (T, bool) {
	if len(s.items) == 0 {
		var zero T
		return zero, false
	}
	item := s.items[len(s.items)-1]
	s.items = s.items[:len(s.items)-1]
	return item, true
}

func (s *Stack[T]) Peek() (T, bool) {
	if len(s.items) == 0 {
		var zero T
		return zero, false
	}
	return s.items[len(s.items)-1], true
}

func (s *Stack[T]) IsEmpty() bool {
	return len(s.items) == 0
}

func (s *Stack[T]) Size() int {
	return len(s.items)
}

func (s *Stack[T]) Clear() {
	s.items = nil
}

func (s *Stack[T]) ToSlice() []T {
	result := make([]T, len(s.items))
	copy(result, s.items)
	return result
}

func StackFromSlice[T any](items []T) *Stack[T] {
	s := &Stack[T]{items: make([]T, len(items))}
	copy(s.items, items)
	return s
}

// MinStack supports O(1) push, pop, peek, and get_min operations.
type MinStack struct {
	items    []int
	minStack []int
}

func NewMinStack() *MinStack {
	return &MinStack{}
}

func (s *MinStack) Push(val int) {
	s.items = append(s.items, val)
	if len(s.minStack) == 0 || val <= s.minStack[len(s.minStack)-1] {
		s.minStack = append(s.minStack, val)
	}
}

func (s *MinStack) Pop() (int, bool) {
	if len(s.items) == 0 {
		return 0, false
	}
	item := s.items[len(s.items)-1]
	s.items = s.items[:len(s.items)-1]
	if item == s.minStack[len(s.minStack)-1] {
		s.minStack = s.minStack[:len(s.minStack)-1]
	}
	return item, true
}

func (s *MinStack) Peek() (int, bool) {
	if len(s.items) == 0 {
		return 0, false
	}
	return s.items[len(s.items)-1], true
}

func (s *MinStack) GetMin() (int, bool) {
	if len(s.minStack) == 0 {
		return 0, false
	}
	return s.minStack[len(s.minStack)-1], true
}

func (s *MinStack) IsEmpty() bool {
	return len(s.items) == 0
}

func (s *MinStack) Size() int {
	return len(s.items)
}

// MaxStack supports O(1) push, pop, peek, and get_max operations.
type MaxStack struct {
	items    []int
	maxStack []int
}

func NewMaxStack() *MaxStack {
	return &MaxStack{}
}

func (s *MaxStack) Push(val int) {
	s.items = append(s.items, val)
	if len(s.maxStack) == 0 || val >= s.maxStack[len(s.maxStack)-1] {
		s.maxStack = append(s.maxStack, val)
	}
}

func (s *MaxStack) Pop() (int, bool) {
	if len(s.items) == 0 {
		return 0, false
	}
	item := s.items[len(s.items)-1]
	s.items = s.items[:len(s.items)-1]
	if item == s.maxStack[len(s.maxStack)-1] {
		s.maxStack = s.maxStack[:len(s.maxStack)-1]
	}
	return item, true
}

func (s *MaxStack) Peek() (int, bool) {
	if len(s.items) == 0 {
		return 0, false
	}
	return s.items[len(s.items)-1], true
}

func (s *MaxStack) GetMax() (int, bool) {
	if len(s.maxStack) == 0 {
		return 0, false
	}
	return s.maxStack[len(s.maxStack)-1], true
}

func (s *MaxStack) IsEmpty() bool {
	return len(s.items) == 0
}

func (s *MaxStack) Size() int {
	return len(s.items)
}

// IsBalancedParentheses checks if string has balanced brackets.
func IsBalancedParentheses(s string) bool {
	stack := NewStack[rune]()
	matching := map[rune]rune{')': '(', ']': '[', '}': '{'}

	for _, ch := range s {
		switch ch {
		case '(', '[', '{':
			stack.Push(ch)
		case ')', ']', '}':
			top, ok := stack.Pop()
			if !ok || top != matching[ch] {
				return false
			}
		}
	}
	return stack.IsEmpty()
}

// EvaluatePostfix evaluates a postfix expression.
func EvaluatePostfix(expression string) (float64, error) {
	stack := NewStack[float64]()
	tokens := strings.Fields(expression)

	for _, token := range tokens {
		switch token {
		case "+", "-", "*", "/":
			b, okB := stack.Pop()
			a, okA := stack.Pop()
			if !okA || !okB {
				return 0, fmt.Errorf("invalid expression")
			}
			var result float64
			switch token {
			case "+":
				result = a + b
			case "-":
				result = a - b
			case "*":
				result = a * b
			case "/":
				if b == 0 {
					return 0, fmt.Errorf("division by zero")
				}
				result = a / b
			}
			stack.Push(result)
		default:
			val, err := strconv.ParseFloat(token, 64)
			if err != nil {
				return 0, fmt.Errorf("invalid token: %s", token)
			}
			stack.Push(val)
		}
	}

	result, ok := stack.Pop()
	if !ok {
		return 0, fmt.Errorf("invalid expression")
	}
	return result, nil
}

// NextGreaterElement finds the next greater element for each element.
func NextGreaterElement(arr []int) []int {
	n := len(arr)
	result := make([]int, n)
	for i := range result {
		result[i] = -1
	}

	stack := NewStack[int]() // stores indices

	for i := 0; i < n; i++ {
		for {
			top, ok := stack.Peek()
			if !ok || arr[top] >= arr[i] {
				break
			}
			idx, _ := stack.Pop()
			result[idx] = arr[i]
		}
		stack.Push(i)
	}

	return result
}
