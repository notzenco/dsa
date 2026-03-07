package datastructures

import (
	"fmt"
	"hash/fnv"
)

const (
	htDefaultCapacity    = 16
	htLoadFactorChaining = 0.75
	htLoadFactorOpen     = 0.5
)

// --- Hash Table with Chaining ---

type htChainEntry[K comparable, V any] struct {
	Key   K
	Value V
}

type HashTableChaining[K comparable, V any] struct {
	buckets  [][]htChainEntry[K, V]
	size     int
	capacity int
}

func NewHashTableChaining[K comparable, V any]() *HashTableChaining[K, V] {
	return &HashTableChaining[K, V]{
		buckets:  make([][]htChainEntry[K, V], htDefaultCapacity),
		capacity: htDefaultCapacity,
	}
}

func (ht *HashTableChaining[K, V]) Size() int {
	return ht.size
}

func (ht *HashTableChaining[K, V]) IsEmpty() bool {
	return ht.size == 0
}

func (ht *HashTableChaining[K, V]) hash(key K) int {
	h := fnv.New64a()
	fmt.Fprint(h, key)
	return int(h.Sum64() % uint64(ht.capacity))
}

func (ht *HashTableChaining[K, V]) Put(key K, value V) {
	if float64(ht.size+1)/float64(ht.capacity) > htLoadFactorChaining {
		ht.resize(ht.capacity * 2)
	}

	idx := ht.hash(key)
	for i, entry := range ht.buckets[idx] {
		if entry.Key == key {
			ht.buckets[idx][i].Value = value
			return
		}
	}
	ht.buckets[idx] = append(ht.buckets[idx], htChainEntry[K, V]{key, value})
	ht.size++
}

func (ht *HashTableChaining[K, V]) Get(key K) (V, bool) {
	idx := ht.hash(key)
	for _, entry := range ht.buckets[idx] {
		if entry.Key == key {
			return entry.Value, true
		}
	}
	var zero V
	return zero, false
}

func (ht *HashTableChaining[K, V]) Delete(key K) bool {
	idx := ht.hash(key)
	for i, entry := range ht.buckets[idx] {
		if entry.Key == key {
			ht.buckets[idx] = append(ht.buckets[idx][:i], ht.buckets[idx][i+1:]...)
			ht.size--
			return true
		}
	}
	return false
}

func (ht *HashTableChaining[K, V]) Contains(key K) bool {
	_, ok := ht.Get(key)
	return ok
}

func (ht *HashTableChaining[K, V]) Keys() []K {
	keys := make([]K, 0, ht.size)
	for _, bucket := range ht.buckets {
		for _, entry := range bucket {
			keys = append(keys, entry.Key)
		}
	}
	return keys
}

func (ht *HashTableChaining[K, V]) Values() []V {
	values := make([]V, 0, ht.size)
	for _, bucket := range ht.buckets {
		for _, entry := range bucket {
			values = append(values, entry.Value)
		}
	}
	return values
}

func (ht *HashTableChaining[K, V]) Clear() {
	ht.buckets = make([][]htChainEntry[K, V], htDefaultCapacity)
	ht.capacity = htDefaultCapacity
	ht.size = 0
}

func (ht *HashTableChaining[K, V]) resize(newCapacity int) {
	oldBuckets := ht.buckets
	ht.buckets = make([][]htChainEntry[K, V], newCapacity)
	ht.capacity = newCapacity
	ht.size = 0

	for _, bucket := range oldBuckets {
		for _, entry := range bucket {
			ht.Put(entry.Key, entry.Value)
		}
	}
}

// --- Hash Table with Open Addressing (Linear Probing) ---

type htOpenEntry[K comparable, V any] struct {
	Key       K
	Value     V
	Occupied  bool
	Tombstone bool
}

type HashTableOpenAddressing[K comparable, V any] struct {
	entries  []htOpenEntry[K, V]
	size     int
	capacity int
}

func NewHashTableOpenAddressing[K comparable, V any]() *HashTableOpenAddressing[K, V] {
	return &HashTableOpenAddressing[K, V]{
		entries:  make([]htOpenEntry[K, V], htDefaultCapacity),
		capacity: htDefaultCapacity,
	}
}

