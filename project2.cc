/*
 * Copyright (C) Mohsen Zohrevandi, 2017
 *               Rida Bazzi 2019
 * Do not share this file with anyone
 */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "lexer.h"

using namespace std;

class LexicalAnalyzer lexer;

//Type deffination for Rule
typedef struct
{
	class Token LHS;
	vector <class Token> RHS;

}RuleType;

bool FoundInVector(string, int, vector<string>);

typedef vector <vector<string>> FirstType;
vector <RuleType> grammar;

vector <string> NonTermVector;
vector <string> TermVector;


void PrintGrammar(vector <RuleType> PrintRules)
{
	//print all the rules
	for (int PrintRulesIndex = 0; PrintRulesIndex < PrintRules.size(); PrintRulesIndex++)
	{
		cout << PrintRules[PrintRulesIndex].LHS.lexeme;
		cout << " -> ";
		if (PrintRules[PrintRulesIndex].RHS[0].token_type == HASH)
		{
			cout << "#";
		}
		else
		{
			if (PrintRules[PrintRulesIndex].RHS.size() > 0)
			{
				for (int ruleIndex = 0; ruleIndex < PrintRules[PrintRulesIndex].RHS.size(); ruleIndex++)
				{
					cout << PrintRules[PrintRulesIndex].RHS[ruleIndex].lexeme;
					cout << " ";
				}
			}
		}
		cout << "\n";

	}
}

/*void PrintFirst(FirstType FirstGram)
{
	//print all the iteams
	for (int PrintRulesIndex = 0; PrintRulesIndex < FirstGram.size(); PrintRulesIndex++)
	{
		cout << FirstGram[0][0];
		cout << " -> ";
		if (FirstGram[PrintRulesIndex][1] == "")
		{
			cout << "#";
		}
		else
		{
			
			for (int ruleIndex = 1; ruleIndex < FirstGram[PrintRulesIndex].size(); ruleIndex++)
			{
				cout << FirstGram[PrintRulesIndex][ruleIndex];
				cout << " ";
			}
			
		}
		cout << "\n";

	}
}*/

void PrintVector(vector<string> vectorToPrint)
{
	//printing for testing
	for (int vectorToPrintIndex = 0; vectorToPrintIndex < vectorToPrint.size(); vectorToPrintIndex++)
	{
		cout << vectorToPrint[vectorToPrintIndex] << " ";
	}
}

// read grammar
void ReadGrammar()
{
	RuleType tempRule;
	class Token myToken;

	myToken = lexer.GetToken();
	while (myToken.token_type != DOUBLEHASH)
	{
		//storing LHS of the rule
		tempRule.LHS = myToken;

		while (myToken.token_type != HASH)
		{
			
			myToken = lexer.GetToken();
			if (myToken.token_type != ARROW)
			{
				tempRule.RHS.push_back(myToken);
			}

		} 

		grammar.push_back(tempRule);

		//clear the tempRule vector
		tempRule.RHS.clear();

		myToken = lexer.GetToken();
	}
	//beging of Nonterminal vector building
	NonTermVector.push_back(grammar[0].LHS.lexeme);
	for (int grammarIndex = 0; grammarIndex < grammar.size(); grammarIndex++)
	{
		myToken = grammar[grammarIndex].LHS;

		if (!FoundInVector(myToken.lexeme, 0, NonTermVector))
		{
			NonTermVector.push_back(myToken.lexeme);
		}
	}
	
	//beging of Terminal vector building
	for (int grammarIndex = 0; grammarIndex < grammar.size(); grammarIndex++)
	{
		for (int RHSindex = 0; RHSindex < grammar[grammarIndex].RHS.size(); RHSindex++)
		{
			if (grammar[grammarIndex].RHS[RHSindex].token_type != HASH)
			{
				if (!FoundInVector(grammar[grammarIndex].RHS[RHSindex].lexeme, 0, NonTermVector))
				{
					if (TermVector.empty())
					{
						TermVector.push_back(grammar[grammarIndex].RHS[RHSindex].lexeme);
					}
					else if (!FoundInVector(grammar[grammarIndex].RHS[RHSindex].lexeme, 0, TermVector))
					{
						TermVector.push_back(grammar[grammarIndex].RHS[RHSindex].lexeme);
					}
				}
			}
		}
	}	
}

