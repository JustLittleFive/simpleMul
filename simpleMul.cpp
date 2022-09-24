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
#include <cstring>
#include <iostream>
#include <tuple>

using namespace std;

/// @brief  Convert string to char[]
/// @param str: string
/// @return char *: the start of char[]
char *str2charl(string str) {
  const char *input = str.c_str();
  int len = str.length();
  char *array = new char[len];

  int i = 0;

  for (i = 0; i < len; i++) {
    if (array[i] != 45 && array[i] < 48 && array[i] > 57) {
      cout << "Invalid input!" << endl;
      exit(0);
    }
    array[i] = input[i];
  }
  return array;
}

/// @brief Input check + handle
/// @param str: main() argv[1] or argv[2]
/// @return tuple<char *arrayHead, int arrayTail, int pLocation, bool
/// isNegative>
tuple<char *, int, int, bool> inputHandle(string str) {
  bool isNegative = 0;
  if (str[0] == '-') {
    isNegative = 1;
    str.erase(0, 1);
  }

  int countE = count(str.begin(), str.end(), 'E');
  int counte = count(str.begin(), str.end(), 'e');
  int countP = count(str.begin(), str.end(), '.');
  int countN = count(str.begin(), str.end(), '-');

  int numType = 0;

  if ((countE + counte + countN + countP) == 0) {
    // input maybe int/long
    numType = 1;
  } else if (countP == 1 && (countE + counte + countN) == 0) {
    // input maybe float/double not in Scientific notation
    numType = 2;
  } else if ((countE + counte) == 1 && countP < 2 && countN < 2) {
    // input maybe float/double in Scientific notation
    numType = 3;
  } else {
    cout << "Invalid input!" << endl;
    exit(0);
  }

  char *arrayHead = new char(0);
  int arrayTail = 0;
  int pLocation = 0;

  switch (numType) {
    case 1: {
      // array could be pure-num char list
      arrayHead = str2charl(str);
      break;
    }
    case 2: {  // use pLocation to record '.'s position
      pLocation = str.find('.');
      str.erase(pLocation, 1);
      pLocation = str.length() - pLocation;
      arrayHead = str2charl(str);
      break;
    }
    case 3: {
      int rpLocation = str.find('.');
      int eLocation = str.find('e');
      if (eLocation == string::npos) {
        eLocation = str.find('E');
      }
      int nLocation = str.find('-');
      // dual validity check
      if (rpLocation < eLocation &&
          (nLocation == string::npos || (eLocation + 1) == nLocation)) {
        // TODO(JustLittleFive): Should split string first
        // DONE
        string strHead = str.substr(0, eLocation);
        if (rpLocation) {
          strHead.erase(rpLocation, 1);
        }
        pLocation = strHead.length() - rpLocation;
        str.erase(eLocation, 1);
        string strTail = str.substr(eLocation, str.size());
        arrayHead = str2charl(strHead);
        arrayTail = stoi(str2charl(strTail));
        break;
      } else {
        cout << "Invalid input!" << endl;
        exit(0);
      }
      break;
    }
    default: {
      cout << "Invalid input!" << endl;
      exit(0);
    }
  }

  // input check & convert: DONE
  // OUTPUT: arryHead, arrayTail, plocation, isNegative
  tuple<char *, int, int, bool> ret =
      make_tuple(arrayHead, arrayTail, pLocation, isNegative);
  return ret;
}

/// @brief mul two input bit by bit to aviod data loss
/// @param array1: input in char[] form
/// @param array2: input in char[] form
/// @return ret: string form, could it start with '.'?
string hugeMul(char *array1, char *array2) {
  char res[strlen(array1) + strlen(array2)] = {0};

  for (int i = strlen(array1) - 1; i >= 0; i--) {
    for (int l = strlen(array2) - 1; l >= 0; l--) {
      // -'0' converts char to number in real
      // by minus/add '0's ascii so we can caculate directly
      res[i + l + 1] += (array1[i] - '0') * (array2[l] - '0');
      res[i + l] += res[i + l + 1] / 10;
      res[i + l + 1] %= 10;
    }
  }

  char retP[sizeof(res)] = {0};
  for (int i = 0; i < sizeof(res); i++) {
    retP[i] = res[i] + '0';
  }

  string ret(retP);
  while (ret[0] == '0') {
    ret.erase(0, 1);
  }

  return ret;
}

int main(int argc, char *argv[]) {
  string str1 = argv[1];
  string str2 = argv[2];
  // cout << "input" << endl;
  // string str1 = "";
  // string str2 = "";
  // cin >> str1 >> str2;
  tuple<char *, int, int, bool> input1;
  tuple<char *, int, int, bool> input2;
  input1 = inputHandle(str1);
  // cout << "input1 check" << endl;
  input2 = inputHandle(str2);
  // cout << "input2 check" << endl;
  // cout << "str1: " << get<0>(input1) << endl;
  // cout << "str2: " << get<0>(input2) << endl;

  // calculate result - or +
  bool isNegative = get<3>(input1) ^ get<3>(input2);
  // char negative = (get<3>(input1) * get<3>(input2)) ? '-' : '';
  // cout << "hugeMul start" << endl;
  string resHead = hugeMul(get<0>(input1), get<0>(input2));
  // cout << "hugeMul end" << endl;
  int resTail = get<1>(input1) + get<1>(input2);
  int pLocation = get<2>(input1) + get<2>(input2);

  while (resHead.length() <= pLocation) {
    resHead.insert(0, "0");
  }
  resHead.insert(resHead.length() - pLocation, ".");

  cout << str1 << " * " << str2 << " = ";
  if (isNegative) {
    cout << '-';
  }
  cout << resHead;
  if (resTail) {
    cout << 'E' << resTail;
  }
  cout << endl;
  return 0;
}
