package dsa.datastructures;

import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.List;
import java.util.NoSuchElementException;

import static org.junit.jupiter.api.Assertions.*;

class StackTest {

    // ---- Stack<T> tests ----

    @Test
    void testPushAndPop() {
        Stack<Integer> stack = new Stack<>();
        stack.push(1);
        stack.push(2);
        stack.push(3);
        assertEquals(3, stack.pop());
        assertEquals(2, stack.pop());
        assertEquals(1, stack.pop());
    }

    @Test
    void testPopEmpty() {
        Stack<Integer> stack = new Stack<>();
        assertThrows(NoSuchElementException.class, stack::pop);
    }

    @Test
    void testPeek() {
        Stack<String> stack = new Stack<>();
        stack.push("a");
        stack.push("b");
        assertEquals("b", stack.peek());
        assertEquals(2, stack.size());
    }

    @Test
    void testPeekEmpty() {
        Stack<Integer> stack = new Stack<>();
        assertThrows(NoSuchElementException.class, stack::peek);
    }

    @Test
    void testIsEmpty() {
        Stack<Integer> stack = new Stack<>();
        assertTrue(stack.isEmpty());
        stack.push(42);
        assertFalse(stack.isEmpty());
        stack.pop();
        assertTrue(stack.isEmpty());
    }

    @Test
    void testSize() {
        Stack<Integer> stack = new Stack<>();
        assertEquals(0, stack.size());
        stack.push(1);
        stack.push(2);
        assertEquals(2, stack.size());
        stack.pop();
        assertEquals(1, stack.size());
    }

    @Test
    void testClear() {
        Stack<Integer> stack = new Stack<>();
        stack.push(1);
        stack.push(2);
        stack.push(3);
        stack.clear();
        assertTrue(stack.isEmpty());
        assertEquals(0, stack.size());
    }

    @Test
    void testToList() {
        Stack<Integer> stack = new Stack<>();
        stack.push(1);
        stack.push(2);
        stack.push(3);
        assertEquals(List.of(1, 2, 3), stack.toList());
    }

    @Test
    void testContains() {
        Stack<String> stack = new Stack<>();
        stack.push("hello");
        stack.push("world");
        assertTrue(stack.contains("hello"));
        assertFalse(stack.contains("foo"));
    }

    @Test
    void testFromList() {
        List<Integer> list = List.of(10, 20, 30);
        Stack<Integer> stack = Stack.fromList(list);
        assertEquals(3, stack.size());
        assertEquals(30, stack.pop());
        assertEquals(20, stack.pop());
        assertEquals(10, stack.pop());
    }

    // ---- MinStack tests ----

    @Test
    void testMinStackBasic() {
        Stack.MinStack<Integer> ms = new Stack.MinStack<>();
        ms.push(5);
        assertEquals(5, ms.getMin());
        ms.push(3);
        assertEquals(3, ms.getMin());
        ms.push(7);
        assertEquals(3, ms.getMin());
    }

    @Test
    void testMinStackPopUpdatesMin() {
        Stack.MinStack<Integer> ms = new Stack.MinStack<>();
        ms.push(2);
        ms.push(1);
        ms.push(3);
        assertEquals(1, ms.getMin());
        ms.pop(); // remove 3
        assertEquals(1, ms.getMin());
        ms.pop(); // remove 1
        assertEquals(2, ms.getMin());
    }

    @Test
    void testMinStackWithDuplicates() {
        Stack.MinStack<Integer> ms = new Stack.MinStack<>();
        ms.push(1);
        ms.push(1);
        ms.push(2);
        assertEquals(1, ms.getMin());
        ms.pop(); // remove 2
        assertEquals(1, ms.getMin());
        ms.pop(); // remove 1
        assertEquals(1, ms.getMin());
        ms.pop(); // remove 1
        assertTrue(ms.isEmpty());
    }

    @Test
    void testMinStackEmpty() {
        Stack.MinStack<Integer> ms = new Stack.MinStack<>();
        assertThrows(NoSuchElementException.class, ms::getMin);
        assertThrows(NoSuchElementException.class, ms::pop);
        assertThrows(NoSuchElementException.class, ms::peek);
    }

    @Test
    void testMinStackPeek() {
        Stack.MinStack<Integer> ms = new Stack.MinStack<>();
        ms.push(10);
        ms.push(5);
        assertEquals(5, ms.peek());
        assertEquals(2, ms.size());
    }

    // ---- MaxStack tests ----

    @Test
    void testMaxStackBasic() {
        Stack.MaxStack<Integer> ms = new Stack.MaxStack<>();
        ms.push(1);
        assertEquals(1, ms.getMax());
        ms.push(5);
        assertEquals(5, ms.getMax());
        ms.push(3);
        assertEquals(5, ms.getMax());
    }

