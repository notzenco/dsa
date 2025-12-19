"""
Stack (LIFO - Last In, First Out)

╔════════════════════════════════════════════════════════════════════════════╗
║                           VISUAL REPRESENTATION                            ║
╠════════════════════════════════════════════════════════════════════════════╣
║                                                                            ║
║  Stack Structure:                                                          ║
║  ┌─────────────────┐                                                       ║
║  │       TOP       │  ◄── push() and pop() happen here                     ║
║  ├─────────────────┤                                                       ║
║  │       40        │  ◄── most recently added (top)                        ║
║  ├─────────────────┤                                                       ║
║  │       30        │                                                       ║
║  ├─────────────────┤                                                       ║
║  │       20        │                                                       ║
║  ├─────────────────┤                                                       ║
║  │       10        │  ◄── first added (bottom)                             ║
║  └─────────────────┘                                                       ║
║      BOTTOM                                                                ║
║                                                                            ║
║  Push Operation:                                                           ║
║  Before: [10, 20, 30]  (top = 30)                                          ║
║  push(40)                                                                  ║
║  After:  [10, 20, 30, 40]  (top = 40)                                      ║
║                                                                            ║
║  Pop Operation:                                                            ║
║  Before: [10, 20, 30, 40]  (top = 40)                                      ║
║  pop() → returns 40                                                        ║
║  After:  [10, 20, 30]  (top = 30)                                          ║
║                                                                            ║
║  Min Stack Visualization:                                                  ║
║  ┌─────────────────┬─────────────────┐                                     ║
║  │  Main Stack     │   Min Stack     │                                     ║
║  ├─────────────────┼─────────────────┤                                     ║
║  │      5          │      5          │  ◄── push 5: min = 5                ║
║  │      3          │      3          │  ◄── push 3: min = 3                ║
║  │      7          │      3          │  ◄── push 7: min still 3            ║
║  │      2          │      2          │  ◄── push 2: min = 2                ║
║  └─────────────────┴─────────────────┘                                     ║
║                                                                            ║
╚════════════════════════════════════════════════════════════════════════════╝

COMPLEXITY:
┌─────────────────────┬─────────────┬─────────────┬─────────────┐
│ Operation           │ Time        │ Space       │ Notes       │
├─────────────────────┼─────────────┼─────────────┼─────────────┤
│ push                │ O(1)*       │ O(1)        │ *amortized  │
│ pop                 │ O(1)        │ O(1)        │             │
│ peek/top            │ O(1)        │ O(1)        │             │
│ is_empty            │ O(1)        │ O(1)        │             │
│ size                │ O(1)        │ O(1)        │             │
│ get_min (MinStack)  │ O(1)        │ O(n)        │ extra stack │
│ get_max (MaxStack)  │ O(1)        │ O(n)        │ extra stack │
└─────────────────────┴─────────────┴─────────────┴─────────────┘

LEETCODE PROBLEMS:
- #20 Valid Parentheses
- #155 Min Stack
- #232 Implement Queue using Stacks
- #225 Implement Stack using Queues
- #150 Evaluate Reverse Polish Notation
- #71 Simplify Path
- #84 Largest Rectangle in Histogram
- #85 Maximal Rectangle
- #739 Daily Temperatures
- #496 Next Greater Element I
- #503 Next Greater Element II
- #42 Trapping Rain Water

USE CASES:
- Function call stack (recursion)
- Undo/Redo operations
- Browser back/forward buttons
- Expression evaluation (postfix, prefix)
- Parentheses matching
- Depth-first search (DFS)
- Backtracking algorithms
- Syntax parsing
"""

from typing import TypeVar, Generic, Iterator, Optional, List

T = TypeVar('T')


