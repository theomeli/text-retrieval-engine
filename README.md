# Text Retrieval Engine

This project implements a parallel information retrieval engine and it will use the cores of a multicore system. 
The project supports two basic functions 1) the creation of an inverted index of a documents collection and 2) the processing of a set of queries. More specifically:

1. *Index creation*: a txt file is given as an input. In this file each line has the document id and subsequently its words. File's first line contains the number of documents that file has in total. For example: <br />
```
3
1 this is the first document
2 this is the second document
3 this is another document
```
The assumption we do is that index that will be created will be fit in RAM so that it doesn't need to be stored in external memory.

2. *Query Processing*: a txt file is given as an input. In this file each line has the query id, the number of answers that will be returned and query words. First line has the number of queries. For example: <br />
```
3
1 5 information retrieval
2 4 data structures
3 3 data mining
```
The query processing is done following the [vector model](https://en.wikipedia.org/wiki/Vector_space_model) which uses the cosine as similarity function and weights are creating according the formula TF x IDF. 

A sample of the results for the given documents and queries is given: <br />
![image1](https://user-images.githubusercontent.com/4678649/28319192-e08bda52-6bd5-11e7-87cd-20ba5afa4778.png)

TODOS: refactoring of class ProcessFiles, multithreading processing
