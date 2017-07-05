/*
* The MIT License
*
* Copyright 2017 Totoros.
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
* Author: Totoros
*
* Created on June 12, 2017, 10:02 PM
*/

#ifndef TEXTRETRIEVALENGINE_H
#define TEXTRETRIEVALENGINE_H
#include "ProcessFiles.h"
#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <vector>
#include <set>
#include <algorithm>

class TextRetrievalEngine {
public:
	TextRetrievalEngine();
	TextRetrievalEngine(ProcessFiles* p);
	TextRetrievalEngine(const TextRetrievalEngine& orig);
	virtual ~TextRetrievalEngine();

	/**
	* 
	* @return 
	*/
	set<string> getTerms() const { return terms; };

	/**
	* 
	* @return 
	*/
	vector<int> getMaxFrequency() const { return maxFrequency; };

	/**
	* 
	* @return 
	*/
	map<bool, vector<map<string, int>>> getFrequencies() const { return frequencies; }

	/**
	* 
	* @return 
	*/
	ProcessFiles* getP() const { return p; }

	/**
	*
	* @return
	*/
	vector<vector<double>> getWeights(bool isQuery) { return weights[isQuery]; };

	/**
	* 
	* @param isTermOfQuery
	*/
	void computeDocsWeight(bool isQuery);

	/**
	* It initializes the private members: frequenciesOfTerms (a list of maps) and terms (a set of strings)
	*/
	void computeFrequencies();

private:
	ProcessFiles*     p;

	vector<int>      maxFrequency;

	map<bool, vector<double>> idfs;

	set<string>       terms;
	map<bool, vector<map<string, int>>> frequencies;

	map<bool, vector<vector<double>>> weights;
	size_t* queriesIndicesNonZero;

	/**
	* It computes the frequency of a particular term in a particular document or query
	* @param term is the term whose frequency is computed
	* @param documentId is the document id where the term belongs
	* @param isTermOfQuery examines if term belongs in a query or in a document
	* @return the frequency of the term
	*/
	int computeFreqOfTerm(const string& term, int documentId, bool isTermOfQuery);

	/**
	* It computes the greatest frequency of the terms in the current document
	* @param documentId is the id of the document to search in
	* @return the greatest frequency
	*/
	void computeMaxFreq(int docId, bool isQuery);

	/**
	* It computes the number of documents with the current term
	* @param term is the term which the function searchs among the documents
	* @return the number of documents with the current term
	*/
	size_t getNDocsWithTerm(const string term);

	/**
	* It initializes the private member idfs to two vectors of size terms.size()
	*/
	void initializeIdfs();

	/**
	* It computes the IDF of the current term according to the formula IDF = ln(N/nt)/ln(N).
	* In the case of term in a query it is IDF = ln(N/nt)
	* N is the number of documents of the collection,
	* nt is the number of documents which contain the term
	* @param term is the term whose IDF we count
	* @param isTermOfQuery checks if term belongs in a query or in a document 
	* @return the IDF of the current term
	*/
	void computeIdfs(bool isQuery);

	/**
	* It computes the normalized frequency of a term according to the followning formula:
	* normalized ft,d = ft,d/maxx(fx,d), where ft,d is the frequency of the current term in 
	* the current document and maxx(fx,d) is the frequency of the term with the greatest 
	* frequency in the document. In the case of a term in a query it is normalized ft,d = 0.5 * ft,d/maxx(fx,d) 
	* @param term is the term whose normalized frequency we count
	* @param documentId is the id of the document where the term belongs
	* @param isTermOfQuery checks if term belongs in a query or in a document
	* @return the normalized frequency
	*/
	double computeNormalizedFreq(const string term, const int documentId, bool isTermOfQuery);

	/**
	* It computes the weight of each term in a particular document and is equal to
	* TF * IDF, where TF = normalized ft,d = ft,d / maxx(fx,d) and IDF = ln(N/nt)/ln(N).
	* In the case of query it is TF = 0.5 * ft,q / maxx(fx,q) and IDF = ln(N/nt)
	* N is the number of documents of the collection,
	* nt is the number of documents which contain the term
	* @param term is the term whose frequency we count
	* @param documentId is the id of the document where the term belongs
	* @param isTermOfQuery examines if term belongs in a query or in a document
	* @return the computed weight
	*/
	void computeDocWeight(const int documentId, bool isQuery);
};

#endif /* TEXTRETRIEVALENGINE_H */

