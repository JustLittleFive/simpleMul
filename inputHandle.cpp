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

#include <iostream>
#include <cstring>

using namespace std;

int main()
{
    string str1 = "";
    string str2 = "";

    cin >> str1 >> str2;

    cout << "input: " << str1 << " " << str2 << endl;

    const char* start1 = str1.c_str();
    const char* start2 = str2.c_str();

    int len1 = str1.length();
    int len2 = str2.length();

    char *array1 = new char[len1];
    char *array2 = new char[len2];

    int i = 0;
    int l = 0;

    int countE = 0;    // count E appear times, so below
    int countP = 0;
    int countN = 0;
    int indexE1 = -1;

    for (i = 0, l = 0; l < len1; i++, l++)
    {
        if ((start1[l] < 45 || start1[l] == 47)
            || (start1[l] > 57 && start1[l] != 101))
        {
            cout << "Invalid input!" << endl;
            return 0;
        }
         switch (start1[l])
        {
        case 'e'  /* constant-expression */:
            /* code */
            countE++;
            indexE1 = l;
            continue;

        case '.':
            countP++;
            i--;
            continue;

        case '-':
            countN++;
            continue;

        default:
            array1[i] = start1[l];
        }
        if (countE > 1 || countN > 1 || countP > 1)
        {
            cout << "Invalid input!" << endl;
            return 0;
        }
    }

    countE = 0;    // reset count E appear times, so below
    countP = 0;
    countN = 0;
    int indexE2 = -1;

    for (i = 0, l = 0; l < len2; i++, l++)
    {
        if ((start2[l] < 48 && start2[l] != 45 && start2[l] != 46)
            || (start2[l] > 57 && start2[l] != 101))
        {
            cout << "Invalid input!" << endl;
            return 0;
        }
        switch (start2[l])
        {
        case 'e'  /* constant-expression */:
            /* code */
            countE++;
            indexE2 = l;
            continue;

        case '.':
            countP++;
            i--;
            continue;

        case '-':
            countN++;
            continue;

        default:
            array2[i] = start2[l];
        }
        if (countE > 1 || countN > 2 || countP > 1)
        {
            cout << "Invalid input!" << endl;
            return 0;
        }
    }

    cout << "result: " << endl;
    cout << "array1: " << array1 << endl;
    cout << "array2: " << array2 << endl;

    cin.get();
    cin.get();
    cin.get();

    return 0;
}
