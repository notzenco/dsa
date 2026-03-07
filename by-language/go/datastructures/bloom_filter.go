package datastructures

import (
	"crypto/md5"
	"crypto/sha256"
	"encoding/binary"
	"math"
)

// BloomFilter is a space-efficient probabilistic data structure.
type BloomFilter struct {
	bits     []bool
	size     int
	numHash  int
	count    int
}

func NewBloomFilter(expectedItems int, falsePositiveRate float64) *BloomFilter {
	size := optimalSize(expectedItems, falsePositiveRate)
	numHash := optimalHashes(size, expectedItems)
	return &BloomFilter{
		bits:    make([]bool, size),
		size:    size,
		numHash: numHash,
	}
}

func NewBloomFilterWithSize(size, numHash int) *BloomFilter {
	return &BloomFilter{
		bits:    make([]bool, size),
		size:    size,
		numHash: numHash,
	}
}

func optimalSize(n int, p float64) int {
	m := -float64(n) * math.Log(p) / (math.Log(2) * math.Log(2))
	return int(math.Ceil(m))
}

func optimalHashes(m, n int) int {
	k := float64(m) / float64(n) * math.Log(2)
	return int(math.Max(1, math.Round(k)))
}

func (bf *BloomFilter) getHashes(item string) []int {
	h1 := md5Hash(item)
	h2 := sha256Hash(item)

	hashes := make([]int, bf.numHash)
	for i := 0; i < bf.numHash; i++ {
		combined := h1 + uint64(i)*h2
		hashes[i] = int(combined % uint64(bf.size))
	}
	return hashes
}

func md5Hash(s string) uint64 {
	h := md5.Sum([]byte(s))
	return binary.BigEndian.Uint64(h[:8])
}

func sha256Hash(s string) uint64 {
	h := sha256.Sum256([]byte(s))
	return binary.BigEndian.Uint64(h[:8])
}

func (bf *BloomFilter) Add(item string) {
	for _, idx := range bf.getHashes(item) {
		bf.bits[idx] = true
	}
	bf.count++
}

func (bf *BloomFilter) Contains(item string) bool {
	for _, idx := range bf.getHashes(item) {
		if !bf.bits[idx] {
			return false
		}
	}
	return true
}

func (bf *BloomFilter) Count() int {
	return bf.count
}

func (bf *BloomFilter) FalsePositiveRate() float64 {
	setBits := 0
	for _, b := range bf.bits {
		if b {
			setBits++
		}
	}
	ratio := float64(setBits) / float64(bf.size)
	return math.Pow(ratio, float64(bf.numHash))
}

// CountingBloomFilter supports deletions.
type CountingBloomFilter struct {
	counters []int
	size     int
	numHash  int
	count    int
}

func NewCountingBloomFilter(expectedItems int, falsePositiveRate float64) *CountingBloomFilter {
	size := optimalSize(expectedItems, falsePositiveRate)
	numHash := optimalHashes(size, expectedItems)
	return &CountingBloomFilter{
		counters: make([]int, size),
		size:     size,
		numHash:  numHash,
	}
}

func (cbf *CountingBloomFilter) getHashes(item string) []int {
	h1 := md5Hash(item)
	h2 := sha256Hash(item)

	hashes := make([]int, cbf.numHash)
	for i := 0; i < cbf.numHash; i++ {
		combined := h1 + uint64(i)*h2
		hashes[i] = int(combined % uint64(cbf.size))
	}
	return hashes
}

func (cbf *CountingBloomFilter) Add(item string) {
	for _, idx := range cbf.getHashes(item) {
		cbf.counters[idx]++
	}
	cbf.count++
}

func (cbf *CountingBloomFilter) Contains(item string) bool {
	for _, idx := range cbf.getHashes(item) {
		if cbf.counters[idx] == 0 {
			return false
		}
	}
	return true
}

func (cbf *CountingBloomFilter) Remove(item string) bool {
	if !cbf.Contains(item) {
		return false
	}
	for _, idx := range cbf.getHashes(item) {
		if cbf.counters[idx] > 0 {
			cbf.counters[idx]--
		}
	}
	cbf.count--
	return true
}

func (cbf *CountingBloomFilter) Count() int {
	return cbf.count
}

