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
 * File:   ProcessFiles.h
 * Author: Theomeli
 *
 * Created on June 5, 2017, 1:43 PM
 */

#ifndef PROCESSFILES_H
#define PROCESSFILES_H
#include <fstream>
#include <string>
#include <list>
#include <map>

using namespace std;

class ProcessFiles {
public:
    ProcessFiles();
    ProcessFiles(const ProcessFiles& orig);
    ProcessFiles& operator =(const ProcessFiles& rightSide);
    virtual ~ProcessFiles();
    
    /**
     * getter for private member documentsText
     * @return the ifstream documentsText
     */
     ifstream& getDocumentsText() { return documentsText; }
    
    /**
     * getter for private member queriesText
     * @return the ifstream queriesText
     */
     ifstream& getQueriesText() { return queriesText; }
    
    /**
     * getter for private member documentsTokens
     * @return the list of documentsTokens
     */
     list<string>* getDocumentsTokens() const { return documentsTokens; }
    
    /**
     * getter for private member queriesTokens
     * @return the list of queriesTokens
     */
     list<string>* getQueriesTokens() const { return queriesTokens; }
    
    /**
     * getter for private member nDocuments
     * @return the number of documents
     */
     size_t getNDocuments() const { return nDocuments; }
    
    /**
     * getter for private member nQueries
     * @return the number of queries
     */
     size_t getNQueries() const { return nQueries; }
    
    /**
     * getter for private member nResponses
     * @return the number of responses
     */
     map<size_t, size_t> getNResponses() const { return nResponses; }
    
    /**
     * it reads the documents file. Firstly stores the number of documents
     * to variable nDocuments and then documents' tokens to variable 
     * documentsTokens according to documents' ids
     * @param stream is the stream to be read
     */
    void readDocumentsFile(ifstream& stream);
    
    /**
     * it reads the queries file. Firstly stores the number of queries to 
     * variable nQueries. Consequently stream gives two numbers to be read
     * which stored to map nResponses and finally queries' tokens which are
     * stores to variable queriesTokens according to queries' ids
     * @param stream is the stream to be read
     */
    void readQueriesFile(ifstream& stream);
private:
    //input file stream for the documents 
    ifstream									documentsText;
    //input file stream for the queries
    ifstream									queriesText;
    //number of documents
    //TODO: nDocuments and nQueries to be a 
    //variable map<bool, size_t> nDocs so that 
    //the bool part to answer the query isQuery
    size_t									nDocuments;
    //number of queries
    size_t									nQueries;
    //a list for each document containing its 
    //terms
    //TODO: replace pointer with a vector
    list<string>*								documentsTokens;
    //a list for each query containing its terms
    //TODO: replace pointer with a vector
    list<string>*								queriesTokens;
    //key: the query id, value: the number of 
    //responses that will be returned for this 
    //query
    map <size_t, size_t>							nResponses;
    
    /**
     * it transforms a string to its lower case
     * @param s the string to be transformed
     * @return the trasnformed string
     */
     string makeLower(const string& s);
    
    /**
     * it removes the punctuation from a string 
     * @param s the string to be checked for punctuation
     * @param punct the string of punctuation's characters
     * @return the string without punctuation
     */
     string removePunct(const string& s, const string& punct);
    
    /**
     * it uses the functions makeLower and removePunct
     * @param s the string to be transformed
     * @return a string without punctuation and in a lower case
     */
     string lowerRemovedPunct(const string& s);
};

#endif /* PROCESSFILES_H */

