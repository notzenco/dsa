package dsa.datastructures;

import java.util.*;
import java.util.stream.Collectors;

public class HashTable {

    // ========== Entry ==========
    public static class Entry<K, V> {
        private final K key;
        private V value;

        public Entry(K key, V value) {
            this.key = key;
            this.value = value;
        }

        public K getKey() { return key; }
        public V getValue() { return value; }
        public void setValue(V value) { this.value = value; }

        @Override
        public String toString() {
            return "Entry{" + key + "=" + value + "}";
        }
    }

    // ========== HashTableChaining ==========
    public static class HashTableChaining<K, V> {
        private static final int DEFAULT_CAPACITY = 16;
        private static final double LOAD_FACTOR_THRESHOLD = 0.75;

        private List<List<Entry<K, V>>> buckets;
        private int size;
        private int capacity;

        public HashTableChaining() {
            this(DEFAULT_CAPACITY);
        }

        public HashTableChaining(int capacity) {
            this.capacity = capacity;
            this.size = 0;
            this.buckets = new ArrayList<>(capacity);
            for (int i = 0; i < capacity; i++) {
                buckets.add(new ArrayList<>());
            }
        }

        private int hash(K key) {
            return Math.abs(key.hashCode() % capacity);
        }

        public void put(K key, V value) {
            if (loadFactor() >= LOAD_FACTOR_THRESHOLD) {
                resize();
            }
            int index = hash(key);
            List<Entry<K, V>> bucket = buckets.get(index);
            for (Entry<K, V> entry : bucket) {
                if (entry.getKey().equals(key)) {
                    entry.setValue(value);
                    return;
                }
            }
            bucket.add(new Entry<>(key, value));
            size++;
        }

        public V get(K key) {
            int index = hash(key);
            List<Entry<K, V>> bucket = buckets.get(index);
            for (Entry<K, V> entry : bucket) {
                if (entry.getKey().equals(key)) {
                    return entry.getValue();
                }
            }
            return null;
        }

        public boolean remove(K key) {
            int index = hash(key);
            List<Entry<K, V>> bucket = buckets.get(index);
            Iterator<Entry<K, V>> it = bucket.iterator();
            while (it.hasNext()) {
                Entry<K, V> entry = it.next();
                if (entry.getKey().equals(key)) {
                    it.remove();
                    size--;
                    return true;
                }
            }
            return false;
        }

        public boolean containsKey(K key) {
            int index = hash(key);
            for (Entry<K, V> entry : buckets.get(index)) {
                if (entry.getKey().equals(key)) {
                    return true;
                }
            }
            return false;
        }

        public List<K> keys() {
            List<K> result = new ArrayList<>();
            for (List<Entry<K, V>> bucket : buckets) {
                for (Entry<K, V> entry : bucket) {
                    result.add(entry.getKey());
                }
            }
            return result;
        }

        public List<V> values() {
            List<V> result = new ArrayList<>();
            for (List<Entry<K, V>> bucket : buckets) {
                for (Entry<K, V> entry : bucket) {
                    result.add(entry.getValue());
                }
            }
            return result;
        }

        public List<Entry<K, V>> items() {
            List<Entry<K, V>> result = new ArrayList<>();
            for (List<Entry<K, V>> bucket : buckets) {
                result.addAll(bucket);
            }
            return result;
        }

        public void clear() {
            for (List<Entry<K, V>> bucket : buckets) {
                bucket.clear();
            }
            size = 0;
        }

        public int size() { return size; }
        public boolean isEmpty() { return size == 0; }
        public double loadFactor() { return (double) size / capacity; }
        public int capacity() { return capacity; }

        private void resize() {
            int newCapacity = capacity * 2;
            List<List<Entry<K, V>>> oldBuckets = buckets;
            buckets = new ArrayList<>(newCapacity);
            capacity = newCapacity;
            size = 0;
            for (int i = 0; i < newCapacity; i++) {
                buckets.add(new ArrayList<>());
            }
            for (List<Entry<K, V>> bucket : oldBuckets) {
                for (Entry<K, V> entry : bucket) {
                    put(entry.getKey(), entry.getValue());
                }
            }
        }
    }

    // ========== HashTableOpenAddressing ==========
    public static class HashTableOpenAddressing<K, V> {
        private static final int DEFAULT_CAPACITY = 16;
        private static final double LOAD_FACTOR_THRESHOLD = 0.5;

        private Object[] keys;
        private Object[] values;
        private boolean[] occupied;
        private boolean[] tombstones;
        private int size;
        private int capacity;

        public HashTableOpenAddressing() {
            this(DEFAULT_CAPACITY);
        }

        public HashTableOpenAddressing(int capacity) {
            this.capacity = capacity;
            this.size = 0;
            this.keys = new Object[capacity];
            this.values = new Object[capacity];
            this.occupied = new boolean[capacity];
            this.tombstones = new boolean[capacity];
        }

        private int hash(K key) {
            return Math.abs(key.hashCode() % capacity);
        }