func (cbf *CountingBloomFilter) FalsePositiveRate() float64 {
	nonZero := 0
	for _, c := range cbf.counters {
		if c > 0 {
			nonZero++
		}
	}
	ratio := float64(nonZero) / float64(cbf.size)
	return math.Pow(ratio, float64(cbf.numHash))
}

// ScalableBloomFilter grows automatically when the current filter becomes too full.
type ScalableBloomFilter struct {
	filters           []*BloomFilter
	targetFPRate      float64
	growthFactor      float64
	initialCapacity   int
	totalCount        int
	maxFillRatio      float64
}

func NewScalableBloomFilter(initialCapacity int, targetFPRate float64) *ScalableBloomFilter {
	sbf := &ScalableBloomFilter{
		targetFPRate:    targetFPRate,
		growthFactor:    2.0,
		initialCapacity: initialCapacity,
		maxFillRatio:    0.5,
	}
	sbf.addFilter()
	return sbf
}

func (sbf *ScalableBloomFilter) addFilter() {
	capacity := sbf.initialCapacity
	for i := 0; i < len(sbf.filters); i++ {
		capacity = int(float64(capacity) * sbf.growthFactor)
	}
	// Each successive filter gets a tighter FP rate
	fpRate := sbf.targetFPRate * math.Pow(0.5, float64(len(sbf.filters)+1))
	if fpRate < 1e-10 {
		fpRate = 1e-10
	}
	sbf.filters = append(sbf.filters, NewBloomFilter(capacity, fpRate))
}

func (sbf *ScalableBloomFilter) Add(item string) {
	activeFilter := sbf.filters[len(sbf.filters)-1]
	// Check if active filter is too full
	fillRatio := float64(activeFilter.count) / float64(activeFilter.size)
	if fillRatio > sbf.maxFillRatio {
		sbf.addFilter()
		activeFilter = sbf.filters[len(sbf.filters)-1]
	}
	activeFilter.Add(item)
	sbf.totalCount++
}

func (sbf *ScalableBloomFilter) Contains(item string) bool {
	for _, f := range sbf.filters {
		if f.Contains(item) {
			return true
		}
	}
	return false
}

func (sbf *ScalableBloomFilter) Count() int {
	return sbf.totalCount
}

func (sbf *ScalableBloomFilter) NumFilters() int {
	return len(sbf.filters)
}

func (sbf *ScalableBloomFilter) FalsePositiveRate() float64 {
	// Combined FP rate: 1 - product(1 - fp_i)
	product := 1.0
	for _, f := range sbf.filters {
		product *= (1.0 - f.FalsePositiveRate())
	}
	return 1.0 - product
}

// BloomFilterUnion merges two bloom filters (must have same size and hash count).
func BloomFilterUnion(a, b *BloomFilter) *BloomFilter {
	if a.size != b.size || a.numHash != b.numHash {
		return nil
	}
	result := NewBloomFilterWithSize(a.size, a.numHash)
	for i := range a.bits {
		result.bits[i] = a.bits[i] || b.bits[i]
	}
	result.count = a.count + b.count // approximate
	return result
}

// BloomFilterIntersection creates an intersection of two bloom filters.
func BloomFilterIntersection(a, b *BloomFilter) *BloomFilter {
	if a.size != b.size || a.numHash != b.numHash {
		return nil
	}
	result := NewBloomFilterWithSize(a.size, a.numHash)
	for i := range a.bits {
		result.bits[i] = a.bits[i] && b.bits[i]
	}
	return result
}

// EstimateCount estimates the number of items based on bit fill ratio.
func (bf *BloomFilter) EstimateCount() float64 {
	setBits := 0
	for _, b := range bf.bits {
		if b {
			setBits++
		}
	}
	if setBits == 0 || setBits == bf.size {
		return float64(bf.count)
	}
	return -float64(bf.size) / float64(bf.numHash) * math.Log(1.0-float64(setBits)/float64(bf.size))
}

// Clear resets the bloom filter.
func (bf *BloomFilter) Clear() {
	for i := range bf.bits {
		bf.bits[i] = false
	}
	bf.count = 0
}

// Size returns the bit array size.
func (bf *BloomFilter) Size() int {
	return bf.size
}

// NumHashFunctions returns the number of hash functions.
func (bf *BloomFilter) NumHashFunctions() int {
	return bf.numHash
}
