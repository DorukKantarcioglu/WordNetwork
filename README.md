# WordNetwork

This program initializes an adjacency matrix from five-letter words in *The Stanford GraphBase* and performs several operations regarding the words. Words (vertices) are connected by an undirected edge if they differ from each other by only one letter. **words_vertices.txt** file contains exactly 5757 words, whereas **words_edges.txt** file contains exactly 14135 edges, which include two words separated by a comma.

In addition to the adjacency matrix, a hash table is implemented in order to perform the word lookup more efficiently. The collision resolution strategy for the hash table is through separate chaining.