    @Test
    void testMaxStackPopUpdatesMax() {
        Stack.MaxStack<Integer> ms = new Stack.MaxStack<>();
        ms.push(3);
        ms.push(5);
        ms.push(2);
        assertEquals(5, ms.getMax());
        ms.pop(); // remove 2
        assertEquals(5, ms.getMax());
        ms.pop(); // remove 5
        assertEquals(3, ms.getMax());
    }

    @Test
    void testMaxStackWithDuplicates() {
        Stack.MaxStack<Integer> ms = new Stack.MaxStack<>();
        ms.push(5);
        ms.push(5);
        ms.push(3);
        assertEquals(5, ms.getMax());
        ms.pop(); // remove 3
        assertEquals(5, ms.getMax());
        ms.pop(); // remove 5
        assertEquals(5, ms.getMax());
    }

    @Test
    void testMaxStackEmpty() {
        Stack.MaxStack<Integer> ms = new Stack.MaxStack<>();
        assertThrows(NoSuchElementException.class, ms::getMax);
        assertThrows(NoSuchElementException.class, ms::pop);
        assertThrows(NoSuchElementException.class, ms::peek);
    }

    @Test
    void testMaxStackPeek() {
        Stack.MaxStack<String> ms = new Stack.MaxStack<>();
        ms.push("apple");
        ms.push("banana");
        assertEquals("banana", ms.peek());
        assertEquals("banana", ms.getMax());
    }

    // ---- isBalancedParentheses tests ----

    @Test
    void testBalancedParenthesesValid() {
        assertTrue(Stack.isBalancedParentheses("()"));
        assertTrue(Stack.isBalancedParentheses("()[]{}"));
        assertTrue(Stack.isBalancedParentheses("{[()]}"));
        assertTrue(Stack.isBalancedParentheses(""));
        assertTrue(Stack.isBalancedParentheses("({[]})"));
    }

    @Test
    void testBalancedParenthesesInvalid() {
        assertFalse(Stack.isBalancedParentheses("("));
        assertFalse(Stack.isBalancedParentheses(")"));
        assertFalse(Stack.isBalancedParentheses("([)]"));
        assertFalse(Stack.isBalancedParentheses("{[}]"));
        assertFalse(Stack.isBalancedParentheses("((())"));
    }

    @Test
    void testBalancedParenthesesWithOtherChars() {
        assertTrue(Stack.isBalancedParentheses("a(b[c]d)e"));
        assertFalse(Stack.isBalancedParentheses("a(b[c]d"));
    }

    // ---- evaluatePostfix tests ----

    @Test
    void testEvaluatePostfixAddition() {
        assertEquals(5, Stack.evaluatePostfix("2 3 +"));
    }

    @Test
    void testEvaluatePostfixSubtraction() {
        assertEquals(1, Stack.evaluatePostfix("3 2 -"));
    }

    @Test
    void testEvaluatePostfixMultiplication() {
        assertEquals(12, Stack.evaluatePostfix("3 4 *"));
    }

    @Test
    void testEvaluatePostfixDivision() {
        assertEquals(3, Stack.evaluatePostfix("6 2 /"));
    }

    @Test
    void testEvaluatePostfixComplex() {
        // (2 + 3) * 4 = 20
        assertEquals(20, Stack.evaluatePostfix("2 3 + 4 *"));
        // 5 + ((1 + 2) * 4) - 3 = 14
        assertEquals(14, Stack.evaluatePostfix("5 1 2 + 4 * + 3 -"));
    }

    @Test
    void testEvaluatePostfixSingleNumber() {
        assertEquals(42, Stack.evaluatePostfix("42"));
    }

    // ---- nextGreaterElement tests ----

    @Test
    void testNextGreaterElementBasic() {
        assertArrayEquals(new int[]{5, 5, -1}, Stack.nextGreaterElement(new int[]{2, 1, 5}));
    }

    @Test
    void testNextGreaterElementDescending() {
        assertArrayEquals(new int[]{-1, -1, -1}, Stack.nextGreaterElement(new int[]{5, 4, 3}));
    }

    @Test
    void testNextGreaterElementAscending() {
        assertArrayEquals(new int[]{2, 3, 4, -1}, Stack.nextGreaterElement(new int[]{1, 2, 3, 4}));
    }

    @Test
    void testNextGreaterElementMixed() {
        assertArrayEquals(new int[]{5, 25, 25, -1}, Stack.nextGreaterElement(new int[]{4, 5, 2, 25}));
    }

    @Test
    void testNextGreaterElementEmpty() {
        assertArrayEquals(new int[]{}, Stack.nextGreaterElement(new int[]{}));
    }

    @Test
    void testNextGreaterElementSingle() {
        assertArrayEquals(new int[]{-1}, Stack.nextGreaterElement(new int[]{7}));
    }
}
