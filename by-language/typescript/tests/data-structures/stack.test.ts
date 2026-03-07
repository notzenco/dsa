import { describe, test, expect } from "bun:test";
import {
  Stack,
  MinStack,
  MaxStack,
  isBalancedParentheses,
  evaluatePostfix,
  nextGreaterElement,
} from "../../src/data-structures/stack";

describe("Stack", () => {
  describe("basic operations", () => {
    test("new stack is empty", () => {
      const stack = new Stack<number>();
      expect(stack.isEmpty).toBe(true);
      expect(stack.size).toBe(0);
    });

    test("push increases size", () => {
      const stack = new Stack<number>();
      stack.push(1);
      expect(stack.size).toBe(1);
      expect(stack.isEmpty).toBe(false);
      stack.push(2);
      expect(stack.size).toBe(2);
    });

    test("pop returns last pushed element (LIFO)", () => {
      const stack = new Stack<number>();
      stack.push(10);
      stack.push(20);
      stack.push(30);
      expect(stack.pop()).toBe(30);
      expect(stack.pop()).toBe(20);
      expect(stack.pop()).toBe(10);
    });

    test("pop from empty stack throws", () => {
      const stack = new Stack<number>();
      expect(() => stack.pop()).toThrow("Pop from empty stack");
    });

    test("peek returns top without removing", () => {
      const stack = new Stack<number>();
      stack.push(10);
      stack.push(20);
      expect(stack.peek()).toBe(20);
      expect(stack.size).toBe(2);
    });

    test("peek from empty stack throws", () => {
      const stack = new Stack<number>();
      expect(() => stack.peek()).toThrow("Peek from empty stack");
    });

    test("clear empties the stack", () => {
      const stack = new Stack<number>();
      stack.push(1);
      stack.push(2);
      stack.push(3);
      stack.clear();
      expect(stack.isEmpty).toBe(true);
      expect(stack.size).toBe(0);
    });
  });

  describe("toArray and toString", () => {
    test("toArray returns elements bottom to top", () => {
      const stack = new Stack<number>();
      stack.push(1);
      stack.push(2);
      stack.push(3);
      expect(stack.toArray()).toEqual([1, 2, 3]);
    });

    test("toArray returns copy", () => {
      const stack = new Stack<number>();
      stack.push(1);
      const arr = stack.toArray();
      arr.push(999);
      expect(stack.size).toBe(1);
    });

    test("toArray on empty stack returns empty array", () => {
      const stack = new Stack<number>();
      expect(stack.toArray()).toEqual([]);
    });

    test("toString shows stack contents", () => {
      const stack = new Stack<number>();
      stack.push(1);
      stack.push(2);
      expect(stack.toString()).toBe("Stack([1, 2])");
    });

    test("toString on empty stack", () => {
      const stack = new Stack<number>();
      expect(stack.toString()).toBe("Stack([])");
    });
  });

  describe("iterator", () => {
    test("iterates from bottom to top", () => {
      const stack = new Stack<number>();
      stack.push(1);
      stack.push(2);
      stack.push(3);
      const values: number[] = [];
      for (const v of stack) {
        values.push(v);
      }
      expect(values).toEqual([1, 2, 3]);
    });

    test("spread operator works", () => {
      const stack = new Stack<number>();
      stack.push(10);
      stack.push(20);
      expect([...stack]).toEqual([10, 20]);
    });
  });

  describe("fromArray", () => {
    test("creates stack from array", () => {
      const stack = Stack.fromArray([1, 2, 3]);
      expect(stack.size).toBe(3);
      expect(stack.pop()).toBe(3);
      expect(stack.pop()).toBe(2);
      expect(stack.pop()).toBe(1);
    });

    test("creates empty stack from empty array", () => {
      const stack = Stack.fromArray<number>([]);
      expect(stack.isEmpty).toBe(true);
    });
  });

  describe("with different types", () => {
    test("string stack", () => {
      const stack = new Stack<string>();
      stack.push("hello");
      stack.push("world");
      expect(stack.pop()).toBe("world");
      expect(stack.peek()).toBe("hello");
    });

    test("object stack", () => {
      const stack = new Stack<{ id: number }>();
      stack.push({ id: 1 });
      stack.push({ id: 2 });
      expect(stack.pop()).toEqual({ id: 2 });
    });
  });

  describe("single element operations", () => {
    test("push and pop single element", () => {
      const stack = new Stack<number>();
      stack.push(42);
      expect(stack.pop()).toBe(42);
      expect(stack.isEmpty).toBe(true);
    });

    test("push, pop, push again", () => {
      const stack = new Stack<number>();
      stack.push(1);
      stack.pop();
      stack.push(2);
      expect(stack.peek()).toBe(2);
      expect(stack.size).toBe(1);
    });
  });
});