bool FoundInVector(string srchString, int index, vector<string> srchVector)
{
	
	bool result = false;

	if (srchVector[index] == srchString)
	{
		result = true;
	}
	else if (index < srchVector.size()-1)
	{
		result = FoundInVector(srchString, index+1, srchVector);
	}

	return result;
}

int FindIndex(string srchString, int index, FirstType srchgram)
{

	int result = 0;

	if (srchgram[index][0] == srchString)
	{
		result = index;
	}
	else if (index < srchgram.size() - 1)
	{
		result = FindIndex(srchString, index + 1, srchgram);
	}
	return result;
}


// Task 1
void printTerminalsAndNoneTerminals()
{
	class Token myToken;
    vector <string> ResultsVector;
	if (!grammar.empty())
	{
		//treverse LHS and put into Vector
		
		ResultsVector.push_back(NonTermVector[0]);

		for (int grammarIndex = 0; grammarIndex < grammar.size(); grammarIndex++)
		{
			if (!FoundInVector(grammar[grammarIndex].LHS.lexeme, 0, ResultsVector))
			{
				ResultsVector.push_back(grammar[grammarIndex].LHS.lexeme);
			}
			for (int RHSindex = 0; RHSindex < grammar[grammarIndex].RHS.size(); RHSindex++)
			{
				if (!FoundInVector(grammar[grammarIndex].RHS[RHSindex].lexeme, 0, ResultsVector))
				{
					if (FoundInVector(grammar[grammarIndex].RHS[RHSindex].lexeme, 0, NonTermVector))
					{
						ResultsVector.push_back(grammar[grammarIndex].RHS[RHSindex].lexeme);
					}
					
				}
			}
		}
		PrintVector(ResultsVector);
		PrintVector(TermVector);
	}
	else
	{
		//the was no grammar
		cout << "no Grammar";
	}
}

// Task 2
void RemoveUselessSymbols()
{
	vector <string> GenSet;
	vector <string> ReachSet;
	vector <RuleType> GramGen;
	vector <RuleType> GenReach;
	
	bool Change = true;
	bool RHSmem = true;
	bool RuleGener = false;

	GenSet = TermVector;
	GenSet.push_back("#"); //adding the hash
	
	//step 1 finding Genrating 
	while (Change)
	{
		Change = false;

		for (int grammarIndex = 0; grammarIndex < grammar.size(); grammarIndex++)
		{
			if (grammar[grammarIndex].RHS[0].token_type == HASH)
			{
				RHSmem = true;
			}
			else
			{
				RHSmem = true;
				for (int RHSindex = 0; RHSindex < grammar[grammarIndex].RHS.size() - 1; RHSindex++) //minas one to the index to ignore the hash at the end reprsented by a blank
				{
					RHSmem = RHSmem && FoundInVector(grammar[grammarIndex].RHS[RHSindex].lexeme, 0, GenSet);
				}
			}			
			if (RHSmem)
			{	
				if (!FoundInVector(grammar[grammarIndex].LHS.lexeme, 0, GenSet))
				{
					GenSet.push_back(grammar[grammarIndex].LHS.lexeme);
					Change = true;
				}
			}
		}
	}
	
	//step 2 creating usefull grammar 
	for (int grammarIndex = 0; grammarIndex < grammar.size(); grammarIndex++)
	{
		RuleGener = false;
		if (FoundInVector(grammar[grammarIndex].LHS.lexeme, 0, GenSet))
		{
			
			if (grammar[grammarIndex].RHS[0].token_type == HASH)
			{
				RuleGener = true;
			}
			else
			{
				RuleGener = true;
				for (int RHSindex = 0; RHSindex < grammar[grammarIndex].RHS.size() - 1; RHSindex++)
				{
					RuleGener = RuleGener && FoundInVector(grammar[grammarIndex].RHS[RHSindex].lexeme, 0, GenSet);
				}
			}
		}
		if (RuleGener)
		{
			GramGen.push_back(grammar[grammarIndex]);
		}
	}

	//step 3 creating all reachable rules
	ReachSet.push_back(grammar[0].LHS.lexeme);
		Change = true;
	
	while (Change)
	{
		Change = false;
		for (int GramGenIndex = 0; GramGenIndex < GramGen.size(); GramGenIndex++)
		{
			if (FoundInVector(GramGen[GramGenIndex].LHS.lexeme, 0, ReachSet))
			{
				for (int RHSindex = 0; RHSindex < GramGen[GramGenIndex].RHS.size() - 1; RHSindex++)
				{
					if (!FoundInVector(GramGen[GramGenIndex].RHS[RHSindex].lexeme, 0, ReachSet))
					{
						ReachSet.push_back(GramGen[GramGenIndex].RHS[RHSindex].lexeme);
						Change = true;
					}
				}
			}
		}
	}

	for (int GramGenIndex = 0; GramGenIndex < GramGen.size(); GramGenIndex++)
	{
		if (FoundInVector(GramGen[GramGenIndex].LHS.lexeme, 0, ReachSet))
		{
			GenReach.push_back(GramGen[GramGenIndex]);
		}
	}

	PrintGrammar(GenReach);
}

