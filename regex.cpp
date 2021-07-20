#include "regex.h"
#include <iostream>
#include <vector>

 
/* Initialize the regex operator to apply exactly once. */
RegexOperator::RegexOperator() {
    minRepeat = 1;
    maxRepeat = 1;
}


/* Returns the "minimum repeat count" value. */
int RegexOperator::getMinRepeat() const {
    return minRepeat;
}


/* Returns the "maximum repeat count" value. */
int RegexOperator::getMaxRepeat() const {
    return maxRepeat;
}


/* Sets the "minimum repeat count" value. */
void RegexOperator::setMinRepeat(int n) {
    assert(n >= 0);
    minRepeat = n;
}


/* Sets the "maximum repeat count" value. */
void RegexOperator::setMaxRepeat(int n) {
    assert(n >= -1);
    maxRepeat = n;
}


/* Clears the list of matches stored in the regex operator.  Typically done
 * in preparation to try to match the regex to a new string.
 */
void RegexOperator::clearMatches() {
    matches.clear();
}


/* Records a new match of the operator in the list of matches. */
void RegexOperator::pushMatch(const Range &r) {
    matches.push_back(r);
}


/* Reports how many times the regex operator has successfully matched in the
 * string.
 */
int RegexOperator::numMatches() const {
    return (int) matches.size();
}


/* Removes the last match the operator successfully matched against.  Used for
 * backtracking by the regex engine.
 */
Range RegexOperator::popMatch() {
    Range r = matches.back();
    matches.pop_back();
    return r;
}

MatchChar::MatchChar(char ch) {
    c = ch;
}

bool MatchChar::match(const string &s, Range &r) const
{
    if(r.start >= (int) s.length() || s[r.start]!=c) return false;
    r.end = r.start + 1;
    return true;
}

MatchAny :: MatchAny(bool Dot, bool Digit, bool AlNumeric, bool Space) {
    dot = Dot;
    digit = Digit;
    alphaNumeric = AlNumeric;
    space = Space;
}

bool MatchAny :: match(const string &s, Range &r) const
{
    if(r.start >= (int) s.length()) return false;

    bool found=1;
    if(dot) found = 1;
    else if(digit)
    {
        if(s[r.start] >= '0' && s[r.start]<='9')
            found = 1;
        else 
            found = 0;
    }
    else if(alphaNumeric)
    {
        if( (s[r.start] >= 'a' && s[r.start]<='z') || 
            (s[r.start] >= 'A' && s[r.start]<='Z') ||
            (s[r.start] >= '0' && s[r.start]<='9') ||
            s[r.start] == '_' ) 
            {
                found = 1;
            }
        else    
            found = 0;
    }
    else if(space)
    {
        if(s[r.start] == ' ')
            found = 1;
        else 
            found = 0;
    }
    
    if(!found) return false;

    r.end = r.start+1;
    return true;
}

char MatchAny :: identify()
{
    if(dot) return '.';
    if(digit) return '7';
    if(alphaNumeric) return 'A';
    return 'S';
}

MatchFromSubset :: MatchFromSubset(string s)
{
    str = s;
    len = str.length();
}

bool MatchFromSubset :: match(const string &s, Range &r) const
{
    if(r.start >= (int)s.length()) return false;
    bool found=0;
    
    for(int i=0; i<len; i++)
    {
        if(i+2 < len and str[i+1]=='-')
        {
            int startChar = str[i], endChar = str[i+2];
            if(s[r.start]>=startChar and s[r.start]<=endChar)
            {
                found = 1;
                break;
            }
            i = i+2;
            continue;
        }
        else if(str[i] == s[r.start])
        {
            found = 1;
            break;
        }
    }  
    
    if(found) r.end = r.start + 1;
    return found;
}

ExcludeFromSubset :: ExcludeFromSubset(string s)
{
    str = s;
    len = str.length();
}

char MatchFromSubset :: identify(){
    std::cout<<str;
    return ' ';
}

char ExcludeFromSubset :: identify(){
    std::cout<<"^ "<<str;
    return ' ';
}

bool ExcludeFromSubset :: match(const string &s, Range &r) const
{
    if(r.start >= (int) s.length()) return false;
    bool found=0; // stores if s[r.start] is present in str or not

    for(int i=0; i<len; i++)
    {
        if(s[r.start] == str[i])
        {
            found = 1;
            break;
        }
    }

    found = !found; // if not present then return true;

    if(found) r.end = r.start +1;
    return found;
}