describe("MinStack", () => {
  describe("basic operations", () => {
    test("new MinStack is empty", () => {
      const stack = new MinStack<number>();
      expect(stack.isEmpty).toBe(true);
      expect(stack.size).toBe(0);
    });

    test("push and pop work correctly", () => {
      const stack = new MinStack<number>();
      stack.push(3);
      stack.push(1);
      stack.push(2);
      expect(stack.pop()).toBe(2);
      expect(stack.pop()).toBe(1);
      expect(stack.pop()).toBe(3);
    });

    test("peek returns top element", () => {
      const stack = new MinStack<number>();
      stack.push(5);
      stack.push(3);
      expect(stack.peek()).toBe(3);
      expect(stack.size).toBe(2);
    });

    test("pop from empty throws", () => {
      const stack = new MinStack<number>();
      expect(() => stack.pop()).toThrow("Pop from empty stack");
    });

    test("peek from empty throws", () => {
      const stack = new MinStack<number>();
      expect(() => stack.peek()).toThrow("Peek from empty stack");
    });
  });

  describe("getMin", () => {
    test("tracks minimum correctly", () => {
      const stack = new MinStack<number>();
      stack.push(5);
      expect(stack.getMin()).toBe(5);

      stack.push(3);
      expect(stack.getMin()).toBe(3);

      stack.push(7);
      expect(stack.getMin()).toBe(3);

      stack.push(2);
      expect(stack.getMin()).toBe(2);
    });

    test("minimum updates after pop", () => {
      const stack = new MinStack<number>();
      stack.push(5);
      stack.push(3);
      stack.push(7);
      stack.push(2);

      stack.pop(); // remove 2
      expect(stack.getMin()).toBe(3);

      stack.pop(); // remove 7
      expect(stack.getMin()).toBe(3);

      stack.pop(); // remove 3
      expect(stack.getMin()).toBe(5);
    });

    test("getMin from empty throws", () => {
      const stack = new MinStack<number>();
      expect(() => stack.getMin()).toThrow("getMin from empty stack");
    });

    test("handles duplicate minimums", () => {
      const stack = new MinStack<number>();
      stack.push(3);
      stack.push(3);
      stack.push(3);
      expect(stack.getMin()).toBe(3);
      stack.pop();
      expect(stack.getMin()).toBe(3);
      stack.pop();
      expect(stack.getMin()).toBe(3);
    });

    test("single element is the minimum", () => {
      const stack = new MinStack<number>();
      stack.push(42);
      expect(stack.getMin()).toBe(42);
    });

    test("decreasing sequence", () => {
      const stack = new MinStack<number>();
      stack.push(5);
      stack.push(4);
      stack.push(3);
      stack.push(2);
      stack.push(1);
      expect(stack.getMin()).toBe(1);
      stack.pop();
      expect(stack.getMin()).toBe(2);
    });

    test("increasing sequence", () => {
      const stack = new MinStack<number>();
      stack.push(1);
      stack.push(2);
      stack.push(3);
      stack.push(4);
      expect(stack.getMin()).toBe(1);
      stack.pop();
      expect(stack.getMin()).toBe(1);
    });
  });

  describe("clear, toArray, toString", () => {
    test("clear empties everything", () => {
      const stack = new MinStack<number>();
      stack.push(1);
      stack.push(2);
      stack.clear();
      expect(stack.isEmpty).toBe(true);
      expect(stack.size).toBe(0);
    });

    test("toArray returns elements", () => {
      const stack = new MinStack<number>();
      stack.push(5);
      stack.push(3);
      expect(stack.toArray()).toEqual([5, 3]);
    });

    test("toString", () => {
      const stack = new MinStack<number>();
      stack.push(1);
      stack.push(2);
      expect(stack.toString()).toBe("MinStack([1, 2])");
    });
  });
});