void printFirst(FirstType FirstSet)
{

	vector <string> LocalPrintVector;
	vector <string> TempTermVector;
	const int LHS = 0;
	const int RHS = 1;

	
	for (int PrintIndex1 = 0; PrintIndex1 < FirstSet.size(); PrintIndex1++)
	{
		// clearing temp vectors
		TempTermVector.clear();
		LocalPrintVector.clear();

		if (FirstSet[PrintIndex1][LHS] != "#" &&
			!FoundInVector(FirstSet[PrintIndex1][LHS],0,TermVector))
		{
			cout <<"FIRST("<< FirstSet[PrintIndex1][LHS]<<") = { ";
			
			if (FirstSet[PrintIndex1].size() > 1) // not an empty set
			{
				// building print vector
				for (int index = 1; index < FirstSet[PrintIndex1].size(); index++)
				{
					TempTermVector.push_back(FirstSet[PrintIndex1][index]);
				}
				
				if (FoundInVector("#", 0, TempTermVector))
				{
					LocalPrintVector.push_back("#");
				}

				for (int grammarIndex = 0; grammarIndex < grammar.size(); grammarIndex++)
				{
					for (int RHSindex = 0; RHSindex < grammar[grammarIndex].RHS.size(); RHSindex++)
					{
						if (FoundInVector(grammar[grammarIndex].RHS[RHSindex].lexeme, 0, TempTermVector))
						{
							if (LocalPrintVector.empty())
							{
								LocalPrintVector.push_back(grammar[grammarIndex].RHS[RHSindex].lexeme);
							}
							else if (!FoundInVector(grammar[grammarIndex].RHS[RHSindex].lexeme, 0, LocalPrintVector))
							{
								LocalPrintVector.push_back(grammar[grammarIndex].RHS[RHSindex].lexeme);
							}
						}
					}
				}

				
				if (FoundInVector("#", 0, LocalPrintVector))
				{
					if (LocalPrintVector.size() > 1)
					{
						cout << "#, ";
					}
					else
					{
						cout << "# ";
					}
				}
				for (int PrintIndex2 = 0; PrintIndex2 < LocalPrintVector.size(); PrintIndex2++)
				{
					if (LocalPrintVector[PrintIndex2] != "#")
					{
						cout << LocalPrintVector[PrintIndex2];
						if (PrintIndex2 < LocalPrintVector.size() - 1)
						{
							cout << ", ";
						}
						else
						{
							cout << " ";
						}
					}

				}
			}
			else // empty set
			{
				cout << " ";
			}
			cout << "}\n";
		}

	}
}

