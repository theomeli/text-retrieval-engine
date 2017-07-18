/*
* The MIT License
*
* Copyright 2017 Theomeli.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

/* 
* File:   TextRetrievalEngine.h
* Author: Theomeli
*
* Created on June 12, 2017, 10:02 PM
*/

#ifndef TEXTRETRIEVALENGINE_H
#define TEXTRETRIEVALENGINE_H
#include "ProcessFiles.h"
#include "Compare.h"
#include <iostream>
#include <set>
#include <algorithm>
#include <queue>
#include <iomanip>

class TextRetrievalEngine {
public:
	TextRetrievalEngine();
	TextRetrievalEngine(ProcessFiles* p);
	TextRetrievalEngine(const TextRetrievalEngine& orig);
	virtual ~TextRetrievalEngine();

	/**
	* getter for private member terms
	* @return the set terms
	*/
	set<string> getTerms() const { return terms; };

	/**
	* getter for private member maxFrequencies
	* @return the map maxFrequencies
	*/
	map<bool, vector<size_t>> getMaxFrequencies() const { return maxFrequencies; };

	/**
	* getter for private member frequencies
	* @return the map frequencies
	*/
	map<bool, vector<map<string, size_t>>> getFrequencies() const { return frequencies; }

	/**
	* getter for private member p
	* @return the pointer p
	*/
	ProcessFiles* getP() const { return p; }

	/**
	* getter for private member weights
	* @return the vector weights
	*/
	vector<vector<double>> getWeights(bool isQuery) { return weights[isQuery]; };

	/**
	* It initializes the private members: frequenciesOfTerms (a list of maps) and terms (a set of strings)
	*/
	void computeFrequencies();

	/**
	* It initializes the private member idfs to two vectors of size terms.size()
	*/
	void initializeIdfs();

	/**
	* It computes the weights of all queries or documents calling
	* function computeDocWeight
	* @param isQuery true stands for query, false for document
	*/
	void computeDocsWeight(bool isQuery);

	/**
	* It displays the results. Given the queries and documents it is
	* displayed a list of queries and their associated documents sorted
	* by their weight
	*/
	void displayResults();

private:
	//it contains the data from queries and documents files
	ProcessFiles*								p;
	//it contains the idfs for each term contained in 
	//private member terms. Boolean part stands for
	//queries when it is true, documents when is false
	map<bool, vector<double>>						idfs;
	//a set of the terms appeared in documents and queries
	set<string>								terms;
	//it contains for each vector of terms the frequency
	//of the particular term. Boolean part stands for 
	//queries when it is true, documents when is false
	map<bool, vector<map<string, size_t>>>					frequencies;
	//it represents the frequency of the most often
	//appeared term in a vector of terms. Boolean part
	//stands for queries when it is true, documents when
	//is false
	map<bool, vector<size_t>>						maxFrequencies;
	//it represents the weight of a term in a particular
	//vector of terms. Boolean part stands for queries 
	//when it is true, documents when is false
	map<bool, vector<vector<double>>>					weights;

	/**
	* It computes the frequency of a particular term in a particular document or query
	* @param term is the term whose frequency is computed
	* @param docId is the document or query id where the term belongs
	* @param isTermOfQuery checks if term belongs in a query or in a document
	* @return the frequency of the term
	*/
	size_t computeFreqOfTerm(const string& term, size_t docId, bool isTermOfQuery);

	/**
	* It computes the greatest frequency of the terms in the current document or query
	* and store it to maxFrequencies private member
	* @param docId is the id of the document or query to search in
	* @param isQuery true stands for query and false for document
	*/
	void computeMaxFreq(size_t docId, bool isQuery);

	/**
	* It computes the number of documents with the current term
	* @param term is the term which the function searches among the documents
	* @return the number of documents with the current term
	*/
	size_t getNDocsWithTerm(const string term);

	/**
	* It computes the IDFs according to the formula IDF = ln(N/nt)/ln(N).
	* IDFs for queries are computed with the formula IDF = ln(N/nt)
	* N is the number of documents of the collection,
	* nt is the number of documents which contain the term.
	* These values are stored to the private member idfs.
	* @param isQuery checks if function will compute queries' or documents' idfs
	*/
	void computeIdfs(bool isQuery);

	/**
	* It computes the normalized frequency of a term according to the following formula:
	* normalized_ft,d = ft,d/maxx(fx,d), where ft,d is the frequency of the current term in 
	* the current document and maxx(fx,d) is the frequency of the term with the greatest 
	* frequency in the document. In the case of a term in a query it is normalized_ft,d = 0.5 * ft,d/maxx(fx,d) 
	* @param term is the term whose normalized frequency we count
	* @param documentId is the id of the document where the term belongs
	* @param isTermOfQuery checks if term belongs in a query or in a document
	* @return the normalized frequency
	*/
	double computeNormalizedFreq(const string term, const size_t documentId, bool isTermOfQuery);

	/**
	* It computes the weight of each term in a particular document according to the formula
	* TF * IDF, where TF = normalized_ft,d = ft,d / maxx(fx,d) and IDF = ln(N/nt)/ln(N).
	* In the case of a query it is TF = 0.5 * ft,q / maxx(fx,q) and IDF = ln(N/nt)
	* N is the number of documents of the collection,
	* nt is the number of documents which contain the term.
	* These values are stored to the private member weights.
	* @param documentId is the id of the document or query where the term belongs
	* @param isQuery checks if term belongs in a query or in a document
	*/
	void computeDocWeight(const size_t documentId, bool isQuery);

	/**
	* It computes the angle between two vectors using their inner product.
	* More precisely we use this function to find the similarity between 
	* two vectors: query and document. The larger is the computed value the 
	* more similar are the two vectors.
	* @param query is the vector with the weights of a query
	* @param document is the vector with the weights of a document
	* @return the computed angle's cosine between query and document
	*/
	double computeCosine(vector<double> query, vector<double> document);

	/**
	* It computes a vector's length according to Euclidean norm
	* @param aVector a vector whose length is computed
	* @return the computed length
	*/
	double computeLength(vector<double> aVector);

	/**
	* It computes a sorted by its weight structure which contains the weight and 
	* the associated document id
	* @param queryId is the query's id for which the weights are computed
	* @param nResponses the number of weights we need to store for this query
	* @return a structure of a priority list with pairs weight-docId
	*/
	priority_queue<pair<size_t, double>, vector<pair<size_t, double>>, Compare> getSortedSimilarities(size_t queryId, size_t nResponses);
};

#endif /* TEXTRETRIEVALENGINE_H */
