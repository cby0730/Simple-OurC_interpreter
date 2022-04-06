# Simple-OurC-interpreter

## Introduction.  

<img width="556" alt="截圖 2022-04-06 下午1 43 31" src="https://user-images.githubusercontent.com/85448144/161903783-3d2e1ae3-45a9-4380-b699-a896555bf58b.png">
依照上圖的文法，利用Recursive Descent Parsing，實作出interpreter.  

不斷讀取指令直到遇到'quit'，且'quit'只有在第一個token時才會被視作為停止interpreter  
  
## Token Types  

IDENT starts with a letter and is followed by digits or letters or underlines.  
NUM is either an integer or a float number. ( Could be 1, 1.2, 0.5, .73 and 05 )  
SIGN is delimiter

## Files  

main.cpp 是全部的程式碼  
test_file.txt 是額外寫的測試檔案

## Example  

### Input:  
<img width="442" alt="截圖 2022-04-06 下午9 39 49" src="https://user-images.githubusercontent.com/85448144/161987983-0cfae4aa-2f91-4eb9-8137-39b8b31e0bad.png">

### Output:  
<img width="313" alt="截圖 2022-04-06 下午9 40 18" src="https://user-images.githubusercontent.com/85448144/161988095-82704a55-c91b-4268-8825-2e4ecd941ad4.png">

### Input:  
<img width="307" alt="截圖 2022-04-06 下午9 47 06" src="https://user-images.githubusercontent.com/85448144/161989457-98de68d2-4751-48d8-8ab5-d5866075bc23.png">

### Output:  
<img width="450" alt="截圖 2022-04-06 下午9 47 38" src="https://user-images.githubusercontent.com/85448144/161989567-eb3e1da9-e51f-4a29-9331-b7f0ba8a372c.png">
