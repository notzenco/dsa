package dsa.datastructures;

import java.util.ArrayList;
import java.util.List;
import java.util.NoSuchElementException;

public class DoublyLinkedList<T> {

    public static class DNode<T> {
        public T data;
        public DNode<T> prev;
        public DNode<T> next;

        public DNode(T data) {
            this.data = data;
            this.prev = null;
            this.next = null;
        }
    }

    private DNode<T> head;
    private DNode<T> tail;
    private int size;

    public DoublyLinkedList() {
        this.head = null;
        this.tail = null;
        this.size = 0;
    }

    public void prepend(T value) {
        DNode<T> newNode = new DNode<>(value);
        if (head == null) {
            head = newNode;
            tail = newNode;
        } else {
            newNode.next = head;
            head.prev = newNode;
            head = newNode;
        }
        size++;
    }

    public void append(T value) {
        DNode<T> newNode = new DNode<>(value);
        if (tail == null) {
            head = newNode;
            tail = newNode;
        } else {
            newNode.prev = tail;
            tail.next = newNode;
            tail = newNode;
        }
        size++;
    }

    public void insert(int index, T value) {
        if (index < 0 || index > size) {
            throw new IndexOutOfBoundsException("Index: " + index + ", Size: " + size);
        }
        if (index == 0) {
            prepend(value);
            return;
        }
        if (index == size) {
            append(value);
            return;
        }
        DNode<T> current = getNode(index);
        DNode<T> newNode = new DNode<>(value);
        newNode.prev = current.prev;
        newNode.next = current;
        current.prev.next = newNode;
        current.prev = newNode;
        size++;
    }

    public T popFirst() {
        if (head == null) {
            throw new NoSuchElementException("List is empty");
        }
        T value = head.data;
        if (head == tail) {
            head = null;
            tail = null;
        } else {
            head = head.next;
            head.prev = null;
        }
        size--;
        return value;
    }

    public T popLast() {
        if (tail == null) {
            throw new NoSuchElementException("List is empty");
        }
        T value = tail.data;
        if (head == tail) {
            head = null;
            tail = null;
        } else {
            tail = tail.prev;
            tail.next = null;
        }
        size--;
        return value;
    }

    public T pop(int index) {
        if (index < 0 || index >= size) {
            throw new IndexOutOfBoundsException("Index: " + index + ", Size: " + size);
        }
        if (index == 0) {
            return popFirst();
        }
        if (index == size - 1) {
            return popLast();
        }
        DNode<T> node = getNode(index);
        node.prev.next = node.next;
        node.next.prev = node.prev;
        size--;
        return node.data;
    }

    public void removeNode(DNode<T> node) {
        if (node == null) {
            throw new IllegalArgumentException("Node cannot be null");
        }
        if (node == head && node == tail) {
            head = null;
            tail = null;
        } else if (node == head) {
            head = head.next;
            head.prev = null;
        } else if (node == tail) {
            tail = tail.prev;
            tail.next = null;
        } else {
            node.prev.next = node.next;
            node.next.prev = node.prev;
        }
        node.prev = null;
        node.next = null;
        size--;
    }

    public boolean remove(T value) {
        DNode<T> node = findNode(value);
        if (node == null) {
            return false;
        }
        removeNode(node);
        return true;
    }

    public int find(T value) {
        DNode<T> current = head;
        int index = 0;
        while (current != null) {
            if (current.data != null ? current.data.equals(value) : value == null) {
                return index;
            }
            current = current.next;
            index++;
        }
        return -1;
    }

    public DNode<T> findNode(T value) {
        DNode<T> current = head;
        while (current != null) {
            if (current.data != null ? current.data.equals(value) : value == null) {
                return current;
            }
            current = current.next;
        }
        return null;
    }

    public T get(int index) {
        if (index < 0 || index >= size) {
            throw new IndexOutOfBoundsException("Index: " + index + ", Size: " + size);
        }
        return getNode(index).data;
    }

    public void set(int index, T value) {
        if (index < 0 || index >= size) {
            throw new IndexOutOfBoundsException("Index: " + index + ", Size: " + size);
        }
        getNode(index).data = value;
    }

    public void clear() {
        head = null;
        tail = null;
        size = 0;
    }

    public void reverse() {
        if (size <= 1) {
            return;
        }
        DNode<T> current = head;
        DNode<T> temp;
        while (current != null) {
            temp = current.next;
            current.next = current.prev;
            current.prev = temp;
            current = temp;
        }
        temp = head;
        head = tail;
        tail = temp;
    }

    public List<T> toList() {
        List<T> result = new ArrayList<>();
        DNode<T> current = head;
        while (current != null) {
            result.add(current.data);
            current = current.next;
        }
        return result;
    }

    public List<T> toListReversed() {
        List<T> result = new ArrayList<>();
        DNode<T> current = tail;
        while (current != null) {
            result.add(current.data);
            current = current.prev;
        }
        return result;
    }

    public static <T> DoublyLinkedList<T> fromList(List<T> list) {
        DoublyLinkedList<T> dll = new DoublyLinkedList<>();
        for (T item : list) {
            dll.append(item);
        }
        return dll;
    }

    public int size() {
        return size;
    }

    public boolean isEmpty() {
        return size == 0;
    }

    public void moveToFront(DNode<T> node) {
        if (node == null) {
            throw new IllegalArgumentException("Node cannot be null");
        }
        if (node == head) {
            return;
        }
        // Unlink node
        if (node == tail) {
            tail = tail.prev;
            tail.next = null;
        } else {
            node.prev.next = node.next;
            node.next.prev = node.prev;
        }
        // Link at front
        node.prev = null;
        node.next = head;
        head.prev = node;
        head = node;
    }

    public void moveToBack(DNode<T> node) {
        if (node == null) {
            throw new IllegalArgumentException("Node cannot be null");
        }
        if (node == tail) {
            return;
        }
        // Unlink node
        if (node == head) {
            head = head.next;
            head.prev = null;
        } else {
            node.prev.next = node.next;
            node.next.prev = node.prev;
        }
        // Link at back
        node.next = null;
        node.prev = tail;
        tail.next = node;
        tail = node;
    }

    // ---- Private helpers ----

    private DNode<T> getNode(int index) {
        DNode<T> current;
        if (index < size / 2) {
            current = head;
            for (int i = 0; i < index; i++) {
                current = current.next;
            }
        } else {
            current = tail;
            for (int i = size - 1; i > index; i--) {
                current = current.prev;
            }
        }
        return current;
    }
}
