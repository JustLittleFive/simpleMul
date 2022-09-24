/*
 Copyright 2022 Google LLC

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include <algorithm>
#include <tuple>
#include <components.hpp>

using namespace std;

int main(int argc, char *argv[])
{
    string str1 = argv[1];
    string str2 = argv[2];
    tuple<char *, int, int, bool> input1;
    tuple<char *, int, int, bool> input2;
    input1 = inputHandle(str1);
    input2 = inputHandle(str2);

    
}

/// @brief Input check + handle
/// @param str: main() argv[1] or argv[2]
/// @return tuple<char *arrayHead, int arrayTail, int pLocation, bool isNegative>
tuple<char *, int, int, bool> inputHandle(string str)
{
    bool isNegative = 0;
    if (str[0] == '-')
    {
        isNegative = 1;
        str.erase(0, 1);
    }

    int countE = count(str.begin(), str.end(), 'E');
    int counte = count(str.begin(), str.end(), 'e');
    int countP = count(str.begin(), str.end(), '.');
    int countN = count(str.begin(), str.end(), '-');

    int numType = 0;

    if ((countE + counte + countN + countP) == 0)
    {
        // input maybe int/long
        numType = 1;
    }
    else if (countP == 1 && (countE + counte + countN) == 0)
    {
        // input maybe float/double not in Scientific notation
        numType = 2;
    }
    else if ((countE + counte) == 1 && countP < 2 && countN < 2)
    {
        // input maybe float/double in Scientific notation
        numType = 3;
    }
    else
    {
        cout << "Invalid input!" << endl;
        exit(0);
    }

    char *arrayHead = new char(0);
    int arrayTail = 0;
    int pLocation = -1;

    switch (numType)
    {
    case 1:
        // array could be pure-num char list
        arrayHead = str2charl(str);
        break;
    case 2:
        // use pLocation to record '.'s position
        pLocation = str.find('.');
        str.erase(pLocation, 1);
        arrayHead = str2charl(str);
        break;
    case 3:
        pLocation = str.find('.');
        int eLocation = str.find('e');
        if (!eLocation)
        {
            eLocation = str.find('E');
        }
        int nLocation = str.find('-');
        // dual validity check
        if (pLocation < eLocation && (nLocation == string::npos || (eLocation + 1) == nLocation))
        {
            if (nLocation)
            {
                str.erase(nLocation, 1);
            }
            if (pLocation)
            {
                str.erase(pLocation, 1);
                eLocation -= 1;
            }
            // TODO(JustLittleFive): Should split string first
            // DONE
            string strHead = str.substr(0, eLocation);
            str.erase(eLocation, 1);
            string strTail = str.substr(eLocation, str.size());
            arrayHead = str2charl(strHead);
            arrayTail = stoi(str2charl(strTail));
            break;
        }
        else
        {
            cout << "Invalid input!" << endl;
            exit(0);
        }
        break;
    default:
        cout << "Invalid input!" << endl;
        exit(0);
    }

    // input check & convert: DONE
    // OUTPUT: arryHead, arrayTail, plocation
    tuple<char *, int, int, bool>ret = make_tuple(arrayHead, arrayTail, pLocation, isNegative);
    return ret;
}

char *str2charl(string str)
{
    const char *start = str.c_str();
    int len = str.length();

    char *array = new char[len];

    int i = 0;
    int l = 0;

    int indexE = -1;
    int flocate = -1;

    for (i = 0, l = 0; l < len; i++, l++)
    {
        if ((start[l] < 45 || start[l] == 47) || (start[l] > 57 && start[l] != 101))
        {
            cout << "Invalid input!" << endl;
            return 0;
        }
        array[i] = start[l];
        if (flocate > -1)
        {
            flocate++;
        }
    }
    return array;
}
