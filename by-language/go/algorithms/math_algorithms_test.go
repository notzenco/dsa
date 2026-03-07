package algorithms

import "testing"

func TestGCD(t *testing.T) {
	tests := []struct {
		a, b, expected int
	}{
		{12, 8, 4},
		{100, 75, 25},
		{17, 13, 1},
		{0, 5, 5},
		{5, 0, 5},
		{-12, 8, 4},
	}
	for _, tt := range tests {
		result := GCD(tt.a, tt.b)
		if result != tt.expected {
			t.Errorf("GCD(%d, %d) = %d, expected %d", tt.a, tt.b, result, tt.expected)
		}
	}
}

func TestLCM(t *testing.T) {
	tests := []struct {
		a, b, expected int
	}{
		{4, 6, 12},
		{12, 15, 60},
		{7, 3, 21},
		{0, 5, 0},
	}
	for _, tt := range tests {
		result := LCM(tt.a, tt.b)
		if result != tt.expected {
			t.Errorf("LCM(%d, %d) = %d, expected %d", tt.a, tt.b, result, tt.expected)
		}
	}
}

func TestIsPrime(t *testing.T) {
	primes := []int{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 97}
	notPrimes := []int{-1, 0, 1, 4, 6, 8, 9, 15, 25, 100}

	for _, p := range primes {
		if !IsPrime(p) {
			t.Errorf("%d should be prime", p)
		}
	}
	for _, n := range notPrimes {
		if IsPrime(n) {
			t.Errorf("%d should not be prime", n)
		}
	}
}

func TestSieveOfEratosthenes(t *testing.T) {
	primes := SieveOfEratosthenes(30)
	expected := []int{2, 3, 5, 7, 11, 13, 17, 19, 23, 29}
	if len(primes) != len(expected) {
		t.Fatalf("expected %d primes, got %d", len(expected), len(primes))
	}
	for i, p := range expected {
		if primes[i] != p {
			t.Errorf("index %d: expected %d, got %d", i, p, primes[i])
		}
	}
}

func TestSieveSmall(t *testing.T) {
	if SieveOfEratosthenes(1) != nil {
		t.Error("sieve(1) should be nil")
	}
	primes := SieveOfEratosthenes(2)
	if len(primes) != 1 || primes[0] != 2 {
		t.Errorf("sieve(2) should be [2], got %v", primes)
	}
}

func TestPrimeFactors(t *testing.T) {
	tests := []struct {
		n        int
		expected []int
	}{
		{12, []int{2, 2, 3}},
		{100, []int{2, 2, 5, 5}},
		{17, []int{17}},
		{1, nil},
	}
	for _, tt := range tests {
		result := PrimeFactors(tt.n)
		if len(result) != len(tt.expected) {
			t.Errorf("PrimeFactors(%d) = %v, expected %v", tt.n, result, tt.expected)
			continue
		}
		for i := range result {
			if result[i] != tt.expected[i] {
				t.Errorf("PrimeFactors(%d)[%d] = %d, expected %d", tt.n, i, result[i], tt.expected[i])
			}
		}
	}
}

func TestFastPow(t *testing.T) {
	tests := []struct {
		base, exp, mod, expected int
	}{
		{2, 10, 1000, 24},
		{3, 5, 13, 9},
		{2, 0, 100, 1},
		{5, 3, 1, 0},
	}
	for _, tt := range tests {
		result := FastPow(tt.base, tt.exp, tt.mod)
		if result != tt.expected {
			t.Errorf("FastPow(%d, %d, %d) = %d, expected %d", tt.base, tt.exp, tt.mod, result, tt.expected)
		}
	}
}

func TestEulerTotient(t *testing.T) {
	tests := []struct {
		n, expected int
	}{
		{1, 1},
		{2, 1},
		{6, 2},
		{10, 4},
		{12, 4},
	}
	for _, tt := range tests {
		result := EulerTotient(tt.n)
		if result != tt.expected {
			t.Errorf("EulerTotient(%d) = %d, expected %d", tt.n, result, tt.expected)
		}
	}
}

func TestExtendedGCD(t *testing.T) {
	gcd, x, y := ExtendedGCD(35, 15)
	if gcd != 5 {
		t.Errorf("expected gcd 5, got %d", gcd)
	}
	if 35*x+15*y != 5 {
		t.Errorf("Bezout identity failed: 35*%d + 15*%d != 5", x, y)
	}
}

func TestModInverse(t *testing.T) {
	inv := ModInverse(3, 7)
	if inv != 5 {
		t.Errorf("expected 5, got %d", inv)
	}
	if (3*inv)%7 != 1 {
		t.Error("inverse verification failed")
	}

	inv = ModInverse(2, 4)
	if inv != -1 {
		t.Errorf("expected -1 (no inverse), got %d", inv)
	}
}