vector<RegexOperator *> parseRegex(const string &expr)
{
    vector<RegexOperator *> parsed;

    size_t len = expr.length();
    for(size_t i=0; i<len; i++)
    {
        RegexOperator* op; 
        size_t endIndex = i+1; // the end of curr-Substring in expr
        if(expr[i] == '[')
        {
            size_t startIndex = i;
            endIndex = expr.find(']', startIndex);
            
            assert(endIndex != string::npos);
            
            if(i+1 < len && expr[i+1]=='^')
            {
                startIndex = i+2;
                endIndex--;
                string subset = expr.substr( startIndex, endIndex - startIndex + 1);
                op = new ExcludeFromSubset(subset);
            }
            else
            {
                startIndex = i+1;
                endIndex--;
                string subset = expr.substr(startIndex, endIndex - startIndex + 1);
                op = new MatchFromSubset(subset);
            }
            endIndex += 2;
            
            parsed.push_back(op);
        }
        else if(expr[i] == '\\')
        {
            if(i+1 < len)
            {
                if(expr[i+1]=='.' || expr[i+1]=='\\')
                {
                    endIndex = i+2;
                    op = new MatchChar(expr[i+1]);
                    parsed.push_back(op);
                }
                else if(expr[i+1]=='d') // "\d -> Matches nums between 0-9"
                {
                    endIndex = i+2; 
                    op = new MatchAny(0, 1, 0, 0);
                    parsed.push_back(op);
                }
                else if(expr[i+1]=='w') // "\w -> matches alphaNumeric and _"
                {
                    endIndex = i+2; 
                    op = new MatchAny(0, 0, 1, 0);
                    parsed.push_back(op);
                }
                else if(expr[i+1]=='s') // "\s -> matches a white space"
                {
                    endIndex = i+2; 
                endIndex = i+2;
                    endIndex = i+2; 
                    op = new MatchAny(0, 0, 0, 1);
                    parsed.push_back(op);
                }
            }
        }
        else if(expr[i] == '.') // "." -> matches any character.
        {
            op = new MatchAny(1, 0, 0, 0);
            parsed.push_back(op);
        }
        else
        {
            op = new MatchChar(expr[i]);
            parsed.push_back(op);
        }

        if(endIndex < len)
        {
            if(expr[endIndex] == '*')
            {
                op->setMinRepeat(0);
                op->setMaxRepeat(-1);
                i = endIndex;
            }
            else if(expr[endIndex] == '+')
            {
                op->setMinRepeat(1);
                op->setMaxRepeat(-1);
                i = endIndex;
            }
            else if(expr[endIndex] == '?')
            {
                op->setMinRepeat(0);
                op->setMaxRepeat(1);
                i = endIndex;
            }
            else if(expr[endIndex] == '{') // endIndex points to start of operator
            {
                int minRep=-1, maxRep=-1;

                if(endIndex+1 < len) minRep = expr[endIndex+1] - '0';
                if(endIndex + 4 < len and expr[endIndex+2]==',' and expr[endIndex+4]=='}')
                    maxRep = expr[endIndex+3]-'0';

                if(minRep!=-1 and maxRep!=-1) // {2,3} type operators
                {
                    op->setMinRepeat(minRep);
                    op->setMaxRepeat(maxRep);
                    i = endIndex+4;
                }
                else if(endIndex+1 < len and expr[endIndex+2]=='}' and minRep != -1 and maxRep == -1)
                {
                    // {2} type operators
                    op->setMinRepeat(minRep);
                    op->setMaxRepeat(minRep);
                    i = endIndex+2;
                }
            }
        }
        // if no operator found after curr unit, either endIndex = len or endIndex is not operator
        // ab*c : endIndex = 2 for i==1, after calc i=2;
        // ab{3,4}c : endIndex = 2 for i==1, after calculations i=6;
        // so if not found then i<endIndex.
        if(i < endIndex) i = endIndex - 1;
    }

    return parsed;
}

void clearRegex(vector<RegexOperator *> regex)
{
    int n = regex.size();
    for(int i=0; i<n ;i++)
    {
        delete(regex[i]);
    }
    regex.clear();
}

// int main()
// {
//     string regex = "\\d{2,3}";
//     vector<RegexOperator *> parsedOperators;
//     parsedOperators = parseRegex(regex);

//     for(auto i : parsedOperators)
//     {
//         cout<<i->identify();
//         cout<<" : "<<i->getMinRepeat()<<" -> "<<i->getMaxRepeat()<<endl;
//     }
//     return 0;
// }