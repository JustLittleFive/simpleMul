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
// #include <cstring>
#include <cmath>
#include <iostream>
#include <string>

using namespace std;

/// @brief Schönhage–Strassen algorithm, inspired by the article
/// https://blog.csdn.net/u010983881/article/details/77503519
/// @param str1
/// @param str2
/// @return string
string karatsuba(string str1, string str2) {
  // 计算拆分长度
  int size1 = str1.length();
  int size2 = str2.length();
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


int main(){
  string str1 = "";
  string str2 = "";
  cout << "input: " << endl;
  cin >> str1 >> str2;
  string res = karatsuba(str1, str2);
  cout << "result: " << res << endl;
  return 0;
}