func TestIsPerfectSquare(t *testing.T) {
	squares := []int{0, 1, 4, 9, 16, 25, 100, 144}
	for _, s := range squares {
		if !IsPerfectSquare(s) {
			t.Errorf("%d should be a perfect square", s)
		}
	}
	notSquares := []int{-1, 2, 3, 5, 7, 8, 10, 99}
	for _, n := range notSquares {
		if IsPerfectSquare(n) {
			t.Errorf("%d should not be a perfect square", n)
		}
	}
}

func TestIntSqrt(t *testing.T) {
	tests := []struct {
		n, expected int
	}{
		{0, 0},
		{1, 1},
		{4, 2},
		{8, 2},
		{9, 3},
		{15, 3},
		{16, 4},
		{100, 10},
	}
	for _, tt := range tests {
		result := IntSqrt(tt.n)
		if result != tt.expected {
			t.Errorf("IntSqrt(%d) = %d, expected %d", tt.n, result, tt.expected)
		}
	}
}

func TestNChooseK(t *testing.T) {
	tests := []struct {
		n, k, expected int
	}{
		{5, 2, 10},
		{10, 3, 120},
		{0, 0, 1},
		{5, 0, 1},
		{5, 5, 1},
		{5, 6, 0},
	}
	for _, tt := range tests {
		result := NChooseK(tt.n, tt.k)
		if result != tt.expected {
			t.Errorf("C(%d,%d) = %d, expected %d", tt.n, tt.k, result, tt.expected)
		}
	}
}

func TestCatalanNumber(t *testing.T) {
	expected := []int{1, 1, 2, 5, 14, 42}
	for i, e := range expected {
		result := CatalanNumber(i)
		if result != e {
			t.Errorf("Catalan(%d) = %d, expected %d", i, result, e)
		}
	}
}

func TestCountDivisors(t *testing.T) {
	tests := []struct {
		n, expected int
	}{
		{1, 1},
		{6, 4},
		{12, 6},
		{28, 6},
		{0, 0},
	}
	for _, tt := range tests {
		result := CountDivisors(tt.n)
		if result != tt.expected {
			t.Errorf("CountDivisors(%d) = %d, expected %d", tt.n, result, tt.expected)
		}
	}
}

func TestSumDivisors(t *testing.T) {
	tests := []struct {
		n, expected int
	}{
		{1, 1},
		{6, 12},
		{12, 28},
		{0, 0},
	}
	for _, tt := range tests {
		result := SumDivisors(tt.n)
		if result != tt.expected {
			t.Errorf("SumDivisors(%d) = %d, expected %d", tt.n, result, tt.expected)
		}
	}
}

func TestIsPowerOfTwo(t *testing.T) {
	powers := []int{1, 2, 4, 8, 16, 32, 64, 128, 1024}
	for _, p := range powers {
		if !IsPowerOfTwo(p) {
			t.Errorf("%d should be power of 2", p)
		}
	}
	notPowers := []int{0, -1, 3, 5, 6, 7, 9, 15}
	for _, n := range notPowers {
		if IsPowerOfTwo(n) {
			t.Errorf("%d should not be power of 2", n)
		}
	}
}

func TestNextPowerOfTwo(t *testing.T) {
	tests := []struct {
		n, expected int
	}{
		{1, 1},
		{2, 2},
		{3, 4},
		{5, 8},
		{15, 16},
		{16, 16},
		{17, 32},
		{0, 1},
	}
	for _, tt := range tests {
		result := NextPowerOfTwo(tt.n)
		if result != tt.expected {
			t.Errorf("NextPowerOfTwo(%d) = %d, expected %d", tt.n, result, tt.expected)
		}
	}
}

func TestCountBits(t *testing.T) {
	tests := []struct {
		n, expected int
	}{
		{0, 0},
		{1, 1},
		{7, 3},
		{255, 8},
		{1023, 10},
	}
	for _, tt := range tests {
		result := CountBits(tt.n)
		if result != tt.expected {
			t.Errorf("CountBits(%d) = %d, expected %d", tt.n, result, tt.expected)
		}
	}
}

func TestReverseBits(t *testing.T) {
	// 0b00000000000000000000000000000001 -> 0b10000000000000000000000000000000
	result := ReverseBits(1)
	if result != 1<<31 {
		t.Errorf("ReverseBits(1) = %d, expected %d", result, uint32(1<<31))
	}
	// Self-inverse: reverse(reverse(n)) == n
	n := uint32(43261596)
	if ReverseBits(ReverseBits(n)) != n {
		t.Error("reverse should be self-inverse")
	}
}
