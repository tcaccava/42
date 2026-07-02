def helper(i, n):
	if i > n:
		return
	print("Day", i)
	helper(i + 1, n)

def ft_count_harvest_recursive():
	n = int(input("Days until harvest: "))
	helper(1, n)
	print("Harvest time!")
	
