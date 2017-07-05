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
 * File:   ProcessFiles.h
 * Author: Totoros
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
     * 
     * @return 
     */
	ifstream& getDocumentsText() { return documentsText; }
    
    /**
     * 
     * @return 
     */
	ifstream& getQueriesText() { return queriesText; }
    
    /**
     * getter for documentsTokens
     * @return the list of documentsTokens
     */
	list<string>* getDocumentsTokens() const { return documentsTokens; }
    
    /**
     * getter for queriesTokens
     * @return the list of queriesTokens
     */
	list<string>* getQueriesTokens() const { return queriesTokens; }
    
    /**
     * 
     * @return 
     */
	int getNDocuments() const { return nDocuments; }
    
    /**
     * 
     * @return 
     */
	int getNQueries() const { return nQueries; }
    
    /**
     * 
     * @return 
     */
	map<int, int> getNResponses() const { return nResponses; }
    
    /**
     * 
     * @param stream 
     */
    void readDocumentsFile(ifstream& stream);
    
    /**
     * 
     * @param stream
     */
    void readQueriesFile(ifstream& stream);
private:
    ifstream documentsText;
    ifstream queriesText;
    int nDocuments;
    int nQueries;
    list<string>* documentsTokens;
    list<string>* queriesTokens;
    //key: the query id, value: the number of responses will be returned for this query
    map <int, int> nResponses;
    
    /**
     * 
     * @param s
     * @return 
     */
    string makeLower(const string& s);
    
    /**
     * 
     * @param s
     * @param punct
     * @return 
     */
    string removePunct(const string& s, const string& punct);
    
    /**
     * 
     * @param s
     * @return 
     */
    string lowerRemovedPunct(const string& s);
};

#endif /* PROCESSFILES_H */

