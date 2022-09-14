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

    const char* start1 = str1.c_str();
    const char* start2 = str2.c_str();

    int len1 = str1.length();
    int len2 = str2.length();

    int flocate1 = -1;
    int flocate2 = -1;

    char *array1 = new char[len1];
    char *array2 = new char[len2];

    int i = 0;
    int l = 0;

    for (i = 0, l = 0; l < len1; i++, l++)
    {
        if (start1[l] == '.')
        {
            flocate1++;
            i--;
            continue;
        }
        if (flocate1 != -1)
        {
            flocate1++;
        }
        array1[i] = start1[l];
    }

    for (i = 0, l = 0; l < len2; i++, l++)
    {
        if (start2[l] == '.')
        {
            flocate2++;
            i--;
            continue;
        }
        if (flocate2 != -1)
        {
            flocate2++;
        }
        array2[i] = start2[l];
    }

    int flocate = flocate1 + flocate2;
    if (flocate == -2)
    {
        flocate = 0;
    }

    // char *res = (char *)malloc(sizeof(char)*(sizeof(array1) + sizeof(array2)));
    // memset(res, 0, sizeof(char)*(sizeof(array1) + sizeof(array2)));
    char res[strlen(array1) + strlen(array2)] = {0};    

    for (i = strlen(array1) - 1; i >= 0; i--)
    {
        for (l  = strlen(array2) - 1; l >= 0; l--)
        {
            // -'0' trans char to number in real by minus/add '0's ascii so we can caculate directly
            res[i + l + 1] += (array1[i] - '0') * (array2[l] - '0');
            res[i + l] +=  res[i + l + 1] / 10;
            res[i + l + 1] %= 10;
        }
    }

    int countZero = 0;
    while (res[countZero] == 0)
    {  
        countZero++;
    }
    // char *ret = (char *)malloc(sizeof(char)* (sizeof(array1) + sizeof(array2) + 2));
    // memset(res, 0, sizeof(char)* (sizeof(array1) + sizeof(array2) + 2));

    int sizeRet = 1;
    if (flocate == 0)
    {
        sizeRet = sizeof(res) - countZero;
    }    
    else
    {
        sizeRet = sizeof(res) - countZero + 1;
    }

    char ret[sizeRet + 1] = {0}; 

    for (i = countZero, l = 0; i < sizeof(res); i++, l++)
    {
        if (l == sizeof(res) - countZero - flocate && flocate != 0)
        {
            ret[l] = '.';
            i--;
            continue;
        }
        ret[l] = res[i] + '0';
    }

    cout << ret << endl;

    // for (i = 0; i < sizeof(ret); i++)
    // {
    //     if (ret[i] == '.')
    //     {
    //         cout << '.';
    //         continue;
    //     }
    //     // cout << i << ":" << (int)ret[i] << endl;
    //     cout << (int)ret[i];
    // }

    return 0;
}