describe("MaxStack", () => {
  describe("basic operations", () => {
    test("new MaxStack is empty", () => {
      const stack = new MaxStack<number>();
      expect(stack.isEmpty).toBe(true);
      expect(stack.size).toBe(0);
    });

    test("push and pop work correctly", () => {
      const stack = new MaxStack<number>();
      stack.push(3);
      stack.push(1);
      stack.push(2);
      expect(stack.pop()).toBe(2);
      expect(stack.pop()).toBe(1);
      expect(stack.pop()).toBe(3);
    });

    test("peek returns top element", () => {
      const stack = new MaxStack<number>();
      stack.push(5);
      stack.push(3);
      expect(stack.peek()).toBe(3);
    });

    test("pop from empty throws", () => {
      const stack = new MaxStack<number>();
      expect(() => stack.pop()).toThrow("Pop from empty stack");
    });

    test("peek from empty throws", () => {
      const stack = new MaxStack<number>();
      expect(() => stack.peek()).toThrow("Peek from empty stack");
    });
  });

  describe("getMax", () => {
    test("tracks maximum correctly", () => {
      const stack = new MaxStack<number>();
      stack.push(2);
      expect(stack.getMax()).toBe(2);

      stack.push(5);
      expect(stack.getMax()).toBe(5);

      stack.push(3);
      expect(stack.getMax()).toBe(5);

      stack.push(7);
      expect(stack.getMax()).toBe(7);
    });

    test("maximum updates after pop", () => {
      const stack = new MaxStack<number>();
      stack.push(2);
      stack.push(5);
      stack.push(3);
      stack.push(7);

      stack.pop(); // remove 7
      expect(stack.getMax()).toBe(5);

      stack.pop(); // remove 3
      expect(stack.getMax()).toBe(5);

      stack.pop(); // remove 5
      expect(stack.getMax()).toBe(2);
    });

    test("getMax from empty throws", () => {
      const stack = new MaxStack<number>();
      expect(() => stack.getMax()).toThrow("getMax from empty stack");
    });

    test("handles duplicate maximums", () => {
      const stack = new MaxStack<number>();
      stack.push(5);
      stack.push(5);
      stack.push(5);
      expect(stack.getMax()).toBe(5);
      stack.pop();
      expect(stack.getMax()).toBe(5);
      stack.pop();
      expect(stack.getMax()).toBe(5);
    });

    test("single element is the maximum", () => {
      const stack = new MaxStack<number>();
      stack.push(42);
      expect(stack.getMax()).toBe(42);
    });

    test("increasing sequence", () => {
      const stack = new MaxStack<number>();
      stack.push(1);
      stack.push(2);
      stack.push(3);
      stack.push(4);
      stack.push(5);
      expect(stack.getMax()).toBe(5);
      stack.pop();
      expect(stack.getMax()).toBe(4);
    });

    test("decreasing sequence", () => {
      const stack = new MaxStack<number>();
      stack.push(5);
      stack.push(4);
      stack.push(3);
      stack.push(2);
      expect(stack.getMax()).toBe(5);
      stack.pop();
      expect(stack.getMax()).toBe(5);
    });
  });

  describe("clear, toArray, toString", () => {
    test("clear empties everything", () => {
      const stack = new MaxStack<number>();
      stack.push(1);
      stack.push(2);
      stack.clear();
      expect(stack.isEmpty).toBe(true);
    });

    test("toArray returns elements", () => {
      const stack = new MaxStack<number>();
      stack.push(5);
      stack.push(3);
      expect(stack.toArray()).toEqual([5, 3]);
    });

    test("toString", () => {
      const stack = new MaxStack<number>();
      stack.push(1);
      stack.push(2);
      expect(stack.toString()).toBe("MaxStack([1, 2])");
    });
  });
});

