#	DIJKSTRA'S ALGORITHM for MIPS assembly
#	Román Castellarin
#	Binary Heap as Priority Queue. Total complexity O((E + V) log V)

#	CONVENTIONS:
#	- Outgoing edge e=(d,v) is represented by word e = (d<<16) | v
#	- For a heap node n, its childrens are c1 = n<<1 and c2 = (n<<1)|1,
#	  and its parent is p(n) = n>>1
#	- $S0 := O.S. return address
#	- $S1 := |V| Number of Vertices
#	- $S2 := |E| Number of Edges
#	- $S3 := Priority Queue size
#	- $S4 := 0x1 (constant one)
#	- $S5 := 0x1000 #0x10000000 (infinity)

.data
msgEnd:			.asciiz "Graph Done!\n"
msgtest1:
msgtest2: msgSplash:		.asciiz "DIJKSTRA'S ALGORITHM for MIPS assembly\nInsert |V|: "
msgNeighbor:	.asciiz "Insert vertex neighbors number and data: "
msgSource:		.asciiz "\nInsert source vertex or -1 for exit: "
msgInfinity:	.asciiz "INF"

# Can handle up to 131072 edges (d, u) and 65536 vertices (1.5 MiB of memory)
PQ: .word 0:6553	#0:131072		# Heap data
AL: .word 0:6553	#0:131072		# Adjacency Lists (Graph data)
G:	.word 0:6553	#0:65537		# Graph: Ptrs. to each vertex's adj. list
D:	.word 0:6553	#0:65536		# Array of distances (Answer)