class Stack(Generic[T]):
    """
    A stack implementation using a dynamic array (list).

    This is the standard stack with O(1) push and pop operations.
    Uses Python's list which provides amortized O(1) append.
    """

    def __init__(self) -> None:
        """Initialize an empty stack."""
        self._data: List[T] = []

    def __len__(self) -> int:
        """Return the number of elements in the stack."""
        return len(self._data)

    def __iter__(self) -> Iterator[T]:
        """Iterate over elements from bottom to top."""
        return iter(self._data)

    def __repr__(self) -> str:
        """Return string representation of the stack."""
        return f"Stack({self._data})"

    def __eq__(self, other: object) -> bool:
        """Check equality with another stack."""
        if not isinstance(other, Stack):
            return NotImplemented
        return self._data == other._data

    def __contains__(self, value: T) -> bool:
        """Check if value exists in stack."""
        return value in self._data

    @property
    def is_empty(self) -> bool:
        """Return True if stack is empty."""
        return len(self._data) == 0

    def push(self, value: T) -> None:
        """
        Push an element onto the stack.

        Time Complexity: O(1) amortized

        Args:
            value: The value to push.
        """
        self._data.append(value)

    def pop(self) -> T:
        """
        Pop and return the top element.

        Time Complexity: O(1)

        Returns:
            The top element.

        Raises:
            IndexError: If the stack is empty.
        """
        if self.is_empty:
            raise IndexError("Pop from empty stack")
        return self._data.pop()

    def peek(self) -> T:
        """
        Return the top element without removing it.

        Time Complexity: O(1)

        Returns:
            The top element.

        Raises:
            IndexError: If the stack is empty.
        """
        if self.is_empty:
            raise IndexError("Peek from empty stack")
        return self._data[-1]

    def clear(self) -> None:
        """Remove all elements from the stack."""
        self._data.clear()

    def to_list(self) -> List[T]:
        """Convert stack to list (bottom to top)."""
        return self._data.copy()

    @classmethod
    def from_list(cls, items: List[T]) -> "Stack[T]":
        """
        Create a stack from a list.

        First element of list becomes bottom of stack.

        Args:
            items: List of items.

        Returns:
            A new Stack.
        """
        stack: Stack[T] = cls()
        for item in items:
            stack.push(item)
        return stack


class MinStack(Generic[T]):
    """
    A stack that supports O(1) retrieval of minimum element.

    Uses an auxiliary stack to track minimums.
    """

    def __init__(self) -> None:
        """Initialize an empty min stack."""
        self._data: List[T] = []
        self._min_stack: List[T] = []

    def __len__(self) -> int:
        """Return the number of elements in the stack."""
        return len(self._data)

    def __repr__(self) -> str:
        """Return string representation."""
        return f"MinStack({self._data})"

    @property
    def is_empty(self) -> bool:
        """Return True if stack is empty."""
        return len(self._data) == 0

    def push(self, value: T) -> None:
        """
        Push an element onto the stack.

        Time Complexity: O(1)

        Args:
            value: The value to push.
        """
        self._data.append(value)

        if not self._min_stack or value <= self._min_stack[-1]:
            self._min_stack.append(value)

    def pop(self) -> T:
        """
        Pop and return the top element.

        Time Complexity: O(1)

        Returns:
            The top element.

        Raises:
            IndexError: If the stack is empty.
        """
        if self.is_empty:
            raise IndexError("Pop from empty stack")

        value = self._data.pop()

        if value == self._min_stack[-1]:
            self._min_stack.pop()

        return value

    def peek(self) -> T:
        """
        Return the top element without removing it.

        Time Complexity: O(1)

        Returns:
            The top element.

        Raises:
            IndexError: If the stack is empty.
        """
        if self.is_empty:
            raise IndexError("Peek from empty stack")
        return self._data[-1]

    def get_min(self) -> T:
        """
        Return the minimum element in the stack.

        Time Complexity: O(1)

        Returns:
            The minimum element.

        Raises:
            IndexError: If the stack is empty.
        """
        if self.is_empty:
            raise IndexError("get_min from empty stack")
        return self._min_stack[-1]