describe("isBalancedParentheses", () => {
  test("empty string is balanced", () => {
    expect(isBalancedParentheses("")).toBe(true);
  });

  test("simple balanced pairs", () => {
    expect(isBalancedParentheses("()")).toBe(true);
    expect(isBalancedParentheses("[]")).toBe(true);
    expect(isBalancedParentheses("{}")).toBe(true);
  });

  test("nested balanced", () => {
    expect(isBalancedParentheses("({[]})")).toBe(true);
    expect(isBalancedParentheses("((()))")).toBe(true);
    expect(isBalancedParentheses("{[()]}")).toBe(true);
  });

  test("multiple groups balanced", () => {
    expect(isBalancedParentheses("()[]{}")).toBe(true);
    expect(isBalancedParentheses("()()")).toBe(true);
  });

  test("unbalanced strings", () => {
    expect(isBalancedParentheses("(")).toBe(false);
    expect(isBalancedParentheses(")")).toBe(false);
    expect(isBalancedParentheses("(]")).toBe(false);
    expect(isBalancedParentheses("([)]")).toBe(false);
    expect(isBalancedParentheses("((())")).toBe(false);
  });

  test("string with non-bracket characters", () => {
    expect(isBalancedParentheses("a(b)c")).toBe(true);
    expect(isBalancedParentheses("hello")).toBe(true);
    expect(isBalancedParentheses("a(b")).toBe(false);
  });

  test("single closing bracket", () => {
    expect(isBalancedParentheses("]")).toBe(false);
    expect(isBalancedParentheses("}")).toBe(false);
  });
});

describe("evaluatePostfix", () => {
  test("simple addition", () => {
    expect(evaluatePostfix("2 3 +")).toBe(5);
  });

  test("simple subtraction", () => {
    expect(evaluatePostfix("5 3 -")).toBe(2);
  });

  test("simple multiplication", () => {
    expect(evaluatePostfix("4 3 *")).toBe(12);
  });

  test("simple division", () => {
    expect(evaluatePostfix("10 2 /")).toBe(5);
  });

  test("complex expression", () => {
    // (2 + 3) * 4 = 20
    expect(evaluatePostfix("2 3 + 4 *")).toBe(20);
  });

  test("another complex expression", () => {
    // 5 + ((1 + 2) * 4) - 3 = 14
    expect(evaluatePostfix("5 1 2 + 4 * + 3 -")).toBe(14);
  });

  test("single number", () => {
    expect(evaluatePostfix("42")).toBe(42);
  });

  test("division by zero throws", () => {
    expect(() => evaluatePostfix("5 0 /")).toThrow("Division by zero");
  });

  test("invalid token throws", () => {
    expect(() => evaluatePostfix("2 abc +")).toThrow("Invalid token: abc");
  });

  test("too many operators throws", () => {
    expect(() => evaluatePostfix("2 + +")).toThrow("Invalid expression");
  });

  test("too many operands throws", () => {
    expect(() => evaluatePostfix("2 3 4 +")).toThrow("Invalid expression");
  });

  test("decimal numbers", () => {
    expect(evaluatePostfix("2.5 3.5 +")).toBe(6);
  });

  test("negative result", () => {
    expect(evaluatePostfix("3 5 -")).toBe(-2);
  });
});

describe("nextGreaterElement", () => {
  test("basic case", () => {
    expect(nextGreaterElement([4, 5, 2, 10, 8])).toEqual([5, 10, 10, -1, -1]);
  });

  test("decreasing sequence", () => {
    expect(nextGreaterElement([5, 4, 3, 2, 1])).toEqual([-1, -1, -1, -1, -1]);
  });

  test("increasing sequence", () => {
    expect(nextGreaterElement([1, 2, 3, 4, 5])).toEqual([2, 3, 4, 5, -1]);
  });

  test("single element", () => {
    expect(nextGreaterElement([1])).toEqual([-1]);
  });

  test("empty array", () => {
    expect(nextGreaterElement([])).toEqual([]);
  });

  test("all same elements", () => {
    expect(nextGreaterElement([3, 3, 3])).toEqual([-1, -1, -1]);
  });

  test("two elements", () => {
    expect(nextGreaterElement([1, 2])).toEqual([2, -1]);
    expect(nextGreaterElement([2, 1])).toEqual([-1, -1]);
  });
});
