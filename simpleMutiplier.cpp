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

bool isNumber(const std::string str)
{
    double checker = 0;
    try
    {
        checker = std::stod(str);
    }
    catch(const std::exception& e)
    {
        return false;
    }
    return true;
}

int main()
{
    double  first = 1;
    double  second = 1;
    std::string str1 = "";
    std::string str2 = "";

    //hint: if string combine like "123qwe", stod() will convert it into "123"(double). 

    std::cin >> str1 >> str2;

    if (isNumber(str1) && isNumber(str2))
    {   
        first = std::stod(str1);
        second = std::stod(str2);
        std::cout << first * second  << std::endl;
        std::cin.get();
        return 0;
    }

    std::cout << "The input cannot be interpret as numbers!" << std::endl;
    std::cin.get();
    return 0;
}