// Task 3
void CalculateFirstSets()
{
	vector <string> InOrderNT;
	vector <string> tempVector;
	FirstType FirstGram;
	int addToIndex = 0;
	int FromIndex = 0;
	int gramRHSindex = 0;
	int changedCounter = 0;

	int FirstIndex = 0;
	bool changed = true;
	bool done = false;
	bool hashFound = false;

	//initulizing episilon
	tempVector.push_back("#");
	tempVector.push_back("#");
	FirstGram.push_back(tempVector);
	tempVector.clear();

	//itiulizing terminals
	for (int TermIndex= 0; TermIndex < TermVector.size(); TermIndex++)
	{
		tempVector.push_back(TermVector[TermIndex]);
		tempVector.push_back(TermVector[TermIndex]);
		FirstGram.push_back(tempVector);
		tempVector.clear();
	}

	//Nonterminals in order vector

	InOrderNT.push_back(NonTermVector[0]);

	for (int grammarIndex = 0; grammarIndex < grammar.size(); grammarIndex++)
	{
		if (!FoundInVector(grammar[grammarIndex].LHS.lexeme, 0, InOrderNT))
		{
			InOrderNT.push_back(grammar[grammarIndex].LHS.lexeme);
		}
		for (int RHSindex = 0; RHSindex < grammar[grammarIndex].RHS.size(); RHSindex++)
		{
			if (!FoundInVector(grammar[grammarIndex].RHS[RHSindex].lexeme, 0, InOrderNT))
			{
				if (FoundInVector(grammar[grammarIndex].RHS[RHSindex].lexeme, 0, NonTermVector))
				{
					InOrderNT.push_back(grammar[grammarIndex].RHS[RHSindex].lexeme);
				}

			}
		}
	}

	//itiulizing nonternimals
	for (int nonTermIndex = 0; nonTermIndex < InOrderNT.size(); nonTermIndex++)
	{
		tempVector.push_back(InOrderNT[nonTermIndex]);
		FirstGram.push_back(tempVector);
		tempVector.clear();
	}
	
	changed = true;
	while (changed)
	{

		changed = false;
		for (int grammarIndex = 0; grammarIndex < grammar.size(); grammarIndex++)
		{
			addToIndex = FindIndex(grammar[grammarIndex].LHS.lexeme, 0, FirstGram);

			gramRHSindex = 0;
			FromIndex = FindIndex(grammar[grammarIndex].RHS[gramRHSindex].lexeme, 0, FirstGram);

			if (FirstGram[FromIndex].size() == 1)
			{
				hashFound = false;
			}
			else
			{
				hashFound = true; // Need to get into following while loop
				while (hashFound && gramRHSindex < grammar[grammarIndex].RHS.size())
				{
					hashFound = false;
					FromIndex = FindIndex(grammar[grammarIndex].RHS[gramRHSindex].lexeme, 0, FirstGram);
					for (int FirstIndex = 1; FirstIndex < FirstGram[FromIndex].size(); FirstIndex++)
					{
						if (FirstGram[FromIndex][FirstIndex] == "#")
						{
							hashFound = true;
						}
						
						else if (!FoundInVector(FirstGram[FromIndex][FirstIndex], 0, FirstGram[addToIndex]))
						{
							FirstGram[addToIndex].push_back(FirstGram[FromIndex][FirstIndex]); //Rule 3
							changed = true;
						}
					}

					if (hashFound)
					{
						gramRHSindex = gramRHSindex + 1; // Go to next Grammar RHS -- Rule 4
					}
				}

				if (hashFound) //Rule 5
				{
					if (!FoundInVector("#", 0, FirstGram[addToIndex]))
					{
						FirstGram[addToIndex].push_back("#");
						changed = true;
					}
				}
			}
		}
	}
	printFirst(FirstGram);
	cout << "\n";
}

    
int main (int argc, char* argv[])
{
    int task;

    if (argc < 2)
    {
        cout << "Error: missing argument\n";
        return 1;
    }

    /*
       Note that by convention argv[0] is the name of your executable,
       and the first argument to your program is stored in argv[1]
     */

    task = atoi(argv[1]);

    
    ReadGrammar();  // Reads the input grammar from standard input
                    // and represent it internally in data structures
                    // ad described in project 2 presentation file

    switch (task) {
        case 1: printTerminalsAndNoneTerminals();
            break;

        case 2: RemoveUselessSymbols();
		
            break;

        case 3: CalculateFirstSets();
		
            break;

        default:
            cout << "Error: unrecognized task number " << task << "\n";
            break;
    }
    return 0;
}

