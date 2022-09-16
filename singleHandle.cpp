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
#include <tuple>

using namespace std;

int main()
{
    tuple<bool, char *, int> result;

    string str = "";

    cin >> str;

    const char* start = str.c_str();

    int len = str.length();

    char *array = new char[len];

    int i = 0;
    int l = 0;

    int countE = 0;    // count E appear times, so below
    int countP = 0;
    int countN = 0;
    int indexE = -1;

    for (i = 0, l = 0; l < len; i++, l++)
    {
        if ((start[l] < 45 || start[l] == 47)
            || (start[l] > 57 && start[l] != 101))
        {
            cout << "Invalid input!" << endl;
            return 0;
        }
         switch (start[l])
        {
        case 'e'  /* constant-expression */:
            /* code */
            countE++;
            indexE = l;
            continue;

        case '.':
            countP++;
            i--;
            continue;

        case '-':
            countN++;
            continue;

        default:
            array[i] = start[l];
        }
        if (countE > 1 || countN > 2 || countP > 1)
        {
            cout << "Invalid input!" << endl;
            return 0;
        }
    }

    bool isNegative = false;
    if (array[0] == '-')
    {
        isNegative = true;
    }

    result = make_tuple(isNegative, array, indexE);

}