        public void put(K key, V value) {
            if (loadFactor() >= LOAD_FACTOR_THRESHOLD) {
                resize();
            }
            int index = hash(key);
            int firstTombstone = -1;
            for (int i = 0; i < capacity; i++) {
                int probe = (index + i) % capacity;
                if (tombstones[probe]) {
                    if (firstTombstone == -1) {
                        firstTombstone = probe;
                    }
                    continue;
                }
                if (!occupied[probe]) {
                    int insertAt = (firstTombstone != -1) ? firstTombstone : probe;
                    keys[insertAt] = key;
                    values[insertAt] = value;
                    occupied[insertAt] = true;
                    tombstones[insertAt] = false;
                    size++;
                    return;
                }
                if (keys[probe].equals(key)) {
                    values[probe] = value;
                    return;
                }
            }
            // Should not reach here if load factor is maintained
            if (firstTombstone != -1) {
                keys[firstTombstone] = key;
                values[firstTombstone] = value;
                occupied[firstTombstone] = true;
                tombstones[firstTombstone] = false;
                size++;
            }
        }

        @SuppressWarnings("unchecked")
        public V get(K key) {
            int index = hash(key);
            for (int i = 0; i < capacity; i++) {
                int probe = (index + i) % capacity;
                if (!occupied[probe] && !tombstones[probe]) {
                    return null;
                }
                if (occupied[probe] && !tombstones[probe] && keys[probe].equals(key)) {
                    return (V) values[probe];
                }
            }
            return null;
        }

        public boolean remove(K key) {
            int index = hash(key);
            for (int i = 0; i < capacity; i++) {
                int probe = (index + i) % capacity;
                if (!occupied[probe] && !tombstones[probe]) {
                    return false;
                }
                if (occupied[probe] && !tombstones[probe] && keys[probe].equals(key)) {
                    occupied[probe] = false;
                    tombstones[probe] = true;
                    keys[probe] = null;
                    values[probe] = null;
                    size--;
                    return true;
                }
            }
            return false;
        }

        public boolean containsKey(K key) {
            return get(key) != null;
        }

        @SuppressWarnings("unchecked")
        public List<K> keys() {
            List<K> result = new ArrayList<>();
            for (int i = 0; i < capacity; i++) {
                if (occupied[i] && !tombstones[i]) {
                    result.add((K) keys[i]);
                }
            }
            return result;
        }

        @SuppressWarnings("unchecked")
        public List<V> values() {
            List<V> result = new ArrayList<>();
            for (int i = 0; i < capacity; i++) {
                if (occupied[i] && !tombstones[i]) {
                    result.add((V) values[i]);
                }
            }
            return result;
        }

        @SuppressWarnings("unchecked")
        public List<Entry<K, V>> items() {
            List<Entry<K, V>> result = new ArrayList<>();
            for (int i = 0; i < capacity; i++) {
                if (occupied[i] && !tombstones[i]) {
                    result.add(new Entry<>((K) keys[i], (V) values[i]));
                }
            }
            return result;
        }

        public void clear() {
            keys = new Object[capacity];
            values = new Object[capacity];
            occupied = new boolean[capacity];
            tombstones = new boolean[capacity];
            size = 0;
        }

        public int size() { return size; }
        public boolean isEmpty() { return size == 0; }
        public double loadFactor() { return (double) size / capacity; }
        public int capacity() { return capacity; }

        @SuppressWarnings("unchecked")
        private void resize() {
            int newCapacity = capacity * 2;
            Object[] oldKeys = keys;
            Object[] oldValues = values;
            boolean[] oldOccupied = occupied;
            boolean[] oldTombstones = tombstones;
            int oldCapacity = capacity;

            capacity = newCapacity;
            keys = new Object[newCapacity];
            values = new Object[newCapacity];
            occupied = new boolean[newCapacity];
            tombstones = new boolean[newCapacity];
            size = 0;

            for (int i = 0; i < oldCapacity; i++) {
                if (oldOccupied[i] && !oldTombstones[i]) {
                    put((K) oldKeys[i], (V) oldValues[i]);
                }
            }
        }
    }

    // ========== HashSet ==========
    public static class HashSet<K> {
        private static final Object PRESENT = new Object();
        private final HashTableChaining<K, Object> table;

        public HashSet() {
            table = new HashTableChaining<>();
        }

        public void add(K key) {
            table.put(key, PRESENT);
        }

        public boolean remove(K key) {
            return table.remove(key);
        }

        public boolean contains(K key) {
            return table.containsKey(key);
        }

        public int size() { return table.size(); }
        public boolean isEmpty() { return table.isEmpty(); }
        public void clear() { table.clear(); }

        public List<K> toList() {
            return table.keys();
        }

        public static <K> HashSet<K> fromList(List<K> list) {
            HashSet<K> set = new HashSet<>();
            for (K item : list) {
                set.add(item);
            }
            return set;
        }

        public HashSet<K> union(HashSet<K> other) {
            HashSet<K> result = new HashSet<>();
            for (K key : this.toList()) {
                result.add(key);
            }
            for (K key : other.toList()) {
                result.add(key);
            }
            return result;
        }

        public HashSet<K> intersection(HashSet<K> other) {
            HashSet<K> result = new HashSet<>();
            for (K key : this.toList()) {
                if (other.contains(key)) {
                    result.add(key);
                }
            }
            return result;
        }

        public HashSet<K> difference(HashSet<K> other) {
            HashSet<K> result = new HashSet<>();
            for (K key : this.toList()) {
                if (!other.contains(key)) {
                    result.add(key);
                }
            }
            return result;
        }
    }
}
