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
#include <cmath>
#include <cstring>
#include <iostream>
#include <tuple>

using namespace std;

/// @brief Convert string to char[] with in-progress validity check
/// @param str: string
/// @return array: char[]
char *str2charl(string str) {
  const char *input = str.c_str();
  int len = str.length();
  char *array = new char[len];

  for (int i = 0; i < len; i++) {
    if (array[i] != 45 && array[i] < 48 && array[i] > 57) {
      cout << "Invalid input!" << endl;
      exit(0);
    }
    array[i] = input[i];
  }
  return array;
}

/// @brief Input early check + preprocessing
/// @param str: string, main() argv[1] or argv[2]
/// @return tuple<char *arrayHead, int arrayTail, int pLocation, bool
/// isNegative>
///   @retval arrayHead: the decimal part
///   @retval arrayTail: ths power part
///   @retval pLocation: decimal point position
///   @retval isNegative: the parity
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
    case 2: {
      // use pLocation to record '.'s position
      int rpLocation = str.find('.');
      str.erase(rpLocation, 1);
      pLocation = str.length() - rpLocation;
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
        if (rpLocation != string::npos) {
          strHead.erase(rpLocation, 1);
        }
        pLocation = strHead.length() - rpLocation;
        arrayHead = str2charl(strHead);
        str.erase(eLocation, 1);
        string strTail = str.substr(eLocation, str.size());
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
/// @param array1: char[], input in char[] form
/// @param array2: char[], input in char[] form
/// @return ret: string, could it start with '.'? no
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

  // hint: use string ret(retP) directly could possiblely include the '\n', and
  // will ruin the decimal point position calculation.
  string ret = "";
  for (int i = 0; i < sizeof(retP); i++) {
      // use push_back() to avoid the hint
      ret.push_back(retP[i]);
  }
  while(ret[0] == '0'){
    ret.erase(0,1);
  }

  return ret;
}

/// @brief Schönhage–Strassen algorithm, inspired by the article
/// https://blog.csdn.net/u010983881/article/details/77503519
/// @param str1
/// @param str2
/// @return string
string karatsuba(string str1, string str2) {
  // 计算拆分长度
  int size1 = str1.length();
  int size2 = str2.length();
  if (size1 == 0 || size2 == 0) {
    return to_string(0);
  }
  if (size1 < 3 || size2 < 3) {
    int num1 = stoi(str1);
    int num2 = stoi(str2);
    //递归终止条件
    return to_string(num1 * num2);
  }

  int halfN = max(size1, size2) / 2;

  /* 拆分为a, b, c, d */
  string a = str1.substr(0, size1 - halfN);
  string b = str1.substr(size1 - halfN, size1);
  string c = str2.substr(0, size2 - halfN);
  string d = str2.substr(size2 - halfN, size2);

  // 计算z2, z0, z1, 此处的乘法使用递归
  long z2 = stol(karatsuba(a, c));
  long z0 = stol(karatsuba(b, d));
  long z1 = stol(karatsuba((a + b), (c + d))) - z0 - z2;

  return to_string(z2 * pow(10, (2 * halfN)) + z1 * pow(10, halfN) + z0);
}

/// @brief A simple multiplier with full precision
/// @param argc: int, should be 3
/// @param argv: char[], 2 input parms
/// @return state: state code 0 with output result to terminal
int main(int argc, char *argv[]) {
  // string str1 = argv[1];
  // string str2 = argv[2];
  // input with ostream:
  cout << "input" << endl;
  string str1 = "";
  string str2 = "";
  cin >> str1 >> str2;
  tuple<char *, int, int, bool> input1;
  tuple<char *, int, int, bool> input2;
  input1 = inputHandle(str1);
  input2 = inputHandle(str2);

  // calculate result - or +
  bool isNegative = get<3>(input1) ^ get<3>(input2);
  // calculate result value or its decimal part
  string resHead = hugeMul(get<0>(input1), get<0>(input2));

  // // calculate in different way
  // string array1 = "";
  // char *out1 = get<0>(input1);
  // for (int i = 0; i < sizeof(out1); i++) {
  //   if (out1[i]) {
  //     array1.push_back(out1[i]);
  //   }
  // }
  // cout << "array1: " << array1 << endl;
  // string array2 = "";
  // char *out2 = get<0>(input2);
  // for (int i = 0; i < sizeof(out2); i++) {
  //   if (out2[i]) {
  //     array2.push_back(out2[i]);
  //   }
  // }
  // cout << "array2: " << array2 << endl;
  // string resHead = karatsuba(array1, array2);
  // cout << "resHead: " << resHead << endl;

  // calculate result power part
  int resTail = get<1>(input1) + get<1>(input2);
  // calculate decimal point position
  int pLocation = get<2>(input1) + get<2>(input2);
  // combine decimal part with decimal point
  while (resHead.length() <= pLocation) {
    resHead.insert(0, "0");
  }
  if (pLocation > 0) {
    resHead.insert(resHead.length() - pLocation, ".");
  }
  // format output
  cout << str1 << " * " << str2 << " = ";
  if (isNegative) {
    cout << '-';
  }
  cout << resHead;
  if (resTail) {
    cout << 'E' << resTail;
  }
  cout << endl;
  /// @todo: combine the result into one object to store/transmit/reuse
  return 0;
}
