var table = make(map[int]int)

func entry(x int) int {
	table[1] = 1
	val, ok := table[x]
	if ok {
		return val
	}
	table[x] = 1 + entry(x-entry(entry(x-1)))
	return table[x]
}
