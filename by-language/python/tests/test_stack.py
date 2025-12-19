"""
Tests for Stack implementations.
"""

import pytest
from data_structures.stack import (
    Stack, MinStack, MaxStack,
    is_balanced_parentheses, evaluate_postfix, next_greater_element
)


class TestStackBasics:
    """Test basic stack operations."""

    def test_init(self):
        """Test initialization."""
        stack = Stack()
        assert len(stack) == 0
        assert stack.is_empty

    def test_push(self):
        """Test pushing elements."""
        stack = Stack()
        stack.push(10)
        stack.push(20)

        assert len(stack) == 2
        assert not stack.is_empty

    def test_pop(self):
        """Test popping elements."""
        stack = Stack.from_list([10, 20, 30])
        assert stack.pop() == 30
        assert stack.pop() == 20
        assert stack.pop() == 10

    def test_pop_empty_raises(self):
        """Test popping from empty stack raises error."""
        stack = Stack()
        with pytest.raises(IndexError):
            stack.pop()

    def test_peek(self):
        """Test peeking at top element."""
        stack = Stack.from_list([10, 20, 30])
        assert stack.peek() == 30
        assert len(stack) == 3  # Size unchanged

    def test_peek_empty_raises(self):
        """Test peeking empty stack raises error."""
        stack = Stack()
        with pytest.raises(IndexError):
            stack.peek()


class TestStackUtilities:
    """Test utility methods."""

    def test_clear(self):
        """Test clearing the stack."""
        stack = Stack.from_list([1, 2, 3])
        stack.clear()
        assert stack.is_empty

    def test_to_list(self):
        """Test converting to list."""
        stack = Stack.from_list([1, 2, 3])
        assert stack.to_list() == [1, 2, 3]

    def test_from_list(self):
        """Test creating from list."""
        stack = Stack.from_list([1, 2, 3])
        assert stack.pop() == 3
        assert stack.pop() == 2

    def test_iter(self):
        """Test iteration (bottom to top)."""
        stack = Stack.from_list([1, 2, 3])
        result = list(stack)
        assert result == [1, 2, 3]

    def test_contains(self):
        """Test __contains__."""
        stack = Stack.from_list([1, 2, 3])
        assert 2 in stack
        assert 5 not in stack

    def test_repr(self):
        """Test string representation."""
        stack = Stack.from_list([1, 2, 3])
        assert "1" in repr(stack)

    def test_equality(self):
        """Test equality comparison."""
        s1 = Stack.from_list([1, 2, 3])
        s2 = Stack.from_list([1, 2, 3])
        s3 = Stack.from_list([1, 2, 4])

        assert s1 == s2
        assert s1 != s3


class TestMinStack:
    """Test MinStack operations."""

    def test_basic_operations(self):
        """Test basic push/pop/peek."""
        stack = MinStack()
        stack.push(3)
        stack.push(1)
        stack.push(2)

        assert stack.peek() == 2
        assert stack.pop() == 2
        assert len(stack) == 2

    def test_get_min(self):
        """Test getting minimum element."""
        stack = MinStack()
        stack.push(5)
        assert stack.get_min() == 5

        stack.push(3)
        assert stack.get_min() == 3

        stack.push(7)
        assert stack.get_min() == 3

        stack.push(1)
        assert stack.get_min() == 1

    def test_get_min_after_pop(self):
        """Test minimum updates after pop."""
        stack = MinStack()
        stack.push(5)
        stack.push(3)
        stack.push(7)
        stack.push(1)

        assert stack.get_min() == 1
        stack.pop()  # Remove 1
        assert stack.get_min() == 3

    def test_get_min_empty_raises(self):
        """Test get_min on empty stack."""
        stack = MinStack()
        with pytest.raises(IndexError):
            stack.get_min()

    def test_duplicate_minimums(self):
        """Test with duplicate minimum values."""
        stack = MinStack()
        stack.push(2)
        stack.push(1)
        stack.push(1)

        assert stack.get_min() == 1
        stack.pop()
        assert stack.get_min() == 1
        stack.pop()
        assert stack.get_min() == 2


class TestMaxStack:
    """Test MaxStack operations."""

    def test_get_max(self):
        """Test getting maximum element."""
        stack = MaxStack()
        stack.push(1)
        assert stack.get_max() == 1

        stack.push(5)
        assert stack.get_max() == 5

        stack.push(3)
        assert stack.get_max() == 5

        stack.push(7)
        assert stack.get_max() == 7

    def test_get_max_after_pop(self):
        """Test maximum updates after pop."""
        stack = MaxStack()
        stack.push(1)
        stack.push(5)
        stack.push(3)
        stack.push(7)

        assert stack.get_max() == 7
        stack.pop()  # Remove 7
        assert stack.get_max() == 5


class TestBalancedParentheses:
    """Test balanced parentheses utility."""

    def test_balanced(self):
        """Test balanced expressions."""
        assert is_balanced_parentheses("()") is True
        assert is_balanced_parentheses("()[]{}") is True
        assert is_balanced_parentheses("{[()]}") is True
        assert is_balanced_parentheses("((()))") is True

    def test_unbalanced(self):
        """Test unbalanced expressions."""
        assert is_balanced_parentheses("(") is False
        assert is_balanced_parentheses(")") is False
        assert is_balanced_parentheses("([)]") is False
        assert is_balanced_parentheses("{[}]") is False
        assert is_balanced_parentheses("((())") is False

    def test_empty_string(self):
        """Test empty string."""
        assert is_balanced_parentheses("") is True

    def test_with_other_characters(self):
        """Test with non-parentheses characters."""
        assert is_balanced_parentheses("a(b)c") is True
        assert is_balanced_parentheses("a + (b * c)") is True


class TestEvaluatePostfix:
    """Test postfix expression evaluation."""

    def test_addition(self):
        """Test addition."""
        assert evaluate_postfix("2 3 +") == 5

    def test_subtraction(self):
        """Test subtraction."""
        assert evaluate_postfix("5 3 -") == 2

    def test_multiplication(self):
        """Test multiplication."""
        assert evaluate_postfix("4 5 *") == 20

    def test_division(self):
        """Test division."""
        assert evaluate_postfix("10 2 /") == 5

    def test_complex_expression(self):
        """Test complex expression."""
        # (2 + 3) * 4 = 20
        assert evaluate_postfix("2 3 + 4 *") == 20
        # 5 + ((1 + 2) * 4) - 3 = 5 + 12 - 3 = 14
        assert evaluate_postfix("5 1 2 + 4 * + 3 -") == 14

    def test_division_by_zero(self):
        """Test division by zero."""
        with pytest.raises(ValueError):
            evaluate_postfix("5 0 /")

    def test_invalid_expression(self):
        """Test invalid expression."""
        with pytest.raises(ValueError):
            evaluate_postfix("2 +")


class TestNextGreaterElement:
    """Test next greater element utility."""

    def test_basic(self):
        """Test basic case."""
        result = next_greater_element([4, 5, 2, 10, 8])
        assert result == [5, 10, 10, -1, -1]

    def test_decreasing(self):
        """Test decreasing sequence."""
        result = next_greater_element([5, 4, 3, 2, 1])
        assert result == [-1, -1, -1, -1, -1]

    def test_increasing(self):
        """Test increasing sequence."""
        result = next_greater_element([1, 2, 3, 4, 5])
        assert result == [2, 3, 4, 5, -1]

    def test_single_element(self):
        """Test single element."""
        result = next_greater_element([42])
        assert result == [-1]

    def test_empty(self):
        """Test empty list."""
        result = next_greater_element([])
        assert result == []
