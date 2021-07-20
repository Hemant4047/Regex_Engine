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

MatchAny :: MatchAny() {}

bool MatchAny :: match(const string &s, Range &r) const
{
    if(r.start >= (int) s.length()) return false;
    r.end = r.start+1;
    return true;
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
        if(str[i] == s[r.start])
        {
            found = 1;
            break;
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
        size_t endIndex = i+1;
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
            if(i+1 < len && (expr[i+1]=='.' || expr[i+1]=='\\'))
            {
                endIndex = i+2;
                op = new MatchChar(expr[i+1]);
                parsed.push_back(op);
            }
        }
        else if(expr[i] == '.')
        {
            op = new MatchAny();
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
        }
        if(i!=endIndex) i = endIndex - 1;
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
//     string regex = "ab+c?d*[ef]+g[^ghi]*j.+k";
//     vector<RegexOperator *> parsedOperators;
//     parsedOperators = parseRegex(regex);

//     for(auto i : parsedOperators)
//     {
//         cout<<i->identify();
//         cout<<" : "<<i->getMinRepeat()<<" -> "<<i->getMaxRepeat()<<endl;
//     }
//     return 0;
// }