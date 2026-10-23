package algorithms

import "sort"

// Permutations returns all permutations of a slice.
func Permutations[T any](arr []T) [][]T {
	var result [][]T
	var backtrack func(int)
	backtrack = func(start int) {
		if start == len(arr) {
			perm := make([]T, len(arr))
			copy(perm, arr)
			result = append(result, perm)
			return
		}
		for i := start; i < len(arr); i++ {
			arr[start], arr[i] = arr[i], arr[start]
			backtrack(start + 1)
			arr[start], arr[i] = arr[i], arr[start]
		}
	}
	backtrack(0)
	return result
}

// Subsets returns all subsets (power set) of a slice.
func Subsets[T any](arr []T) [][]T {
	var result [][]T
	var current []T
	var backtrack func(int)
	backtrack = func(start int) {
		subset := make([]T, len(current))
		copy(subset, current)
		result = append(result, subset)

		for i := start; i < len(arr); i++ {
			current = append(current, arr[i])
			backtrack(i + 1)
			current = current[:len(current)-1]
		}
	}
	backtrack(0)
	return result
}

// CombinationSum returns all unique combinations where chosen numbers sum to target.
// Each number can be used unlimited times.
func CombinationSum(candidates []int, target int) [][]int {
	sort.Ints(candidates)
	var result [][]int
	var current []int

	var backtrack func(int, int)
	backtrack = func(start, remaining int) {
		if remaining == 0 {
			combo := make([]int, len(current))
			copy(combo, current)
			result = append(result, combo)
			return
		}
		for i := start; i < len(candidates); i++ {
			if candidates[i] > remaining {
				break
			}
			current = append(current, candidates[i])
			backtrack(i, remaining-candidates[i])
			current = current[:len(current)-1]
		}
	}
	backtrack(0, target)
	return result
}

// CombinationSum2 returns all unique combinations where chosen numbers sum to target.
// Each number can be used at most once.
func CombinationSum2(candidates []int, target int) [][]int {
	sort.Ints(candidates)
	var result [][]int
	var current []int

	var backtrack func(int, int)
	backtrack = func(start, remaining int) {
		if remaining == 0 {
			combo := make([]int, len(current))
			copy(combo, current)
			result = append(result, combo)
			return
		}
		for i := start; i < len(candidates); i++ {
			if candidates[i] > remaining {
				break
			}
			if i > start && candidates[i] == candidates[i-1] {
				continue
			}
			current = append(current, candidates[i])
			backtrack(i+1, remaining-candidates[i])
			current = current[:len(current)-1]
		}
	}
	backtrack(0, target)
	return result
}

// NQueens solves the N-Queens problem and returns all solutions.
// Each solution is a slice of column positions for queens in each row.
func NQueens(n int) [][]int {
	var result [][]int
	queens := make([]int, n)
	cols := make([]bool, n)
	diag1 := make([]bool, 2*n-1) // row + col
	diag2 := make([]bool, 2*n-1) // row - col + n - 1

	var backtrack func(int)
	backtrack = func(row int) {
		if row == n {
			sol := make([]int, n)
			copy(sol, queens)
			result = append(result, sol)
			return
		}
		for col := 0; col < n; col++ {
			if cols[col] || diag1[row+col] || diag2[row-col+n-1] {
				continue
			}
			queens[row] = col
			cols[col] = true
			diag1[row+col] = true
			diag2[row-col+n-1] = true
			backtrack(row + 1)
			cols[col] = false
			diag1[row+col] = false
			diag2[row-col+n-1] = false
		}
	}
	backtrack(0)
	return result
}

// SolveSudoku solves a 9x9 Sudoku puzzle in place. Returns true if solvable.
func SolveSudoku(board *[9][9]int) bool {
	for i := 0; i < 9; i++ {
		for j := 0; j < 9; j++ {
			if board[i][j] == 0 {
				for num := 1; num <= 9; num++ {
					if isValidSudoku(board, i, j, num) {
						board[i][j] = num
						if SolveSudoku(board) {
							return true
						}
						board[i][j] = 0
					}
				}
				return false
			}
		}
	}
	return true
}

func isValidSudoku(board *[9][9]int, row, col, num int) bool {
	for i := 0; i < 9; i++ {
		if board[row][i] == num || board[i][col] == num {
			return false
		}
	}
	startRow, startCol := (row/3)*3, (col/3)*3
	for i := startRow; i < startRow+3; i++ {
		for j := startCol; j < startCol+3; j++ {
			if board[i][j] == num {
				return false
			}
		}
	}
	return true
}

