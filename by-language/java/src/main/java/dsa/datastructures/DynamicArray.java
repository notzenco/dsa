package dsa.datastructures;

import java.util.ArrayList;
import java.util.List;
import java.util.NoSuchElementException;

public class DynamicArray<T> {

    public static final int DEFAULT_CAPACITY = 8;
    public static final int GROWTH_FACTOR = 2;
    public static final double SHRINK_THRESHOLD = 0.25;

    private Object[] data;
    private int size;
    private int capacity;

    public DynamicArray() {
        this.capacity = DEFAULT_CAPACITY;
        this.data = new Object[capacity];
        this.size = 0;
    }

    public DynamicArray(int initialCapacity) {
        if (initialCapacity <= 0) {
            throw new IllegalArgumentException("Capacity must be positive");
        }
        this.capacity = initialCapacity;
        this.data = new Object[capacity];
        this.size = 0;
    }

    @SuppressWarnings("unchecked")
    public T get(int index) {
        if (index < 0 || index >= size) {
            throw new IndexOutOfBoundsException("Index: " + index + ", Size: " + size);
        }
        return (T) data[index];
    }

    public void set(int index, T value) {
        if (index < 0 || index >= size) {
            throw new IndexOutOfBoundsException("Index: " + index + ", Size: " + size);
        }
        data[index] = value;
    }

    public void append(T value) {
        ensureCapacity();
        data[size] = value;
        size++;
    }

    public void insert(int index, T value) {
        if (index < 0 || index > size) {
            throw new IndexOutOfBoundsException("Index: " + index + ", Size: " + size);
        }
        ensureCapacity();
        System.arraycopy(data, index, data, index + 1, size - index);
        data[index] = value;
        size++;
    }

    @SuppressWarnings("unchecked")
    public T pop(int index) {
        if (index < 0 || index >= size) {
            throw new IndexOutOfBoundsException("Index: " + index + ", Size: " + size);
        }
        T value = (T) data[index];
        System.arraycopy(data, index + 1, data, index, size - index - 1);
        data[size - 1] = null;
        size--;
        shrinkIfNeeded();
        return value;
    }

    public boolean remove(T value) {
        int index = find(value);
        if (index == -1) {
            return false;
        }
        pop(index);
        return true;
    }

    public int find(T value) {
        for (int i = 0; i < size; i++) {
            if (data[i] != null ? data[i].equals(value) : value == null) {
                return i;
            }
        }
        return -1;
    }

    public void clear() {
        for (int i = 0; i < size; i++) {
            data[i] = null;
        }
        size = 0;
        capacity = DEFAULT_CAPACITY;
        data = new Object[capacity];
    }

    public void reverse() {
        for (int i = 0; i < size / 2; i++) {
            Object temp = data[i];
            data[i] = data[size - 1 - i];
            data[size - 1 - i] = temp;
        }
    }

    @SuppressWarnings("unchecked")
    public List<T> toList() {
        List<T> result = new ArrayList<>();
        for (int i = 0; i < size; i++) {
            result.add((T) data[i]);
        }
        return result;
    }

    public static <T> DynamicArray<T> fromList(List<T> list) {
        DynamicArray<T> arr = new DynamicArray<>();
        for (T item : list) {
            arr.append(item);
        }
        return arr;
    }

    public int size() {
        return size;
    }

    public boolean isEmpty() {
        return size == 0;
    }

    public int capacity() {
        return capacity;
    }

    // ---- Private helpers ----

    private void ensureCapacity() {
        if (size == capacity) {
            capacity *= GROWTH_FACTOR;
            Object[] newData = new Object[capacity];
            System.arraycopy(data, 0, newData, 0, size);
            data = newData;
        }
    }

    private void shrinkIfNeeded() {
        if (capacity > DEFAULT_CAPACITY && size < capacity * SHRINK_THRESHOLD) {
            capacity = Math.max(DEFAULT_CAPACITY, capacity / GROWTH_FACTOR);
            Object[] newData = new Object[capacity];
            System.arraycopy(data, 0, newData, 0, size);
            data = newData;
        }
    }
}