class MaxStack(Generic[T]):
    """
    A stack that supports O(1) retrieval of maximum element.

    Uses an auxiliary stack to track maximums.
    """

    def __init__(self) -> None:
        """Initialize an empty max stack."""
        self._data: List[T] = []
        self._max_stack: List[T] = []

    def __len__(self) -> int:
        """Return the number of elements in the stack."""
        return len(self._data)

    def __repr__(self) -> str:
        """Return string representation."""
        return f"MaxStack({self._data})"

    @property
    def is_empty(self) -> bool:
        """Return True if stack is empty."""
        return len(self._data) == 0

    def push(self, value: T) -> None:
        """
        Push an element onto the stack.

        Time Complexity: O(1)

        Args:
            value: The value to push.
        """
        self._data.append(value)

        if not self._max_stack or value >= self._max_stack[-1]:
            self._max_stack.append(value)

    def pop(self) -> T:
        """
        Pop and return the top element.

        Time Complexity: O(1)

        Returns:
            The top element.

        Raises:
            IndexError: If the stack is empty.
        """
        if self.is_empty:
            raise IndexError("Pop from empty stack")

        value = self._data.pop()

        if value == self._max_stack[-1]:
            self._max_stack.pop()

        return value

    def peek(self) -> T:
        """
        Return the top element without removing it.

        Time Complexity: O(1)

        Returns:
            The top element.

        Raises:
            IndexError: If the stack is empty.
        """
        if self.is_empty:
            raise IndexError("Peek from empty stack")
        return self._data[-1]

    def get_max(self) -> T:
        """
        Return the maximum element in the stack.

        Time Complexity: O(1)

        Returns:
            The maximum element.

        Raises:
            IndexError: If the stack is empty.
        """
        if self.is_empty:
            raise IndexError("get_max from empty stack")
        return self._max_stack[-1]


# Utility functions that use stacks


def is_balanced_parentheses(s: str) -> bool:
    """
    Check if parentheses in a string are balanced.

    Supports: (), [], {}

    Time Complexity: O(n)
    Space Complexity: O(n)

    Args:
        s: String containing parentheses.

    Returns:
        True if balanced, False otherwise.
    """
    stack: Stack[str] = Stack()
    pairs = {')': '(', ']': '[', '}': '{'}

    for char in s:
        if char in '([{':
            stack.push(char)
        elif char in ')]}':
            if stack.is_empty or stack.pop() != pairs[char]:
                return False

    return stack.is_empty


def evaluate_postfix(expression: str) -> float:
    """
    Evaluate a postfix (Reverse Polish Notation) expression.

    Time Complexity: O(n)
    Space Complexity: O(n)

    Args:
        expression: Space-separated postfix expression.
                   Example: "2 3 + 4 *" = (2 + 3) * 4 = 20

    Returns:
        The result of the expression.

    Raises:
        ValueError: If the expression is invalid.
    """
    stack: Stack[float] = Stack()
    operators = {'+', '-', '*', '/'}

    tokens = expression.split()

    for token in tokens:
        if token in operators:
            if len(stack) < 2:
                raise ValueError("Invalid expression")

            b = stack.pop()
            a = stack.pop()

            if token == '+':
                stack.push(a + b)
            elif token == '-':
                stack.push(a - b)
            elif token == '*':
                stack.push(a * b)
            elif token == '/':
                if b == 0:
                    raise ValueError("Division by zero")
                stack.push(a / b)
        else:
            try:
                stack.push(float(token))
            except ValueError:
                raise ValueError(f"Invalid token: {token}")

    if len(stack) != 1:
        raise ValueError("Invalid expression")

    return stack.pop()


def next_greater_element(nums: List[int]) -> List[int]:
    """
    Find the next greater element for each element in the array.

    For each element, find the first greater element to its right.
    If none exists, use -1.

    Time Complexity: O(n)
    Space Complexity: O(n)

    Args:
        nums: List of integers.

    Returns:
        List where result[i] is the next greater element for nums[i].

    Example:
        [4, 5, 2, 10, 8] -> [5, 10, 10, -1, -1]
    """
    n = len(nums)
    result = [-1] * n
    stack: Stack[int] = Stack()  # Stack of indices

    for i in range(n):
        while not stack.is_empty and nums[stack.peek()] < nums[i]:
            idx = stack.pop()
            result[idx] = nums[i]
        stack.push(i)

    return result
