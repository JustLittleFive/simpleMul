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

int main()
{
    char demo1[20] = "hello";
    char demo2[20] = { 'h' , 'e' , 'l' , 'l' , 'o' };
    char demo3[20] = { '0' };
    char demo4[30] = {0x3C, 0x41, 0x52, 0x4D, 0x2E, 0x50, 0x4F, 0x57, 0x45, 0x52, 0x45, 0x44, 0x3E, 0x0D, 0x0A};

    for (int i = 0; i < 19; i++)
    {
        demo3[i] = i + '0';
    }
    

    std::cout << demo1 << std::endl << demo2 << std::endl << demo3 << std::endl;

    std::cout << demo4 << std::endl;

    std::cin.get();
    std::cin.get();


}