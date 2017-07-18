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
 * File:   TextRetrievalEngine.cpp
 * Author: Theomeli
 * 
 * Created on June 12, 2017, 10:02 PM
 */

#include "TextRetrievalEngine.h"

TextRetrievalEngine::TextRetrievalEngine() {
    p = new ProcessFiles;
}


TextRetrievalEngine::TextRetrievalEngine(ProcessFiles* p) {
    this -> p = new ProcessFiles;
    *(this->p) = *p;
    
    //initializing frequencies
    vector<map<string, size_t>> temp1(p->getNQueries() + 1, map<string, size_t>());
    frequencies[true] = temp1;
    vector<map<string, size_t>> temp2(p->getNDocuments() + 1, map<string, size_t>());
    frequencies[false] = temp2;

    //initializing weights
    vector<vector<double>> temp3(p->getNQueries() + 1, vector<double>());
    weights[true] = temp3;
    vector<vector<double>> temp4(p->getNDocuments() + 1, vector<double>());
    weights[false] = temp4;

    //initializing maxFrequencies
    vector<size_t> temp5(p->getNQueries() + 1, 1);
    maxFrequencies[true] = temp5;
    vector<size_t> temp6(p->getNDocuments() + 1, 1);
    maxFrequencies[false] = temp6;
}


TextRetrievalEngine::TextRetrievalEngine(const TextRetrievalEngine& orig)
    :frequencies(orig.getFrequencies()), maxFrequencies(orig.getMaxFrequencies()) {
    *(this->p) = *(orig.getP());
}


TextRetrievalEngine::~TextRetrievalEngine() {
    delete p;
}


size_t TextRetrievalEngine::computeFreqOfTerm(const string& term, size_t docId, bool isTermOfQuery) {
    list<string>::iterator iter;
    size_t frequency = 0;

    //update frequency for a document term
    if (!isTermOfQuery) {
        for (iter = p->getDocumentsTokens()[docId].begin(); iter != p->getDocumentsTokens()[docId].end(); iter++)
            if (*iter == term)
                frequency++;
    }
    //update frequency for a query term
    else {
        for (iter = p->getQueriesTokens()[docId].begin(); iter != p->getQueriesTokens()[docId].end(); iter++)
            if (*iter == term)
                frequency++;
    }
    
    return frequency;
}


void TextRetrievalEngine::computeFrequencies() {
    //initialize frequencies and insert terms of documents
    size_t nDocuments = p->getNDocuments();
    list<string>::iterator iter;
    for (size_t i = 0; i <= nDocuments; i++) 
        for (iter = p->getDocumentsTokens()[i].begin(); iter != p->getDocumentsTokens()[i].end(); iter++) {
            frequencies[false][i][*iter] = 0;
	    terms.insert(*iter);
        }
    
    //initialize frequencies and insert terms of queries
    size_t nQueries = p->getNQueries();
    for (size_t i = 0; i <= nQueries; i++) {
        for (iter = p->getQueriesTokens()[i].begin(); iter != p->getQueriesTokens()[i].end(); iter++) {
            terms.insert(*iter);
	    frequencies[true][i][*iter] = 0;
	}
    }
    
    for (size_t i = 0; i <= nDocuments; i++)
        for (iter = p->getDocumentsTokens()[i].begin(); iter != p->getDocumentsTokens()[i].end(); iter++)
            frequencies[false][i][*iter]++;
    
    for (size_t i = 0; i <= nQueries; i++)
        for (iter = p->getQueriesTokens()[i].begin(); iter != p->getQueriesTokens()[i].end(); iter++)
	    frequencies[true][i][*iter]++;
}


void TextRetrievalEngine::computeMaxFreq(size_t docId, bool isQuery) {
    map<string, size_t> myMap =  frequencies[isQuery][docId];
    
    for (auto const &ent1 : myMap)
	maxFrequencies[isQuery][docId] = max(ent1.second, maxFrequencies[isQuery][docId]);
}


size_t TextRetrievalEngine::getNDocsWithTerm(const string term) {
    size_t nDocsWithTerm = 0;
    size_t nDocuments = p->getNDocuments();
    for (size_t i = 0; i <= nDocuments; i++) {
	if (frequencies[false][i].find(term) != frequencies[false][i].end()) {
            nDocsWithTerm++;
        }
    }
    
    return nDocsWithTerm;
}


void TextRetrievalEngine::initializeIdfs() {
    vector<double> temp(terms.size() + 1, 0);
    idfs[true] = temp;
    idfs[false] = temp;
}


