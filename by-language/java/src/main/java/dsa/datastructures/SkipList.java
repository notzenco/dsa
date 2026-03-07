package dsa.datastructures;

import java.util.*;

public class SkipList<K extends Comparable<K>, V> {

    private static final int MAX_LEVEL = 32;
    private static final double PROBABILITY = 0.5;

    public static class SkipListEntry<K, V> {
        private final K key;
        private V value;

        public SkipListEntry(K key, V value) {
            this.key = key;
            this.value = value;
        }

        public K getKey() { return key; }
        public V getValue() { return value; }
    }

    private static class Node<K, V> {
        K key;
        V value;
        List<Node<K, V>> forward;

        Node(K key, V value, int level) {
            this.key = key;
            this.value = value;
            this.forward = new ArrayList<>(Collections.nCopies(level + 1, null));
        }
    }

    private final Node<K, V> header;
    private int level;
    private int size;
    private final Random random;

    public SkipList() {
        this.header = new Node<>(null, null, MAX_LEVEL);
        this.level = 0;
        this.size = 0;
        this.random = new Random();
    }

    private int randomLevel() {
        int lvl = 0;
        while (lvl < MAX_LEVEL && random.nextDouble() < PROBABILITY) {
            lvl++;
        }
        return lvl;
    }

    public void insert(K key, V value) {
        @SuppressWarnings("unchecked")
        Node<K, V>[] update = new Node[MAX_LEVEL + 1];
        Node<K, V> current = header;

        for (int i = level; i >= 0; i--) {
            while (current.forward.get(i) != null &&
                   current.forward.get(i).key.compareTo(key) < 0) {
                current = current.forward.get(i);
            }
            update[i] = current;
        }

        Node<K, V> next = current.forward.get(0);

        if (next != null && next.key.compareTo(key) == 0) {
            // Key exists, update value
            next.value = value;
            return;
        }

        int newLevel = randomLevel();

        if (newLevel > level) {
            for (int i = level + 1; i <= newLevel; i++) {
                update[i] = header;
            }
            level = newLevel;
        }

        Node<K, V> newNode = new Node<>(key, value, newLevel);

        for (int i = 0; i <= newLevel; i++) {
            newNode.forward.set(i, update[i].forward.get(i));
            update[i].forward.set(i, newNode);
        }

        size++;
    }

    public Optional<V> search(K key) {
        Node<K, V> current = header;
        for (int i = level; i >= 0; i--) {
            while (current.forward.get(i) != null &&
                   current.forward.get(i).key.compareTo(key) < 0) {
                current = current.forward.get(i);
            }
        }
        Node<K, V> next = current.forward.get(0);
        if (next != null && next.key.compareTo(key) == 0) {
            return Optional.of(next.value);
        }
        return Optional.empty();
    }

    public boolean delete(K key) {
        @SuppressWarnings("unchecked")
        Node<K, V>[] update = new Node[MAX_LEVEL + 1];
        Node<K, V> current = header;

        for (int i = level; i >= 0; i--) {
            while (current.forward.get(i) != null &&
                   current.forward.get(i).key.compareTo(key) < 0) {
                current = current.forward.get(i);
            }
            update[i] = current;
        }

        Node<K, V> target = current.forward.get(0);

        if (target == null || target.key.compareTo(key) != 0) {
            return false;
        }

        for (int i = 0; i <= level; i++) {
            if (update[i].forward.get(i) != target) {
                break;
            }
            update[i].forward.set(i, target.forward.get(i));
        }

        while (level > 0 && header.forward.get(level) == null) {
            level--;
        }

        size--;
        return true;
    }

    public Optional<SkipListEntry<K, V>> min() {
        Node<K, V> first = header.forward.get(0);
        if (first == null) return Optional.empty();
        return Optional.of(new SkipListEntry<>(first.key, first.value));
    }

    public Optional<SkipListEntry<K, V>> max() {
        Node<K, V> current = header;
        for (int i = level; i >= 0; i--) {
            while (current.forward.get(i) != null) {
                current = current.forward.get(i);
            }
        }
        if (current == header) return Optional.empty();
        return Optional.of(new SkipListEntry<>(current.key, current.value));
    }

    public int size() { return size; }
    public boolean isEmpty() { return size == 0; }

    public void clear() {
        for (int i = 0; i <= MAX_LEVEL; i++) {
            header.forward.set(i, null);
        }
        level = 0;
        size = 0;
    }

    /**
     * Returns all entries with keys in [low, high] (inclusive).
     */
    public List<SkipListEntry<K, V>> range(K low, K high) {
        List<SkipListEntry<K, V>> result = new ArrayList<>();
        Node<K, V> current = header;

        // Find the first node >= low
        for (int i = level; i >= 0; i--) {
            while (current.forward.get(i) != null &&
                   current.forward.get(i).key.compareTo(low) < 0) {
                current = current.forward.get(i);
            }
        }

        current = current.forward.get(0);

        while (current != null && current.key.compareTo(high) <= 0) {
            result.add(new SkipListEntry<>(current.key, current.value));
            current = current.forward.get(0);
        }

        return result;
    }

    /**
     * Returns the entry with the greatest key less than or equal to the given key.
     */
    public Optional<SkipListEntry<K, V>> floor(K key) {
        Node<K, V> current = header;
        for (int i = level; i >= 0; i--) {
            while (current.forward.get(i) != null &&
                   current.forward.get(i).key.compareTo(key) <= 0) {
                current = current.forward.get(i);
            }
        }
        if (current == header) return Optional.empty();
        return Optional.of(new SkipListEntry<>(current.key, current.value));
    }

    /**
     * Returns the entry with the smallest key greater than or equal to the given key.
     */
    public Optional<SkipListEntry<K, V>> ceiling(K key) {
        Node<K, V> current = header;
        for (int i = level; i >= 0; i--) {
            while (current.forward.get(i) != null &&
                   current.forward.get(i).key.compareTo(key) < 0) {
                current = current.forward.get(i);
            }
        }
        Node<K, V> next = current.forward.get(0);
        if (next == null) return Optional.empty();
        return Optional.of(new SkipListEntry<>(next.key, next.value));
    }
}
