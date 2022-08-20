// anglosaxon.cpp
#include <iostream>
#include <string>
#include <algorithm>
#include "vector.h"
#include "map.h"
#include "glob.h"
using namespace std;

const string INPUT_DIR   = "angloSaxon_Corpus";
const string POETRY_DIR  = "Poetry";
const string PROSE_DIR   = "Prose";

/* Get a list of filenames starting with a given pattern.

     Parameters:
       pattern:  a string containing the pattern of the desired filenames
       fileList: a Vector containing the filenames with the given pattern
       
     Return value: none
*/

void getFiles(string pattern, Vector<string>& fileList)
{
    glob_t globBuffer;
    glob(pattern.c_str(), GLOB_TILDE, NULL, &globBuffer);

    for(int i = 0; i < globBuffer.gl_pathc; i++)
    {
        fileList.add(globBuffer.gl_pathv[i]);
    }

    if (globBuffer.gl_pathc > 0)
    {
        globfree(&globBuffer);
    } 
}

// Check whether a word in a vector or not.
bool searchElement(string a, Vector<string> b)
{
    for (int i = 0; i < b.size(); i++)
    {
        if (a == b[i])
            return true;
    }
    return false;
}



// Find the key with the highest value in a map of integers.
string findKeyMax(int identified, Map<string,int> b)
{
    for (string key: b)
    {
        if (b[key] == identified)
          return key;
    }
    return "";
}

// Check the word and lower all of the characters and remove some specific punctuations.
string lowerstrip(string s){
    string a;
    char ch;
    for (int i = 0; i < s.length(); i++)
      {
        ch = s[i];
        if (ch == ';' || ch == '&' || ch == '#')
          a = a + ch;
        else if (isalpha(ch))
          if (!islower(ch))
            a = a + (char) tolower(ch);
          else
            a = a + ch;
      }
    return a;
}

// Use vector objects to keep track of words and print out the ten words with highest frequency
void onlyInPoetryVector(Vector<string> poetryFiles, Vector<string> proseFiles)
{
    // create two vectors: keyList to contain key, and valList to contain the frequency of key 
    Vector <string> keyList;
    Vector <int> valList;
    // prose and poetry keep all words from prose files and poetry files
    Vector <string> poetry;
    Vector <string> prose;
    // diff vector stores all of words in poetry but not in prose, and it is not unique.
    Vector <string> diff;
    // the vectors names and ranking stores the string and its corresponding occurrence according to repititoin.
    Vector <string> names;
    Vector <int> ranking;
    string word;
  

    for (string filename: poetryFiles)
    {
        ifstream is1(filename);
        while (!is1.eof())
        {
            is1 >> word;
            poetry.add(lowerstrip(word));
        }
    }
    for (string file: proseFiles)
    {
        ifstream is2(file);
        while (!is2.eof())
        {
            is2 >> word;
            prose.add(lowerstrip(word));
        }
    }
    
    for (int i = 0; i < poetry.size(); i++)
    {
        if (!searchElement(poetry[i],prose))
            diff.add(poetry[i]);
    }


    for (int i= 0; i<10; i++)
    {
        ranking.add(10);
        names.add("  ");
    }

    int Length = diff.size()-1;
    for (int x= 0; x<Length+1; x++)
    {
        int c = 1;

        for (int y =x+1; y<Length+1; y++)
        {
            if(diff[x] == diff[y])
            {
                diff.remove(y);
                y = y-1;
                Length -=1;
                c = c+ 1;

            }
        }
        if (c<ranking[0])continue;
    for (int i =0; i< 10; i++)
    {
        if (ranking[9] < c) 
        {
            ranking.insert(10,c);
            names.insert(10,diff[x]);
            ranking.remove(0);
            names.remove(0);
            break;
        }
        else if (ranking[i] >= c)
        {
            ranking.insert(i, c);
            names.insert(i,diff[x]);
            ranking.remove(0);
            names.remove(0);
            break;


        }
        
    }

    }

    for (int i = 0; i< 10; i++)
    {
        cout << names[9-i] << ": " << ranking[9-i] << " times" << endl; 
    }
    
 }

// Use map object to keep track all of words and also their frequencies in text files
void onlyInPoetryMap(Vector<string> poetryFiles, Vector<string> proseFiles)
{
    /* create two maps, poetry1 contains the words in poetry files, 
    prose1 contains the words in prose files. */
    Map<string,int> poetry1;
    Map<string,int> prose1;
    Map<string,int> res;
    string word;
    string ch;
    int count = 0;
    int max = 0;

    // check every words in poetry and prose files and count them.
    for (string filename: poetryFiles)
    {
        ifstream is1(filename);
        while (!is1.eof())
        {
            is1 >> word;
            ch = lowerstrip(word);
            if (poetry1.containsKey(ch))
              poetry1[ch] += 1;
            else
              poetry1[ch] = 1;
        }
    }
    for (string file: proseFiles)
    {
        ifstream is2(file);
        while (!is2.eof())
        {
            is2 >> word;
            ch = lowerstrip(word);
            if (prose1.containsKey(ch))
                prose1[ch] += 1;
            else
                prose1[ch] = 1;
        }
    }
        
    // add keys that appear in poetry1 but not in prose1 to 'res' vector.
    for (string key: poetry1)
    {
        if (!prose1.containsKey(key))
          res[key] = poetry1[key];
    }
    while (count < 10)
    {
        int max = 0;
        string idn;
        // find the maximum frequencies of words in 'res' vector.
        for (string key: res)
        {
            if (res[key] > max)
              max = res[key];
        }
        idn = findKeyMax(max,res);
        // print out the top 10 words with highest frequency in res.
        cout << idn << ": " << res[idn] << " times" << endl;
        res.remove(idn);
        count = count + 1;
    }

}

int main(int argc, char* argv[])
{
    if ((argc != 2) || (strcmp(argv[1], "-v") && strcmp(argv[1], "-m")))
    {
        cout << "Usage: " << endl;
        cout << "  " << argv[0] << " -v  (for vector-based solution)" << endl;
        cout << "  " << argv[0] << " -m  (for map-based solution)" << endl;
        return 1;
    }

    // Get vectors containing all of the poetry and prose filenames
    Vector<string> poetryFiles, proseFiles;
    getFiles(INPUT_DIR + "/" + POETRY_DIR + "/*", poetryFiles);
    getFiles(INPUT_DIR + "/" + PROSE_DIR + "/*", proseFiles);

    string whichAlg(argv[1]);
    if (whichAlg == "-v")
        onlyInPoetryVector(poetryFiles, proseFiles);
    else
        onlyInPoetryMap(poetryFiles, proseFiles);

    return 0;
}