func (ht *HashTableOpenAddressing[K, V]) Size() int {
	return ht.size
}

func (ht *HashTableOpenAddressing[K, V]) IsEmpty() bool {
	return ht.size == 0
}

func (ht *HashTableOpenAddressing[K, V]) hash(key K) int {
	h := fnv.New64a()
	fmt.Fprint(h, key)
	return int(h.Sum64() % uint64(ht.capacity))
}

func (ht *HashTableOpenAddressing[K, V]) Put(key K, value V) {
	if float64(ht.size+1)/float64(ht.capacity) > htLoadFactorOpen {
		ht.resize(ht.capacity * 2)
	}

	idx := ht.hash(key)
	for i := 0; i < ht.capacity; i++ {
		pos := (idx + i) % ht.capacity
		entry := &ht.entries[pos]
		if !entry.Occupied || entry.Tombstone {
			entry.Key = key
			entry.Value = value
			entry.Occupied = true
			entry.Tombstone = false
			ht.size++
			return
		}
		if entry.Key == key {
			entry.Value = value
			return
		}
	}
}

func (ht *HashTableOpenAddressing[K, V]) Get(key K) (V, bool) {
	idx := ht.hash(key)
	for i := 0; i < ht.capacity; i++ {
		pos := (idx + i) % ht.capacity
		entry := &ht.entries[pos]
		if !entry.Occupied {
			break
		}
		if !entry.Tombstone && entry.Key == key {
			return entry.Value, true
		}
	}
	var zero V
	return zero, false
}

func (ht *HashTableOpenAddressing[K, V]) Delete(key K) bool {
	idx := ht.hash(key)
	for i := 0; i < ht.capacity; i++ {
		pos := (idx + i) % ht.capacity
		entry := &ht.entries[pos]
		if !entry.Occupied {
			break
		}
		if !entry.Tombstone && entry.Key == key {
			entry.Tombstone = true
			ht.size--
			return true
		}
	}
	return false
}

func (ht *HashTableOpenAddressing[K, V]) Contains(key K) bool {
	_, ok := ht.Get(key)
	return ok
}

func (ht *HashTableOpenAddressing[K, V]) Clear() {
	ht.entries = make([]htOpenEntry[K, V], htDefaultCapacity)
	ht.capacity = htDefaultCapacity
	ht.size = 0
}

func (ht *HashTableOpenAddressing[K, V]) resize(newCapacity int) {
	oldEntries := ht.entries
	ht.entries = make([]htOpenEntry[K, V], newCapacity)
	ht.capacity = newCapacity
	ht.size = 0

	for _, entry := range oldEntries {
		if entry.Occupied && !entry.Tombstone {
			ht.Put(entry.Key, entry.Value)
		}
	}
}

// --- Hash Set ---

type HashSet[K comparable] struct {
	table *HashTableChaining[K, struct{}]
}

func NewHashSet[K comparable]() *HashSet[K] {
	return &HashSet[K]{table: NewHashTableChaining[K, struct{}]()}
}

func (s *HashSet[K]) Add(key K) {
	s.table.Put(key, struct{}{})
}

func (s *HashSet[K]) Contains(key K) bool {
	return s.table.Contains(key)
}

func (s *HashSet[K]) Remove(key K) bool {
	return s.table.Delete(key)
}

func (s *HashSet[K]) Size() int {
	return s.table.Size()
}

func (s *HashSet[K]) IsEmpty() bool {
	return s.table.IsEmpty()
}

func (s *HashSet[K]) Items() []K {
	return s.table.Keys()
}

func (s *HashSet[K]) Clear() {
	s.table.Clear()
}

// Union returns a new set with elements from both sets.
func (s *HashSet[K]) Union(other *HashSet[K]) *HashSet[K] {
	result := NewHashSet[K]()
	for _, k := range s.Items() {
		result.Add(k)
	}
	for _, k := range other.Items() {
		result.Add(k)
	}
	return result
}

