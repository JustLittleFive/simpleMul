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
    if (str[i] < 48 || str[i] > 57) {
      if (str[i] != 45) {
        cout << "Invalid input!" << endl;
        exit(0);
      }
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
        string strTail = str.substr(eLocation, str.length());
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
  while (ret[0] == '0') {
    ret.erase(0, 1);
  }
  return ret;
}

/// @brief Fill the digits with 0 to facilitate subsequent calculations,
/// inspired by the article
/// https://www.geeksforgeeks.org/karatsuba-algorithm-for-fast-multiplication-using-divide-and-conquer-algorithm/
/// @param num1: multiplier
/// @param num2: multiplier
/// @return length: max of length(num1) length(num2)
int addZeros(string &num1, string &num2) {
  int l1 = num1.size();
  int l2 = num2.size();
  if (l1 > l2) {
    for (int i = 0; i < l1 - l2; i++) {
      num2 = ('0' + num2);
    }
  } else {
    for (int i = 0; i < l2 - l1; i++) {
      num1 = ('0' + num1);
    }
  }
  return max(l1, l2);
}

/// @brief add two string bit by bit, inspired by the article
/// https://www.geeksforgeeks.org/karatsuba-algorithm-for-fast-multiplication-using-divide-and-conquer-algorithm/
/// @param s1
/// @param s2
/// @return string
string strAdd(string s1, string s2) {
  int carrier = 0;
  int bitresult = 0;
  string fullResult = "";
  string finalResult = "";
  addZeros(s1, s2);
  for (int i = s1.length() - 1; i >= 0; i--) {
    bitresult = s1[i] - '0' + s2[i] - '0' + carrier;
    carrier = 0;
    if (bitresult > 9) {
      carrier = 1;
      bitresult = bitresult - 10;
    }
    fullResult.push_back(bitresult + '0');
  }
  if (carrier == 1) {
    fullResult.push_back('1');
  }
  for (int i = 0; i < fullResult.size(); i++) {
    finalResult.push_back(fullResult[fullResult.size() - i - 1]);
  }
  // finalResult.push_back('\0');
  int pos = finalResult.find_first_not_of('0');
  if (pos != string::npos) {
    finalResult = finalResult.substr(pos, finalResult.size() - pos);
  } else {
    finalResult = "0";
  }
  return finalResult;
}

/// @brief minus two string bit by bit, inspired by the article
/// https://www.geeksforgeeks.org/karatsuba-algorithm-for-fast-multiplication-using-divide-and-conquer-algorithm/
/// @param s1
/// @param s2
/// @return string
string strMinus(string s1, string s2) {
  int carrier = 0;
  int bitresult = 0;
  string fullResult = "";
  string finalResult = "";
  addZeros(s1, s2);
  for (int i = s1.length() - 1; i >= 0; i--) {
    bitresult = s1[i] - s2[i] - carrier;
    carrier = 0;

    if (bitresult < 0) {
      carrier = 1;
      bitresult = bitresult + 10;
    }

    fullResult.push_back(bitresult + '0');
  }
  for (int i = 0; i < fullResult.size(); i++) {
    finalResult.push_back(fullResult[fullResult.size() - i - 1]);
  }
  int pos = finalResult.find_first_not_of('0');
  if (pos != string::npos) {
    finalResult = finalResult.substr(pos, finalResult.size() - pos);
  } else {
    finalResult = "0";
  }
  return finalResult;
}

/// @brief Karatsuba algorithm, inspired by the article
/// https://www.geeksforgeeks.org/karatsuba-algorithm-for-fast-multiplication-using-divide-and-conquer-algorithm/
/// @param str1
/// @param str2
/// @return string
string karatsuba(string str1, string str2) {
  int len = addZeros(str1, str2);

  if (len == 1) {
    return to_string(atoi(str1.c_str()) * atoi(str2.c_str()));
  }

  int subLen = len / 2;
  string a = str1.substr(0, subLen);
  string b = str1.substr(subLen, len - subLen);
  string c = str2.substr(0, subLen);
  string d = str2.substr(subLen, len - subLen);

  string ac = karatsuba(a, c);
  string bd = karatsuba(b, d);
  // string acPbd = strAdd(ac, bd);
  // string adPbc = strAdd(karatsuba(a, d), karatsuba(b, c));
  string aPbcPd = karatsuba(strAdd(a, b), strAdd(c, d));
  string adPbc = strMinus(aPbcPd, strAdd(ac, bd));

  int bitShift = len - subLen;
  for (int i = 0; i < bitShift * 2; i++) {
    ac = ac + '0';
  }
  for (int i = 0; i < bitShift; i++) {
    adPbc = adPbc + '0';
  }

  return strAdd(strAdd(ac, adPbc), bd);
}

/// @brief A simple multiplier with full precision
/// @param argc: int, should be 3
/// @param argv: char[], 2 input parms
/// @return state: state code 0 with output result to terminal
int main(int argc, char *argv[]) {
  string str1 = argv[1];
  string str2 = argv[2];
  // // input in debug mode with ostream:
  // cout << "input" << endl;
  // string str1 = "";
  // string str2 = "";
  // cin >> str1 >> str2;

  tuple<char *, int, int, bool> input1;
  tuple<char *, int, int, bool> input2;
  input1 = inputHandle(str1);
  input2 = inputHandle(str2);

  // calculate result - or +
  bool isNegative = get<3>(input1) ^ get<3>(input2);
  // // calculate result value or its decimal part in native way
  // string resHead = hugeMul(get<0>(input1), get<0>(input2));

  // calculate using Karatsuba algorithm, pre-processing part
  string array1 = "";
  char *out1 = get<0>(input1);
  for (int i = 0; i < sizeof(out1); i++) {
    if (out1[i]) {
      array1.push_back(out1[i]);
    }
  }
  string array2 = "";
  char *out2 = get<0>(input2);
  for (int i = 0; i < sizeof(out2); i++) {
    if (out2[i]) {
      array2.push_back(out2[i]);
    }
  }

  // calculate in Karatsuba algorithm
  string resHead = karatsuba(array1, array2);

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