void TextRetrievalEngine::computeIdfs(bool isQuery) {
    size_t nDocuments = p->getNDocuments();
    size_t i = 1;
    for (auto const &term : terms) {
        double nt = getNDocsWithTerm(term);
	//nt == 0 outputs division with zero error
	if (nt == 0)
	    nt = 1;
	double temp = log(double(nDocuments / nt));
	if (isQuery == true)
	    idfs[isQuery][i] = temp;
	else {
	    idfs[isQuery][i] = temp / log(nDocuments); 
	    temp = temp / log(nDocuments);
	}
        i++;
    }
}


double TextRetrievalEngine::computeNormalizedFreq(const string term, const size_t documentId, bool isTermOfQuery) {
    double result = computeFreqOfTerm(term, documentId, isTermOfQuery) / double(maxFrequencies[isTermOfQuery][documentId]);
    if (!isTermOfQuery)
        return result;
    else
        return result *= 0.5;
}


void TextRetrievalEngine::computeDocWeight(const size_t documentId, bool isQuery) {
    size_t i = 1;
    for (auto const &term : terms) {
	weights[isQuery][documentId].push_back(computeNormalizedFreq(term, documentId, isQuery) * idfs[isQuery][i]);
	i++;
    }
}


void TextRetrievalEngine::computeDocsWeight(bool isQuery) {
    computeIdfs(isQuery);

    for (size_t i = 1; i < weights[isQuery].size(); i++) {
	computeMaxFreq(i, isQuery);
	computeDocWeight(i, isQuery);
    }
}


double TextRetrievalEngine::computeLength(vector<double> aVector) {
    double result = 0;
    vector<double>::iterator iter;
    for (iter = aVector.begin(); iter != aVector.end(); iter++)
	result += pow(*iter, 2);

    return sqrt(result);
}


double TextRetrievalEngine::computeCosine(vector<double> query, vector<double> document) {
    double result = 0;
    for (size_t i = 1; i < query.size(); i++) {
	result += query[i] * document[i];
    }

    return result / (computeLength(query) * computeLength(document));
}


priority_queue<pair<size_t, double>, vector<pair<size_t, double>>, Compare> TextRetrievalEngine::getSortedSimilarities(size_t queryId, size_t nResponses) {
    vector<double> temp(p->getNDocuments() + 1, 0);

    //it stores all the cosines between query with queryId and documents of the collection
    for (size_t i = 1; i <= p->getNDocuments(); i++) {
	temp[i] = computeCosine(weights[true][queryId], weights[false][i]);
    }

    //it makes pairs of term id and its associated weight ordered by the weight
    priority_queue<pair<size_t, double>, vector<pair<size_t, double>>, Compare> temp1;
    for (size_t i = 1; i < temp.size(); i++) {
	pair<size_t, double> p = make_pair(i, temp[i]);
	temp1.push(p);
    }

    //it pushes nResponses pairs to a queue named result
    priority_queue<pair<size_t, double>, vector<pair<size_t, double>>, Compare> result;
    for (size_t i = 0; i < nResponses; i++) {
	result.push(temp1.top());
	temp1.pop();
    }

    return result;
}


void TextRetrievalEngine::displayResults() {
    for (size_t i = 1; i <= p->getNQueries(); i++) {
	size_t nResponses = p->getNResponses()[i];
	cout << "Query to search: " << endl;
	list<string>::iterator iter;
	//it displays the query with id i
	for (iter = p->getQueriesTokens()[i].begin(); iter != p->getQueriesTokens()[i].end(); iter++)
	    cout << *iter << ' ';
	cout << endl << "===================" << endl;
	cout << "Returned documents:";
	cout << endl << "===================" << endl;
	priority_queue<pair<size_t, double>, vector<pair<size_t, double>>, Compare> simularities = getSortedSimilarities(i, nResponses);
	size_t size = simularities.size();
	for (size_t k = 0; k < size; k++) {
	    size_t docSize = 0;
	    pair<size_t, double> aPair = simularities.top();
	    simularities.pop();
	    //it displays the documents resulted from the call of getSortedSimilarities
	    //also stores document's size and use it for a formatted display
	    for (iter = p->getDocumentsTokens()[aPair.first].begin(); iter != p->getDocumentsTokens()[aPair.first].end(); iter++) {
		cout << *iter << ' ';
		docSize += iter->size();
		docSize++;
	    }
	    cout << setw(70 - docSize) << "(with weight " << aPair.second << ')' << endl; 
        }
        cout << endl;
    }
}