// Intersection returns a new set with elements common to both sets.
func (s *HashSet[K]) Intersection(other *HashSet[K]) *HashSet[K] {
	result := NewHashSet[K]()
	for _, k := range s.Items() {
		if other.Contains(k) {
			result.Add(k)
		}
	}
	return result
}

// Difference returns a new set with elements in s but not in other.
func (s *HashSet[K]) Difference(other *HashSet[K]) *HashSet[K] {
	result := NewHashSet[K]()
	for _, k := range s.Items() {
		if !other.Contains(k) {
			result.Add(k)
		}
	}
	return result
}

// SymmetricDifference returns elements in either set but not both.
func (s *HashSet[K]) SymmetricDifference(other *HashSet[K]) *HashSet[K] {
	result := NewHashSet[K]()
	for _, k := range s.Items() {
		if !other.Contains(k) {
			result.Add(k)
		}
	}
	for _, k := range other.Items() {
		if !s.Contains(k) {
			result.Add(k)
		}
	}
	return result
}

// IsSubset checks if s is a subset of other.
func (s *HashSet[K]) IsSubset(other *HashSet[K]) bool {
	if s.Size() > other.Size() {
		return false
	}
	for _, k := range s.Items() {
		if !other.Contains(k) {
			return false
		}
	}
	return true
}

// IsSuperset checks if s is a superset of other.
func (s *HashSet[K]) IsSuperset(other *HashSet[K]) bool {
	return other.IsSubset(s)
}

// Equals checks if two sets contain the same elements.
func (s *HashSet[K]) Equals(other *HashSet[K]) bool {
	return s.Size() == other.Size() && s.IsSubset(other)
}

// LoadFactor returns the current load factor of the chaining hash table.
func (ht *HashTableChaining[K, V]) LoadFactor() float64 {
	return float64(ht.size) / float64(ht.capacity)
}

// Entries returns all key-value pairs.
func (ht *HashTableChaining[K, V]) Entries() []htChainEntry[K, V] {
	entries := make([]htChainEntry[K, V], 0, ht.size)
	for _, bucket := range ht.buckets {
		entries = append(entries, bucket...)
	}
	return entries
}

// GetOrDefault returns value for key, or default if not found.
func (ht *HashTableChaining[K, V]) GetOrDefault(key K, defaultVal V) V {
	if v, ok := ht.Get(key); ok {
		return v
	}
	return defaultVal
}

// PutIfAbsent inserts only if key doesn't exist, returns true if inserted.
func (ht *HashTableChaining[K, V]) PutIfAbsent(key K, value V) bool {
	if ht.Contains(key) {
		return false
	}
	ht.Put(key, value)
	return true
}

// BucketCount returns the number of non-empty buckets.
func (ht *HashTableChaining[K, V]) BucketCount() int {
	count := 0
	for _, bucket := range ht.buckets {
		if len(bucket) > 0 {
			count++
		}
	}
	return count
}

// MaxBucketSize returns the size of the largest bucket.
func (ht *HashTableChaining[K, V]) MaxBucketSize() int {
	maxSize := 0
	for _, bucket := range ht.buckets {
		if len(bucket) > maxSize {
			maxSize = len(bucket)
		}
	}
	return maxSize
}

// LoadFactor returns the current load factor of the open addressing hash table.
func (ht *HashTableOpenAddressing[K, V]) LoadFactor() float64 {
	return float64(ht.size) / float64(ht.capacity)
}

// Keys returns all keys in the open addressing hash table.
func (ht *HashTableOpenAddressing[K, V]) Keys() []K {
	keys := make([]K, 0, ht.size)
	for _, entry := range ht.entries {
		if entry.Occupied && !entry.Tombstone {
			keys = append(keys, entry.Key)
		}
	}
	return keys
}

// Values returns all values in the open addressing hash table.
func (ht *HashTableOpenAddressing[K, V]) Values() []V {
	values := make([]V, 0, ht.size)
	for _, entry := range ht.entries {
		if entry.Occupied && !entry.Tombstone {
			values = append(values, entry.Value)
		}
	}
	return values
}