// GenerateParentheses generates all valid combinations of n pairs of parentheses.
func GenerateParentheses(n int) []string {
	var result []string
	var backtrack func([]byte, int, int)
	backtrack = func(current []byte, open, close int) {
		if len(current) == 2*n {
			result = append(result, string(current))
			return
		}
		if open < n {
			backtrack(append(current, '('), open+1, close)
		}
		if close < open {
			backtrack(append(current, ')'), open, close+1)
		}
	}
	backtrack([]byte{}, 0, 0)
	return result
}

// LetterCombinations returns all letter combinations for phone digits.
func LetterCombinations(digits string) []string {
	if len(digits) == 0 {
		return nil
	}

	phone := map[byte]string{
		'2': "abc", '3': "def", '4': "ghi", '5': "jkl",
		'6': "mno", '7': "pqrs", '8': "tuv", '9': "wxyz",
	}

	var result []string
	var backtrack func(int, []byte)
	backtrack = func(idx int, current []byte) {
		if idx == len(digits) {
			result = append(result, string(current))
			return
		}
		letters := phone[digits[idx]]
		for i := 0; i < len(letters); i++ {
			backtrack(idx+1, append(current, letters[i]))
		}
	}
	backtrack(0, []byte{})
	return result
}

// WordSearch checks if a word exists in a 2D grid of characters.
func WordSearch(board [][]byte, word string) bool {
	if len(board) == 0 || len(word) == 0 {
		return false
	}
	rows := len(board)
	visited := make([][]bool, rows)
	for r := range board {
		visited[r] = make([]bool, len(board[r]))
	}

	var dfs func(int, int, int) bool
	dfs = func(r, c, idx int) bool {
		if idx == len(word) {
			return true
		}
		if r < 0 || r >= rows || c < 0 || c >= len(board[r]) ||
			visited[r][c] || board[r][c] != word[idx] {
			return false
		}
		visited[r][c] = true
		found := dfs(r+1, c, idx+1) || dfs(r-1, c, idx+1) ||
			dfs(r, c+1, idx+1) || dfs(r, c-1, idx+1)
		visited[r][c] = false
		return found
	}

	for r := 0; r < rows; r++ {
		for c := range board[r] {
			if dfs(r, c, 0) {
				return true
			}
		}
	}
	return false
}

// Combinations returns all combinations of k elements from n (1 to n).
func Combinations(n, k int) [][]int {
	var result [][]int
	var current []int

	var backtrack func(int)
	backtrack = func(start int) {
		if len(current) == k {
			combo := make([]int, k)
			copy(combo, current)
			result = append(result, combo)
			return
		}
		for i := start; i <= n; i++ {
			current = append(current, i)
			backtrack(i + 1)
			current = current[:len(current)-1]
		}
	}
	backtrack(1)
	return result
}

// PalindromePartition partitions a string into all possible palindrome substrings.
func PalindromePartition(s string) [][]string {
	var result [][]string
	var current []string

	isPalin := func(str string) bool {
		for i, j := 0, len(str)-1; i < j; i, j = i+1, j-1 {
			if str[i] != str[j] {
				return false
			}
		}
		return true
	}

	var backtrack func(int)
	backtrack = func(start int) {
		if start == len(s) {
			part := make([]string, len(current))
			copy(part, current)
			result = append(result, part)
			return
		}
		for end := start + 1; end <= len(s); end++ {
			substr := s[start:end]
			if isPalin(substr) {
				current = append(current, substr)
				backtrack(end)
				current = current[:len(current)-1]
			}
		}
	}
	backtrack(0)
	return result
}

// RestoreIPAddresses returns all valid IP addresses from a digit string.
func RestoreIPAddresses(s string) []string {
	var result []string
	var segments []string

	var backtrack func(int, int)
	backtrack = func(start, numSegments int) {
		if numSegments == 4 && start == len(s) {
			ip := segments[0] + "." + segments[1] + "." + segments[2] + "." + segments[3]
			result = append(result, ip)
			return
		}
		if numSegments == 4 || start == len(s) {
			return
		}
		for length := 1; length <= 3 && start+length <= len(s); length++ {
			seg := s[start : start+length]
			if len(seg) > 1 && seg[0] == '0' {
				break // no leading zeros
			}
			num := 0
			for _, c := range seg {
				num = num*10 + int(c-'0')
			}
			if num > 255 {
				break
			}
			segments = append(segments, seg)
			backtrack(start+length, numSegments+1)
			segments = segments[:len(segments)-1]
		}
	}
	backtrack(0, 0)
	return result
}
