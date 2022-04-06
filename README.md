# Simple-OurC-interpreter

## Introduction.  

<img width="556" alt="截圖 2022-04-06 下午1 43 31" src="https://user-images.githubusercontent.com/85448144/161903783-3d2e1ae3-45a9-4380-b699-a896555bf58b.png">
依照上圖的文法，利用Recursive Descent Parsing，實作出interpreter.  

不斷讀取指令直到遇到'quit'，且'quit'只有在第一個token時才會被視作為停止interpreter  
  
## Token Types.  

IDENT starts with a letter and is followed by digits or letters or underlines.  
NUM is either an integer or a float number. ( Could be 1, 1.2, 0.5, .73 and 05 )  
SIGN is delimiter

## Files  

main.cpp 是全部的程式碼  
test_file.txt 是額外寫的測試檔案
