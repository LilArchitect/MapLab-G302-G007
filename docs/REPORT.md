## Report

**1. Runtime complexity analysis of initializing the intersections map in Big-O. Include the average, best and worst cases if they are different.**

The build intersection map function iterates over the linked list of street segments calling the function hashmap insert twice, for node1_id and for node2_id.
The runtime complexity of the hashmap insert function is related to the hash function choosen. If it is well-distributed, the load factor will be low so the the average case would be O(1), the best case would be when the bucket is empty beeing also O(1) complexity. The worst scenario would be when lot of keys collide into the same bucket, forcing to go over all the list on every insert, the complexity would become O(n), n beeing the number of streets in the linked list.

Because of build intersection map calls hashmap insert 2n, the best and average case would be O(n): each insert is O(1), so n inserts is O(n). The worst case would be O(n²).

**2. Runtime complexity analysis of finding the coordinates of a street or place given the name in Big-O. Include the average, best and worst cases if they are different.**

The find place function calls normalize, get type and strips prefix and levenshtein, going over the linked list of places and comparing names.
Normalize, get type and strips prefix and levenshtein functions are all string related. The cost by node are O(k), O(k) and O(K^2) respectively, beeing k the length of the string. However, as the size of the strings are limited by a constant (#define SIZE 256), the cost is now O(256^2) which is considered constant asympotically O(1).

As a result of that, the runtime complexity of finding the coordinates of a street or place given the name is O(n), beeing n the number of places in the linked list. The best case is considered when the fist element on the list is the place that we are looking for, so complexity is O(1), the average case would be when the place is in the middle of the list, it would be O(n). 
Finally, the worst case would be when the place is not found on the first iteration so the function called place is called recursively again so complexity is O(n) + O(n) = O(2n), which overall is O(n).

**3. Runtime complexity analysis of your path-finding algorithm in Big-O. Include the average, best and worst cases if they are different.**

The path-finding algorithm choosen is BFS which uses a queue in which in each element stores a linked list of the path found since that moment and a hashmap function to know if a street has already been visited.

The BFS function calls all queue function: enqueue and free queue which have the complexity of O(n), n beeing the size of the queue and dequeue which has O(1).
Also calls is visted hash and visted hash map insert which have O(1) complexity and add to path which has a O(k) complexity where k is the current path length.

Overall, the best case would be O(1), when the origin and the destination are adjacents, the average case would be O((v + e)·(n + k)), where v are the number of street segments (vertices) and e the number of connections between them. For each vertice v and each connection e, it executes add to path and enqueue so the complexity for each connection is O(k + n) as these is executed e and v times, it turns O((v + e)·(n + k)). The worst case would be when k + n approach to v so the complexity is O((v + e)· v).

**4. A plot comparing the latency to find connected streets by sequentially looking through the list (lab 4) compared to using the intersections map (lab 5), depending on the map size.**

**Experimentally determine the results by measuring multiple times your program's behaviour with different relevant scenarios in the same machine. Include your raw data in the report, besides the plot.**

**Explain the results.**

### Raw data:
We have selected 5 random streets for each map:
| Map | Street | Number |
|-----|--------|--------|
| **xs_1** | C. del Baixant | 1 |
| | Av. Vertical | 1 |
| | C. Pompeu Fabra | 2 |
| | Av. Horitzontal | 1 |
| | C. Pompeu Fabra | 10 |
| **xs_2** | Avinguda Diagonal | 197 |
| | Rambla del Poblenou | 130 |
| | Carrer de la Llacuna | 118 |
| | Carrer de Pere IV | 151 |
| | Carrer dels Solsticis | 3 |
| **md_1** | Carrer de Pujades | 309 |
| | Carrer de Pamplona | 106 |
| | Carrer del Clot | 38 |
| | Passatge de Caminal | 24 |
| | Carrer d'Àlaba | 146 |
| **lg_1** | Carrer de Pujades | 309 |
| | Carrer de Sicília | 166 |
| | Carrer de Flandes | 9 |
| | Carrer de Pere IV | 47 |
| | Carrer del Consell de Cent | 426 |
| **xl_1** | Carrer de Ramón y Cajal | 24 |
| | Passeig dels Til·lers | 19 |
| | Carrer de Còrsega | 701 |
| | Carrer de Blasco de Garay | 10 |
| | Carrer del Marroc | 105 |
| **2xl_1** | Passeig de la Vall d'Hebron | 159 |
| | Avinguda de Catalunya | 72 |
| | Carrer de Palaudàries | 12 |
| | Carrer del Torrent de la Guineu | 116 |
| | Carrer de Pallars | 445 |


To compare the latency, we have applied the time function to connected_streets and connected_streets_fast. Asking for the same street, we compared the time of execution of 5 different streets from each map and we wrote down the time results on two separated charts, one for lab 4 results and the other for lab 5, as it is shown in the following images:
 

![alt text](./report_images/raw_data_cs_lab4.png)
![alt text](./report_images/raw_data_csf_lab5.png)


When we had mesured all the times, we calculated the average time execution for each map. The same as before, we did a two separated charts with the results:


![alt text](./report_images/raw_data_media_cs_lab4.png)
![alt text](./report_images/raw_data_media_csf_lab5.png)


Finally we made two plots with the results calculated:

![alt text](./report_images/plot_cs_lab4.png)
![alt text](./report_images/plot_csf_lab5.png)


In these two plots we can see a clear difference between the two labs. 
The sequential approach find_connected_streets gets slower as the size of the map grows, because it has to search among all streets to find it. With the smaller map, it takes around 68820 ns and 7130475 ns for the longest, which is a difference of 7061655 ns.

The intersection map find_connected_streets_fast whereas the sequential approach grows, this approach takes almost the same amount of time in the smallest map than in the longest map. The first one with 34860.2 ns and the last one with 45804 ns which is a difference of 10943.8 ns. As we can see there is a huge difference between the sequential approach and the hashmap. 


**5. A plot comparing the latency to find a path between two points finding connected streets sequentially looking through the list compared to using the intersections map, depending on the map size (but keeping the same origin and destination).**

**Experimentally determine the results by measuring multiple times your program's behaviour with different relevant scenarios in the same machine. Include your raw data in the report, besides the plot.**

**Explain the results.**

![alt text](./report_images/raw_data_bfs_linear.png)
![alt text](./report_images/raw_data_bfs_hash.png)
![alt text](./report_images/plot_bfs_linear.png)
![alt text](./report_images/plot_bfs_hash.png)

**6. A plot comparing the latency to find a path between two points finding connected streets sequentially looking through the list compared to using the intersections map, depending on the distance between the origin and destination (but using the same map).**

**Experimentally determine the results by measuring multiple times your program's behaviour with different relevant scenarios in the same machine. Include your raw data in the report, besides the plot.**

**Explain the results.**

**Fit a curve and justify it based on the runtime complexity from question 3.**

**7. Describe an improvement to the visited data structure in the BFS algorithm to improve latency.**

**Justify which data structure you would use / have used instead of a list to improve performance.**

**Describe its current runtime complexity and the improved runtime complexity.**

**Describe any trade-offs or downsides of your approach regarding latency or memory usage.**

**8. Describe an improvement to the algorithm to find the street segment given a latitude and longitude to improve its runtime complexity / latency.**

**Justify which data structure or algorithm you would use / have used to improve latency.**

**Describe its current runtime complexity and the improved runtime complexity.**

**Describe any trade-offs or downsides of your approach regarding latency or memory usage.**
